/****************************************************************************
 *
 * rg/pkg/hostapd/hostapd/wsc_stubs.c * 
 * 
 * Modifications by Jungo Ltd. Copyright (C) 2008 Jungo Ltd.  
 * All Rights reserved. 
 * 
 * This program is free software; 
 * you can redistribute it and/or modify it under 
 * the terms of the GNU General Public License version 2 as published by 
 * the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License v2 for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program. 
 * If not, write to the Free Software Foundation, 
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA, 
 * or contact Jungo Ltd. 
 * at http://www.jungo.com/openrg/opensource_ack.html
 * 
 * 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "hostapd.h"

int wsc_event_notify(char * pDataSend)
{
    return 0;
}

int wsc_ie_init(struct hostapd_data * hapd)
{
    return 0;
}

int wsc_ie_deinit(struct hostapd_data * hapd)
{
    return 0;
}

