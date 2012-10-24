/****************************************************************************
 *
 * rg/pkg/util/rg_error.h
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
/* SYNC: rg/pkg/util/rg_error.h <-> project/tools/util/rg_error.h */

#ifndef _RG_ERROR_H_
#define _RG_ERROR_H_

#define OS_INCLUDE_STDARG
#include <os_includes.h>
#include <cc_config.h>

/* XXX - temporary, until we will have a glue layer for rg_error in mgt, which
 * will combine OpenRG's log entities with rg_error raw layer */
#include <log_entity_id.h>

#define RG_ERROR_MAX_SIZE 1024

#define LLEVEL_MASK 0xf

#define LNONE 0 /* Must be 0 */
#define LEMERG 1
#define LALERT 2
#define LCRIT 3
#define LERR 4
#define LWARNING 5
#define LNOTICE 6
#define LINFO 7
#define LDEBUG 8

/* From here on it acts as a bit field. */
#define LFLAGS_MASK 0xf00
#define LCONSOLE 0x100
#define LDOEXIT 0x200
#define LDOREBOOT 0x400

#define LDONT_FILTER 0x1000 /* Don't filter the event (ignore its priority) */
#define LNO_PREFIX 0x2000 /* Don't add level prefix to messages */

/* 16 most significant bits are user defined */
#define LUSER_MASK 0xffff0000

typedef unsigned int rg_error_level_t;

#define LEXIT (LCRIT | LDOEXIT | LCONSOLE)
#define LPANIC (LEMERG | LDOEXIT | LCONSOLE)
#define LWARN LWARNING

#ifndef CONFIG_LSP_DIST
extern char *rg_error_level_str[];

typedef void (*rg_error_func_t)(void *data, char *msg,
    int entity_id, rg_error_level_t level);

/* For backwards compatibility reasons */
typedef void (*rg_error_level_func_t)(void *data, char *msg,
    rg_error_level_t level);

#ifndef ENTITY_ID
#define ENTITY_ID 0
#endif

/* The space before the ',' is important. Dont del it ! */
#define rg_error(SEVERITY, FMT, ARGS...) \
    rg_error_full(ENTITY_ID, SEVERITY, FMT , ## ARGS)
#define rg_error_f(SEVERITY, FMT, ARGS...) \
    rg_error_full(ENTITY_ID, SEVERITY, "%s:%d: " FMT, __FUNCTION__, \
	__LINE__ , ## ARGS)
#define rg_error_full_f(ENTITY_ID, SEVERITY, FMT, ARGS...) \
    rg_error_full(ENTITY_ID, SEVERITY, "%s:%d: " FMT, __FUNCTION__, \
	__LINE__ , ## ARGS)
#define rg_verror(SEVERITY, FMT, AP) \
    rg_verror_full(ENTITY_ID, SEVERITY, FMT, AP)
#define rg_perror(SEVERITY, FMT, ARGS...) \
    rg_perror_full(ENTITY_ID, SEVERITY, FMT , ## ARGS)
#define rg_vperror(SEVERITY, FMT, AP) \
    rg_vperror_full(ENTITY_ID, SEVERITY, FMT, AP)
#define rg_error_long(SEVERITY, S) rg_error_long_full(ENTITY_ID, SEVERITY, S)

#define rg_error_errno(SEVERITY, FMT, ARGS...) \
    ({int errsv = errno; rg_error(SEVERITY, FMT, ## ARGS); -errsv; })

#ifdef CONFIG_RG_LOG_STATISTICS
#define statistics_log(ENTITY_ID, FMT, ARGS...) \
    rg_error_full(ENTITY_ID, LINFO, FMT , ##ARGS)
#else
#define statistics_log(ENTITY_ID, FMT, ARGS...)
#endif

/* the exit_func defines the actions to be performed prior to exiting.
 * It can also be used to do the exit() itself with specific exit code.
 */
void rg_error_exit_func_set(void (*exit_func)(void));

/* Call exit cb functions and exit the process */
void rg_error_exit(void);

/* Control whether all messages with level <= LERR are written to console. */
void rg_error_set_console(int on);

/* Set openrg thread_id for rg_error */
void rg_error_set_mt_id(void);

/* Every registered function gets a reference to every log message from the
 * level specified and up.
 */
void default_error_cb(void *data, char *msg, rg_error_level_t level);

void rg_error_register(int sequence, rg_error_func_t func, void *data);
void rg_error_unregister(rg_error_func_t func, void *data);

/* Similar to rg_error_register(), but the function it registers is
 * of type rg_error_level_func_t. This was added especially for backwards
 * compatibility reasons */
void rg_error_register_level(int sequence, rg_error_level_t level,
    rg_error_level_func_t func, void *data);
void rg_error_unregister_level(rg_error_level_func_t func, void *data);

int rg_error_full(int entity_id, rg_error_level_t level,
    const char *format, ...)__attribute__((__format__(__printf__, 3, 4)));
int rg_verror_full(int entity_id, rg_error_level_t level,
    const char *format, va_list ap)__attribute__((nonnull(4)));
int rg_perror_full(int entity_id, rg_error_level_t level,
    const char *format, ...)__attribute__((__format__(__printf__, 3, 4)));
int rg_vperror_full(int entity_id, rg_error_level_t level,
    const char *format, va_list ap)__attribute__((nonnull(4)));

/* Similar to rg_error_full(), but can deal with long strings (larger than
 * RG_ERROR_MAX_SIZE). */
int rg_error_long_full(int entity_id, rg_error_level_t level, char *s);

/* Show stack trace */
void strace_write(void);

#ifdef RG_ERROR_INTERNALS
/* Deliver log message directly, must be used only within openrg */
void log_msg_deliver(int entity_id, rg_error_level_t level, char *msg);
#endif

void rg_error_init(int _reboot_on_exit, int _reboot_on_panic,
    int strace, char *process, int (*_main_addr)(int, char *[]),
    const char *_main_name);
void rg_error_init_default(rg_error_level_t level);
void rg_error_init_nodefault(void);

#else
/* LSP */
#include <stdio.h>
#define rg_error(lev, s...) ( { printf("<%d> ", lev); printf(s); -1; } )
#define rg_perror(lev, s...) ( { rg_error(lev, s); } )
#define rg_error_f(lev, s...) ( { rg_error(lev, s); } )
#define rg_error_errno(lev, s...) ( { rg_error(lev, s); } )
#endif
#ifdef __KERNEL__
extern int (*rg_error_logdev_hook)(char *msg, int exact_len);
#endif

#endif

