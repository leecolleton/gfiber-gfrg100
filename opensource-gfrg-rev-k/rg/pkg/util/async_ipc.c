/****************************************************************************
 *
 * rg/pkg/util/async_ipc.c
 * 
 * Copyright (C) Jungo LTD 2004
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02111-1307, USA.
 *
 * Developed by Jungo LTD.
 * Residential Gateway Software Division
 * www.jungo.com
 * info@jungo.com
 */

#include <async_ipc.h>
#include <ipc.h>
#include <event.h>
#include <estream.h>
#include <util/alloc.h>
#include <util/rg_memory.h>
#include <util/rg_error.h>
#define OS_INCLUDE_INET
#define OS_INCLUDE_STRING
#include <os_includes.h>

#define ASYNC_IPC_MAX_TIMEOUT_SEC 30

typedef enum {
    AIPC_WAIT_FOR_N = 1,
    AIPC_WAIT_FOR_DATA = 2,
    AIPC_WAIT_FOR_N_AND_DATA = 3,
} async_ipc_read_state_t;

typedef struct async_ipc_read_op_t {
    struct async_ipc_read_op_t *next;
    async_ipc_read_state_t state;
    int is_string;
    u32 *n;
    u8 **data;
} async_ipc_read_op_t;

struct async_ipc_t {
    async_ipc_cb_t cb;
    void *context;
    estream_t *e;
    int got_sync;
    char *sync;
    int closing;
    u32 n; /* temp variable to hold length of strings */
    async_ipc_read_op_t *queue; /* pending operations */
};

static void async_ipc_remove_op(async_ipc_read_op_t **q)
{
    async_ipc_read_op_t *next = (*q)->next;

    free(*q);
    *q = next;
}

static void async_ipc_send_sync_str(async_ipc_t *aipc)
{
    aipc->got_sync = 0;
    ZNFREE(aipc->sync);

    /* Send SYNC */
    async_ipc_string_write(aipc, RG_IPC_SYNC_STR);
    /* Schedule SYNC receive */
    async_ipc_string_read(aipc, &aipc->sync);
}

static void async_ipc_free(async_ipc_t *aipc)
{
    aipc->cb = NULL;
    if (aipc->e)
	estream_close(aipc->e);

    while (aipc->queue)
	async_ipc_remove_op(&aipc->queue);
    nfree(aipc->sync);
    free(aipc);
}

static void async_ipc_notify(async_ipc_t *aipc, int status)
{
    if (aipc->cb)
	aipc->cb(aipc->context, status);
}

static void async_ipc_read_start(async_ipc_t *aipc)
{
    if (!aipc->queue)
	return;

    estream_read(aipc->e, sizeof(*aipc->queue->n));
}

static void process_sync(async_ipc_t *aipc)
{
    if ((aipc->n != sizeof(RG_IPC_SYNC_STR) || 
	strcmp(aipc->sync, RG_IPC_SYNC_STR)))
    {
	rg_error(LERR, "Async IPC: Bad sync");
	async_ipc_notify(aipc, -1);
	return;
    }

    aipc->got_sync = 1;

    if (aipc->closing)
    {
	/* We got the sync upon end of message */
	async_ipc_notify(aipc, 0);
	async_ipc_free(aipc);
    }
    else
	async_ipc_read_start(aipc);
}

static void async_ipc_process_op(async_ipc_t *aipc)
{
    async_ipc_remove_op(&aipc->queue);

    if (!aipc->got_sync)
    {
	process_sync(aipc);
	return;
    }

    async_ipc_read_start(aipc);
    async_ipc_notify(aipc, 0);
}

static int async_ipc_estream_func(estream_t *e, u8 *data, int len)
{
    async_ipc_t *aipc = estream_get_data(e);
    async_ipc_read_op_t *op;

    if (!len)
	return 0; /* Finished writing */

    if (!(op = aipc->queue))
    {
	rg_error(LERR, "Async IPC: Received %d unexpected bytes", len);
	goto Error;
    }

    switch (op->state)
    {
	case AIPC_WAIT_FOR_N:
	case AIPC_WAIT_FOR_N_AND_DATA:
	    if (len != sizeof(*op->n))
	    {
		rg_error(LERR, "Async IPC: Received %d while reading N", len);
		goto Error;
	    }
	    /* Avoid possible misalignment of data */
	    memcpy(op->n, data, sizeof(*op->n));
	    *op->n = ntohl(*op->n);
	    if (op->state == AIPC_WAIT_FOR_N_AND_DATA && *op->n)
	    {
		op->state = AIPC_WAIT_FOR_DATA;
		estream_read(aipc->e, *op->n);
	    }
	    else
		async_ipc_process_op(aipc);
	    break;
	case AIPC_WAIT_FOR_DATA:
	    if (len != *op->n)
	    {
		rg_error(LERR, "Async IPC: Expected %d bytes, received %d",
		    *op->n, len);
		goto Error;
	    }
	    *op->data = memdup_e(data, len);
	    if (op->is_string)
	    {
		/* '\0' should be sent by the other side, we want to be on the
		 * safe side */
		(*op->data)[len-1] = 0;
	    }
	    async_ipc_process_op(aipc);
	    break;
    }

    return 0;
Error:
    async_ipc_notify(aipc, -1);
    return 0;
}

