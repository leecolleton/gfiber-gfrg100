/****************************************************************************
 *
 * rg/pkg/ppp/kernel/be_l2tp.h
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

#ifndef _BE_L2TP_H_
#define _BE_L2TP_H_

#include <rg_ioctl.h>

typedef struct l2tp_be_params_t {
    struct in_addr src;
    struct in_addr dst;
    u16 tun_out_id;
    u16 tun_in_id;
    u16 ses_out_id;
    u16 ses_in_id;
    u16 src_port;
    u16 dst_port;
} l2tp_be_params_t;

typedef struct l2tp_be_devices_params_t {
    u32 len;
    char *devices_names;
} l2tp_be_devices_params_t;

#define L2TPSETPARAMS _IOW(RG_IOCTL_PREFIX_L2TP, 1, l2tp_be_params_t)
#define L2TPGETIDLETIME _IOW(RG_IOCTL_PREFIX_L2TP, 2, u32)
#define L2TPSETDEVICE _IOW(RG_IOCTL_PREFIX_L2TP, 3, l2tp_be_devices_params_t)

int l2tp_be_init(void);
void l2tp_be_cleanup(void);

#endif
