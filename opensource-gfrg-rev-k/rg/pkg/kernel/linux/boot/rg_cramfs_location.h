/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/rg_cramfs_location.h
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
#ifndef _RG_CRAMFS_LOCATION_H_
#define _RG_CRAMFS_LOCATION_H_

typedef enum {
    CRAMFS_LOCATION_UNKNOWN = -1,
    CRAMFS_LOCATION_RAM = 0,
    CRAMFS_LOCATION_PHYS_MAPPED_FLASH = 1,
    CRAMFS_LOCATION_UBI = 2,
} cramfs_location_t;

#endif