static int async_ipc_connected(estream_t *e, int connected)
{
    async_ipc_t *aipc = estream_get_data(e);

    if (!connected)
    {
	rg_error(LERR, "Async IPC: Could not connect");
	async_ipc_notify(aipc, -1);
	return 0;
    }

    estream_set_func(e, async_ipc_estream_func);
    async_ipc_read_start(aipc);
    return 0;
}

static void async_ipc_estream_timed_out(void *context)
{
    async_ipc_t *aipc = context;

    estream_close(aipc->e);
}

static void async_ipc_estream_closed(void *context)
{
    async_ipc_t *aipc = context;

    event_timer_del(async_ipc_estream_timed_out, aipc);
    aipc->e = NULL;
    if (aipc->cb)
    {
	rg_error(LERR, "Async IPC: Peer closed connection");
	async_ipc_notify(aipc, -1);
    }
}

async_ipc_t *async_ipc_open(u16 port, async_ipc_cb_t cb, void *context)
{
    async_ipc_t *aipc;
    estream_t *e = estream_connect_tcp(0, async_ipc_connected,
	htonl(INADDR_LOOPBACK), port);

    if (!e)
	return NULL;

    aipc = zalloc_e(sizeof(async_ipc_t));
    aipc->cb = cb;
    aipc->context = context;
    aipc->e = e;

    estream_set_data(e, aipc, async_ipc_estream_closed);

    event_timer_set(ASYNC_IPC_MAX_TIMEOUT_SEC*1000, async_ipc_estream_timed_out,
	aipc);

    async_ipc_send_sync_str(aipc);

    return aipc;
}

void async_ipc_u32_write(async_ipc_t *aipc, u32 n)
{
    n = htonl(n);
    estream_write(aipc->e, &n, sizeof(n));
}

void async_ipc_varbuf_write(async_ipc_t *aipc, u8 *data, int len)
{
    async_ipc_u32_write(aipc, len);
    if (len)
	estream_write(aipc->e, data, len);
}

void async_ipc_string_write(async_ipc_t *aipc, char *str)
{
    async_ipc_varbuf_write(aipc, (u8 *)str, strlen(str) + 1);
}

static void async_ipc_read(async_ipc_t *aipc, async_ipc_read_state_t state,
    int is_string, u8 **data, u32 *n)
{
    async_ipc_read_op_t **last, *op = zalloc_e(sizeof(async_ipc_read_op_t));

    op->state = state;
    op->is_string = is_string;
    op->n = n;
    op->data = data;

    /* Add to end of queue list */
    for (last = &aipc->queue; *last; last = &(*last)->next);
    *last = op;

    /* If this is the first op - start reading */
    if (last == &aipc->queue)
	async_ipc_read_start(aipc);
}

void async_ipc_u32_read(async_ipc_t *aipc, u32 *n)
{
    async_ipc_read(aipc, AIPC_WAIT_FOR_N, 0, NULL, n);
}

void async_ipc_string_read(async_ipc_t *aipc, char **str)
{
    async_ipc_read(aipc, AIPC_WAIT_FOR_N_AND_DATA, 1, (u8 **)str, &aipc->n);
}

void async_ipc_varbuf_read(async_ipc_t *aipc, u8 **data, int *len)
{
    async_ipc_read(aipc, AIPC_WAIT_FOR_N_AND_DATA, 0, data, (u32 *)len);
}

void async_ipc_close(async_ipc_t *aipc, int force)
{
    if (aipc->queue || !aipc->e || !aipc->got_sync || force)
    {
	/* Closing in the middle of IPC. May happen by either user action
	 * (disabling controlling entity), or indirectly by peer closing the
	 * socket (probably a bug).
	 */
	async_ipc_free(aipc);
	return;
    }
    /* Normal close - no more pending read operations and connection to peer
     * still alive. */
    aipc->closing = 1;
    async_ipc_send_sync_str(aipc);
}

