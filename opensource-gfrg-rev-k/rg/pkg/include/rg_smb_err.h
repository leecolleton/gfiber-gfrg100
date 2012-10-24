/****************************************************************************
 *
 * rg/pkg/include/rg_smb_err.h
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

#ifndef _RG_SMB_ERR_H_
#define _RG_SMB_ERR_H_

typedef enum {
    SMB_ERR_NONE = 0,
    SMB_ERR_DIR_NOT_EMPTY = 1,
    SMB_ERR_NOT_DIR = 2,
    SMB_ERR_NO_SUCH_FILE = 3,
    SMB_ERR_ACCESS_DENIED = 4,
    SMB_ERR_NETWORK_ACCESS_DENIED = 5,
    SMB_ERR_OBJECT_NOT_FOUND = 6,
    SMB_ERR_CANNOT_DELETE = 7,
    SMB_ERR_CONNECTION_FAILED = 8,
    SMB_ERR_OBJECT_COLLISION = 9,
    SMB_ERR_BAD_TAR_FORMAT = 10,
    SMB_ERR_DISK_FULL = 11,
    SMB_ERR_ABORT = 12,
    SMB_ERR_TIMEOUT = 13,
    SMB_ERR_SAME_FILE = 14,
    SMB_ERR_HOST_UNREACHABLE = 15,
    SMB_ERR_NOT_FILE = 16,
    SMB_ERR_UNKNOWN = 17,
} smb_err_type_t;

#endif
