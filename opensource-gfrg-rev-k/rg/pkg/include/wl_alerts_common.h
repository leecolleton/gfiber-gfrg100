/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/include/wl_alerts_common.h * 
 * 
 * This file is Jungo's confidential and proprietary property. 
 * This file may not be copied, 
 * distributed or otherwise used in any way without 
 * the express prior approval of Jungo Ltd. 
 * For information contact info@jungo.com
 * 
 * 
 */

#ifndef WL_ALERTS_COMMON_H
#define WL_ALERTS_COMMON_H

#define WL_ALERTS_PATH "/var/run/wl_alerts"
#define SSID_LEN 32
#define MAC_LEN  6

typedef struct {
    char ssid[SSID_LEN];
    char mac[MAC_LEN];
    char auth_ok;
} wl_alert_msg_t;

#endif
