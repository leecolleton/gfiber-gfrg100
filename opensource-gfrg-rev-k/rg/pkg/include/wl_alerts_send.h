/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/include/wl_alerts_send.h * 
 * 
 * This file is Jungo's confidential and proprietary property. 
 * This file may not be copied, 
 * distributed or otherwise used in any way without 
 * the express prior approval of Jungo Ltd. 
 * For information contact info@jungo.com
 * 
 * 
 */

#ifndef WL_ALERTS_SEND
#define WL_ALERTS_SEND

#include "wl_alerts_common.h"

#ifdef __KERNEL__

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0))

#include <linux/socket.h>
#include <net/sock.h>
#include <linux/un.h>
#include <asm/uaccess.h>

void wl_alerts_send(char *ssid, char *mac, char auth_ok)
{
    static struct socket *unsock;
    struct sockaddr_un sa = {
        .sun_family = AF_UNIX,
        .sun_path = WL_ALERTS_PATH,
    };
    wl_alert_msg_t amsg;
    struct iovec iov[] = {
        {
            .iov_base = &amsg,
            .iov_len  = sizeof(amsg),
        },
    };
    struct msghdr msgh = {
        .msg_name    = &sa,
        .msg_namelen = sizeof(sa),
        .msg_flags   = 0,
        .msg_iov     = iov,
        .msg_iovlen  = 1,
    };
    mm_segment_t oldfs;

    if (!unsock) 
    {
        if (sock_create_kern(AF_UNIX, SOCK_DGRAM, 0, &unsock)) 
        {
            unsock = NULL;
            return;
        }
        unsock->sk->sk_reuse = 1;
        unsock->sk->sk_allocation = GFP_ATOMIC;
        unsock->sk->sk_data_ready = 0;
    }
    strncpy(amsg.ssid, ssid, SSID_LEN);
    memcpy(amsg.mac, mac, MAC_LEN);
    amsg.auth_ok = auth_ok;

    oldfs = get_fs(); set_fs(KERNEL_DS);
    sock_sendmsg(unsock, &msgh, iov[0].iov_len);
    set_fs(oldfs);
}

#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,4,0))

#include <linux/socket.h>
#include <net/sock.h>
#include <linux/un.h>
#include <asm/uaccess.h>

void wl_alerts_send(char *ssid, char *mac, char auth_ok)
{
    static struct socket *unsock;
    struct sockaddr_un sa = {
        .sun_family = AF_UNIX,
        .sun_path = WL_ALERTS_PATH,
    };
    wl_alert_msg_t amsg;
    struct iovec iov[] = {
        {
            .iov_base = &amsg,
            .iov_len  = sizeof(amsg),
        },
    };
    struct msghdr msgh = {
        .msg_name    = &sa,
        .msg_namelen = sizeof(sa),
        .msg_flags   = 0,
        .msg_iov     = iov,
        .msg_iovlen  = 1,
    };
    mm_segment_t oldfs;

    if (!unsock) 
    {
        if (sock_create(AF_UNIX, SOCK_DGRAM, 0, &unsock)) 
        {
            unsock = NULL;
            return;
        }
        unsock->sk->reuse = 1;
        unsock->sk->allocation = GFP_ATOMIC;
        unsock->sk->data_ready = 0;
    }
    strncpy(amsg.ssid, ssid, SSID_LEN);
    memcpy(amsg.mac, mac, MAC_LEN);
    amsg.auth_ok = auth_ok;

    oldfs = get_fs(); set_fs(KERNEL_DS);
    sock_sendmsg(unsock, &msgh, iov[0].iov_len);
    set_fs(oldfs);
}

#else
#error "Unsupported kernel"
#endif

#else /* not __KERNEL__ */

#include <sys/socket.h>
#include <sys/un.h>

void wl_alerts_send(char *ssid, char *mac, char auth_ok)
{
    struct sockaddr_un sa = {
        .sun_family = AF_UNIX,
        .sun_path = WL_ALERTS_PATH,
    };
    wl_alert_msg_t amsg;
    struct iovec iov[] = {
        {
            .iov_base = &amsg,
            .iov_len  = sizeof(amsg),
        },
    };
    struct msghdr msgh = {
        .msg_name    = &sa,
        .msg_namelen = sizeof(sa),
        .msg_flags   = 0,
        .msg_iov     = iov,
        .msg_iovlen  = 1,
    };
    static int sd = -1;
    if (sd < 0)
        sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd < 0)
        return;

    strncpy(amsg.ssid, ssid, SSID_LEN);
    memcpy(amsg.mac, mac, MAC_LEN);
    amsg.auth_ok = auth_ok;
    sendmsg(sd, &msgh, 0);
}

#endif /* __KERNEL__ */

#endif
