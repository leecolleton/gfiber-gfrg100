/****************************************************************************
 *
 * rg/pkg/util/openrg_gpl.h * 
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

#ifndef _OPENRG_GPL_H_
#define _OPENRG_GPL_H_

#include <rg_types.h>

#ifndef MZERO
#define MZERO(buf) memset(&(buf), 0, sizeof(buf))
#endif

void sys_sleep(unsigned int seconds);
int sock_socket(int type, u32 src_ip, u16 src_port);
void socket_close(int fd);
int gpl_sys_rg_chrdev_open(int type, int mode);

#endif
