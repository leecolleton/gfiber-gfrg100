/****************************************************************************
 *
 * rg/pkg/util/rg_openlog.h
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

#ifndef _RG_OPENLOG_H_
#define _RG_OPENLOG_H_

/* XXX - temporary, until we will have a glue layer for rg_error in mgt, which
 * will combine OpenRG's log entities with rg_error raw layer */
#include <log_entity_id.h>

#define rg_openlog(IDENT, OPTION, FACILITY) \
    rg_openlog_full(ENTITY_ID, IDENT, OPTION, FACILITY)

extern int rg_openlog_initialized;

void rg_openlog_full(int entity_id, const char *ident, int option,
    int facility);
void rg_closelog(void);

#endif

