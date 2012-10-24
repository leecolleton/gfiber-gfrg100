/****************************************************************************
 *
 * rg/pkg/include/voip_types.h
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

#ifndef _VOIP_TYPES_H_
#define _VOIP_TYPES_H_

#ifdef __KERNEL__
#include <linux/sched.h>
#else
#include <sched.h>
#endif

/* The following enum values are the payload-type values defined in RFC 3551 */
typedef enum {
    JRTP_PAYLOAD_PCMU = 0,
    JRTP_PAYLOAD_G726_32 = 2,
    JRTP_PAYLOAD_G723 = 4,
    JRTP_PAYLOAD_PCMA = 8,
    JRTP_PAYLOAD_G722 = 9,
    JRTP_PAYLOAD_G728 = 15,
    JRTP_PAYLOAD_G729 = 18,
    JRTP_PAYLOAD_CN = 13,
    JRTP_PAYLOAD_DTMF = 101,
    JRTP_PAYLOAD_LINPCM = 102, 
} rtp_payload_type_t;

/* udptl error correction modes */
typedef enum {
    JUDPTL_EC_NONE = 0,
    JUDPTL_EC_FEC,
    JUDPTL_EC_REDUNDANCY
} udptl_ec_mode_t;

typedef enum {
    JUDPTL_CONVERSION_NONE = 0x0,
    JUDPTL_FILL_BIT_REMOVAL = 0x1,
    JUDPTL_TRANSCODING_MMR = 0x2,
    JUDPTL_TRANSCODING_JBIG = 0x4
} udptl_conversion_options_t;

typedef enum {
    JUDPTL_LOC_TCF,
    JUDPTL_TRANS_TCF
} udptl_rate_management_method_t;

#define VOIP_HIGH_SCHED_POLICY SCHED_RR
#define VOIP_HIGH_PRIORITY 10

typedef enum {
    FAX_DETECTION_NONE = 0,
    FAX_DETECTION_ORIGINATING = 1, 
    FAX_DETECTION_TERMINATING = 2, 
    FAX_DETECTION_BOTH = 3
} fax_detection_method_t;

typedef enum {
    FAX_NONE = 0,
    FAX_T38_AUTO = 1,
    FAX_PASSTHROUGH_AUTO = 2,
    FAX_PASSTHROUGH_FORCE = 3,
} faxmethod_t;

#endif
