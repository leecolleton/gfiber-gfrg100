/****************************************************************************
 *
 * rg/pkg/include/be_api_gpl.h
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

#ifndef _BE_API_GPL_H_
#define _BE_API_GPL_H_

struct sk_buff;
struct ppp_softc;

typedef enum {
    PPP_PACKET_SENT = 0,
    PPP_PACKET_DROPPED = 1,
    PPP_RETRY_LATER = 2,
} ppp_send_res_t;

typedef struct {
    int (*input)(struct ppp_softc *ppp_sc, struct sk_buff *m);
    struct ppp_softc *(*be_attach)(char *ifs_name, int flags, int hdrlen,
        ppp_send_res_t (output)(void *o, struct sk_buff **skb), void *ctx);
    void (*be_detach)(struct ppp_softc *sc);
} ppp_ops_t;

int pppoatm_start(ppp_ops_t *ops);
void pppoatm_stop(void);

#ifndef __KERNEL__

 #define OS_INCLUDE_NETIF
 #include <os_includes.h>
#endif
#include <rg_ioctl.h>
#define PPPBE_ATTACH _IOW(RG_IOCTL_PREFIX_PPP, 1, char [IFNAMSIZ])
#define PPPBE_DETACH _IOW(RG_IOCTL_PREFIX_PPP, 2, void *)

typedef struct {
    char if_name[IFNAMSIZ];
    char tty_name[PATH_MAX];
} pppos_binding_t;

#define PPPOSBE_ATTACH _IOW(RG_IOCTL_PREFIX_PPPOS, 1, pppos_binding_t)

int ppposerial_start(ppp_ops_t *ops);
void ppposerial_stop(void);
void *ppposerial_attach(pppos_binding_t *binding);
int ppposerial_detach(void *ctx);

#endif
