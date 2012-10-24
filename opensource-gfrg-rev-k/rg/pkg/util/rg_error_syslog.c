/****************************************************************************
 *
 * rg/pkg/util/rg_error_syslog.c
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

#include <rg_os.h>
#define RG_ERROR_INTERNALS
#include <rg_types.h>
#include <util/rg_error.h>
#define OS_INCLUDE_IO
#define OS_INCLUDE_STD
#define OS_INCLUDE_STRING
#define OS_INCLUDE_SIGNAL
#include <os_includes.h>

#include <syslog.h>
#include "openrg_gpl.h"
#include <util/rg_error.h>
#include <util/rg_openlog.h>
#include <util/rg_error_syslog.h>

int rg_vsyslog(int entity_id, rg_error_level_t severity, reboot_func_t reboot,
    const char *format, va_list ap)
{
    rg_error_level_t _severity = severity & LLEVEL_MASK;
    int need_rg_closelog = 0;

    if (_severity)
    {
	int priority;
	static int priorities[] = {
	    LOG_EMERG,
	    LOG_ALERT,
	    LOG_CRIT,
	    LOG_ERR,
	    LOG_WARNING,
	    LOG_NOTICE,
	    LOG_INFO,
	    LOG_DEBUG,
	};

	priority = LOG_DAEMON | priorities[_severity - 1];

	if (!rg_openlog_initialized)
	{
	    /* There are some external processes that might call rg_error()
	     * indirectly (for example, by calling ipc_connect()), before
	     * rg_openlog() has been called. Since we need the entity ID in
	     * rg_error() calls, we call rg_openlog ourselves, and then close
	     * it, so the process will be able to call rg_openlog later */
	    rg_openlog_full(entity_id, NULL, LOG_CONS | LOG_NDELAY | LOG_PID,
		LOG_USER);
	    need_rg_closelog = 1;
	}
	vsyslog(priority, format, ap);
    }

    if (severity & LDOEXIT && reboot)
	reboot();

    if (need_rg_closelog)
	rg_closelog();

    return -1;
}

int rg_error_syslog(int entity_id, rg_error_level_t severity,
    reboot_func_t reboot, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = rg_vsyslog(entity_id, severity, reboot, format, ap);
    va_end(ap);

    return ret;
}

