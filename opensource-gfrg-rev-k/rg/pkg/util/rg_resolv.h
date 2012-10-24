/****************************************************************************
 *
 * rg/pkg/util/rg_resolv.h * 
 * 
 * Modifications by Jungo Ltd. Copyright (C) 2008 Jungo Ltd.  
 * All Rights reserved. 
 * 
 * This library is free software; 
 * you can redistribute it and/or modify it under 
 * the terms of the GNU Lesser General Public License version 2.1 
 * as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU Lesser General Public License v2.1 for more details.
 * 
 * You should have received a copy of the 
 * GNU Lesser General Public License along with this library. 
 * If not, write to the Free Software Foundation, 
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 * or contact Jungo Ltd. 
 * at http://www.jungo.com/openrg/opensource_ack.html.
 * 
 */

#ifndef _RG_RESOLV_H_
#define _RG_RESOLV_H_

/* DNS services that use OpenRG resolver (IP 127.0.0.1) and allow setting
 * skb->mark to DNS requests */
extern struct __res_state _rg_res;

struct hostent;
struct hostent *rg_gethostbyname(const char *name, int so_mark);
int rg_gethostbyname_r(const char * name,
    struct hostent * result_buf,
    char * buf, size_t buflen,
    struct hostent ** result,
    int * h_errnop,
    int so_mark);
int rg_res_query(const char *dname, int class, int type,
    unsigned char *answer, int anslen, int so_mark);
int rg_res_querydomain(const char *name, const char *domain, int class, 
    int type, unsigned char *answer, int anslen, int so_mark);
int rg_res_init(void);
void rg_res_close(void);
int rg_res_search(const char *name, int class, int type, 
    unsigned char *answer, int anslen, int so_mark);

#endif
