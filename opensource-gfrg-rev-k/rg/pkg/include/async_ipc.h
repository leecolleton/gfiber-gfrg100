/****************************************************************************
 *
 * rg/pkg/include/async_ipc.h
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

#ifndef _ASYNC_IPC_H_
#define _ASYNC_IPC_H_

#include <rg_types.h>

typedef struct async_ipc_t async_ipc_t;

/* Callback function, called when the message was sent, received or on error.
 * status is 0 on success, non-zero otherwise. */
typedef void (*async_ipc_cb_t)(void *context, int status);

/* Create an async IPC object, open the socket and starts a connect. */
async_ipc_t *async_ipc_open(u16 port, async_ipc_cb_t cb, void *context);

/* Add a u32 to the data to be sent. */
void async_ipc_u32_write(async_ipc_t *aipc, u32 n);

/* Add a string to the data to be sent. */
void async_ipc_string_write(async_ipc_t *aipc, char *str);

/* Add a binary buffer to the data to be sent. */
void async_ipc_varbuf_write(async_ipc_t *aipc, u8 *data, int len);

/* Start reading a u32 value. When done, the read value is stored in the
 * location pointed to by n, and the callback is called. */
void async_ipc_u32_read(async_ipc_t *aipc, u32 *n);

/* Start reading a string value. When done, the read value is allocated and
 * stored in the location pointed to by str, and the callback is called. The
 * caller is responsible to free *str (upon success). */
void async_ipc_string_read(async_ipc_t *aipc, char **str);

/* Start reading a varbuf value. When done, the read value is allocated and
 * stored in the location pointed to by data, its length is stored in the
 * location pointed to by len, and the callback is called. The caller is
 * responsible to free *data (upon success). */
void async_ipc_varbuf_read(async_ipc_t *aipc, u8 **data, int *len);

/* XXX: what is force used for ? is it required ? */
void async_ipc_close(async_ipc_t *aipc, int force);

#endif

