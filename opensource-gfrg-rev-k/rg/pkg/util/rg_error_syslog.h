/****************************************************************************
 *
 * rg/pkg/util/rg_error_syslog.h
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
#ifndef _RG_ERROR_SYSLOG_H_
#define _RG_ERROR_SYSLOG_H_

#include <util/rg_error.h>

typedef void (*reboot_func_t)(void);

int rg_vsyslog(int entity_id, rg_error_level_t severity, reboot_func_t reboot,
    const char *format, va_list ap);
int rg_error_syslog(int entity_id, rg_error_level_t severity,
    reboot_func_t reboot, const char *format, ...);

#endif
