#include <rg_config.h>

#ifdef CONFIG_RG_LOCAL_RESOLVE_BY_SO_MARK
/* Resolve by:
 * - Sending request to loopback (127.0.0.1).
 * - Allows setting so_mark that is sent with request's skb.
 * Code is taken from ulibc (ulibc/libc/inet/resolv.c) and trimmed a little.
 */

/* resolv.c: DNS Resolver
 *
 * Copyright (C) 1998  Kenneth Albanowski <kjahds@kjahds.com>,
 *                     The Silver Hammer Group, Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 */

/*
 * Portions Copyright (c) 1985, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * Portions Copyright (c) 1996-1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 *
 *  5-Oct-2000 W. Greathouse  wgreathouse@smva.com
 *                              Fix memory leak and memory corruption.
 *                              -- Every name resolution resulted in
 *                                 a new parse of resolv.conf and new
 *                                 copy of nameservers allocated by
 *                                 strdup.
 *                              -- Every name resolution resulted in
 *                                 a new read of resolv.conf without
 *                                 resetting index from prior read...
 *                                 resulting in exceeding array bounds.
 *
 *                              Limit nameservers read from resolv.conf
 *
 *                              Add "search" domains from resolv.conf
 *
 *                              Some systems will return a security
 *                              signature along with query answer for
 *                              dynamic DNS entries.
 *                              -- skip/ignore this answer
 *
 *                              Include arpa/nameser.h for defines.
 *
 *                              General cleanup
 *
 * 20-Jun-2001 Michal Moskal <malekith@pld.org.pl>
 *   partial IPv6 support (i.e. gethostbyname2() and resolve_address2()
 *   functions added), IPv6 nameservers are also supported.
 *
 * 6-Oct-2001 Jari Korva <jari.korva@iki.fi>
 *   more IPv6 support (IPv6 support for gethostbyaddr();
 *   address family parameter and improved IPv6 support for get_hosts_byname
 *   and read_etc_hosts; getnameinfo() port from glibc; defined
 *   defined ip6addr_any and in6addr_loopback)
 *
 * 2-Feb-2002 Erik Andersen <andersen@codepoet.org>
 *   Added gethostent(), sethostent(), and endhostent()
 *
 * 17-Aug-2002 Manuel Novoa III <mjn3@codepoet.org>
 *   Fixed __read_etc_hosts_r to return alias list, and modified buffer
 *   allocation accordingly.  See MAX_ALIASES and ALIAS_DIM below.
 *   This fixes the segfault in the Python 2.2.1 socket test.
 *
 * 04-Jan-2003 Jay Kulpinski <jskulpin@berkshire.rr.com>
 *   Fixed __decode_dotted to count the terminating null character
 *   in a host name.
 *
 * 02-Oct-2003 Tony J. White <tjw@tjw.org>
 *   Lifted dn_expand() and dependent ns_name_uncompress(), ns_name_unpack(),
 *   and ns_name_ntop() from glibc 2.3.2 for compatibility with ipsec-tools
 *   and openldap.
 *
 * 7-Sep-2004 Erik Andersen <andersen@codepoet.org>
 *   Added gethostent_r()
 *
 */

#define __FORCE_GLIBC
#include <libc-symbols.h>
#include <features.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <signal.h>
#include <bits/errno_values.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <resolv.h>
#include <netdb.h>
#include <ctype.h>
#include <stdbool.h>
#include <arpa/nameser.h>
#include <sys/utsname.h>
#include <sys/un.h>
#include "rg_resolv.h"

/* Defined at toolchain. At all the platforms OpenRG support this is 36.
 * Taken from linux/include/asm-%s/socket.h */
#ifndef SO_MARK
#define SO_MARK 36
#endif

#define __RG_MUTEX_LOCK(M)						\
    do {								 \
        int oldtype; \
	pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, &(M));			\
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype); \
	pthread_mutex_lock(&(M));					\
	((void)0)

#define __RG_MUTEX_UNLOCK(M)								\
	pthread_setcanceltype(oldtype, NULL); \
        pthread_cleanup_pop(1);	\
    } while (0)
#define __RG_MUTEX_STATIC(M,I) static pthread_mutex_t M = I

__RG_MUTEX_STATIC(__resolv_lock, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP);

#define L_gethostbyname
#define L_gethostbyname_r
#define L_encoded
#define L_decoded
#define L_encodeq
#define L_dnslookup
#define L_res_init
#define L_res_query
#define L_encodeh
#define L_decodeh
#define L_decodea
#define L_lengthq
#define L_lengthd




#define MAX_RECURSE 5
#define REPLY_TIMEOUT 10
#define MAX_RETRIES 3
#define MAX_SERVERS 3
#define MAX_SEARCH 4

#define MAX_ALIASES	5

/* 1:ip + 1:full + MAX_ALIASES:aliases + 1:NULL */
#define ALIAS_DIM		(2 + MAX_ALIASES + 1)

#undef DEBUG
/* #define DEBUG */

#ifdef DEBUG
#define DPRINTF(X,args...) fprintf(stderr, X, ##args)
#else
#define DPRINTF(X,args...)
#endif /* DEBUG */

static int __nameservers ;
static char  *__nameserver[MAX_SERVERS];

/* Global stuff (stuff needing to be locked to be thread safe)... */
static int __searchdomains;
static char * __searchdomain[MAX_SEARCH];

/* Structs */
struct resolv_header {
	int id;
	int qr,opcode,aa,tc,rd,ra,rcode;
	int qdcount;
	int ancount;
	int nscount;
	int arcount;
};

struct resolv_question {
	char * dotted;
	int qtype;
	int qclass;
};

struct resolv_answer {
	char * dotted;
	int atype;
	int aclass;
	int ttl;
	int rdlength;
	const unsigned char * rdata;
	int rdoffset;
	char* buf;
	size_t buflen;
	size_t add_count;
};

enum etc_hosts_action {
    GET_HOSTS_BYNAME = 0,
    GETHOSTENT,
    GET_HOSTS_BYADDR,
};


#ifdef L_encodeh
static int __encode_header(struct resolv_header *h, unsigned char *dest, int maxlen)
{
	if (maxlen < HFIXEDSZ)
		return -1;

	dest[0] = (h->id & 0xff00) >> 8;
	dest[1] = (h->id & 0x00ff) >> 0;
	dest[2] = (h->qr ? 0x80 : 0) |
		((h->opcode & 0x0f) << 3) |
		(h->aa ? 0x04 : 0) |
		(h->tc ? 0x02 : 0) |
		(h->rd ? 0x01 : 0);
	dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0f);
	dest[4] = (h->qdcount & 0xff00) >> 8;
	dest[5] = (h->qdcount & 0x00ff) >> 0;
	dest[6] = (h->ancount & 0xff00) >> 8;
	dest[7] = (h->ancount & 0x00ff) >> 0;
	dest[8] = (h->nscount & 0xff00) >> 8;
	dest[9] = (h->nscount & 0x00ff) >> 0;
	dest[10] = (h->arcount & 0xff00) >> 8;
	dest[11] = (h->arcount & 0x00ff) >> 0;

	return HFIXEDSZ;
}
#endif

#ifdef L_decodeh
static int __decode_header(unsigned char *data, struct resolv_header *h)
{
	h->id = (data[0] << 8) | data[1];
	h->qr = (data[2] & 0x80) ? 1 : 0;
	h->opcode = (data[2] >> 3) & 0x0f;
	h->aa = (data[2] & 0x04) ? 1 : 0;
	h->tc = (data[2] & 0x02) ? 1 : 0;
	h->rd = (data[2] & 0x01) ? 1 : 0;
	h->ra = (data[3] & 0x80) ? 1 : 0;
	h->rcode = data[3] & 0x0f;
	h->qdcount = (data[4] << 8) | data[5];
	h->ancount = (data[6] << 8) | data[7];
	h->nscount = (data[8] << 8) | data[9];
	h->arcount = (data[10] << 8) | data[11];

	return HFIXEDSZ;
}
#endif

#ifdef L_encoded
/* Encode a dotted string into nameserver transport-level encoding.
   This routine is fairly dumb, and doesn't attempt to compress
   the data */

static int __encode_dotted(const char *dotted, unsigned char *dest, int maxlen)
{
	unsigned used = 0;

	while (dotted && *dotted) {
		char *c = strchr(dotted, '.');
		int l = c ? c - dotted : strlen(dotted);

		if (l >= (maxlen - used - 1))
			return -1;

		dest[used++] = l;
		memcpy(dest + used, dotted, l);
		used += l;

		if (c)
			dotted = c + 1;
		else
			break;
	}

	if (maxlen < 1)
		return -1;

	dest[used++] = 0;

	return used;
}
#endif

#ifdef L_decoded
/* Decode a dotted string from nameserver transport-level encoding.
   This routine understands compressed data. */

static int __decode_dotted(const unsigned char * const data, int offset,
				  char *dest, int maxlen)
{
	int l;
	bool measure = 1;
	unsigned total = 0;
	unsigned used = 0;

	if (!data)
		return -1;

	while ((l=data[offset++])) {
		if (measure)
		    total++;
		if ((l & 0xc0) == (0xc0)) {
			if (measure)
				total++;
			/* compressed item, redirect */
			offset = ((l & 0x3f) << 8) | data[offset];
			measure = 0;
			continue;
		}

		if ((used + l + 1) >= maxlen)
			return -1;

		memcpy(dest + used, data + offset, l);
		offset += l;
		used += l;
		if (measure)
			total += l;

		if (data[offset] != 0)
			dest[used++] = '.';
		else
			dest[used++] = '\0';
	}

	/* The null byte must be counted too */
	if (measure) {
	    total++;
	}

	DPRINTF("Total decode len = %d\n", total);

	return total;
}
#endif

#ifdef L_lengthd
static int __length_dotted(const unsigned char * const data, int offset)
{
	int orig_offset = offset;
	int l;

	if (!data)
		return -1;

	while ((l = data[offset++])) {

		if ((l & 0xc0) == (0xc0)) {
			offset++;
			break;
		}

		offset += l;
	}

	return offset - orig_offset;
}
#endif

#ifdef L_encodeq
static int __encode_question(const struct resolv_question * const q,
					unsigned char *dest, int maxlen)
{
	int i;

	i = __encode_dotted(q->dotted, dest, maxlen);
	if (i < 0)
		return i;

	dest += i;
	maxlen -= i;

	if (maxlen < 4)
		return -1;

	dest[0] = (q->qtype & 0xff00) >> 8;
	dest[1] = (q->qtype & 0x00ff) >> 0;
	dest[2] = (q->qclass & 0xff00) >> 8;
	dest[3] = (q->qclass & 0x00ff) >> 0;

	return i + 4;
}
#endif

#ifdef L_lengthq
static int __length_question(const unsigned char * const message, int offset)
{
	int i;

	i = __length_dotted(message, offset);
	if (i < 0)
		return i;

	return i + 4;
}
#endif

#ifdef L_decodea
static int __decode_answer(const unsigned char *message, int offset,
				  struct resolv_answer *a)
{
	char temp[256];
	int i;

	i = __decode_dotted(message, offset, temp, sizeof(temp));
	if (i < 0)
		return i;

	message += offset + i;

	a->dotted = strdup(temp);
	a->atype = (message[0] << 8) | message[1];
	message += 2;
	a->aclass = (message[0] << 8) | message[1];
	message += 2;
	a->ttl = (message[0] << 24) |
		(message[1] << 16) | (message[2] << 8) | (message[3] << 0);
	message += 4;
	a->rdlength = (message[0] << 8) | message[1];
	message += 2;
	a->rdata = message;
	a->rdoffset = offset + i + RRFIXEDSZ;

	DPRINTF("i=%d,rdlength=%d\n", i, a->rdlength);

	return i + RRFIXEDSZ + a->rdlength;
}
#endif

#ifdef L_dnslookup
__RG_MUTEX_STATIC(mylock, PTHREAD_MUTEX_INITIALIZER);

/* Just for the record, having to lock __dns_lookup() just for these two globals
 * is pretty lame.  I think these two variables can probably be de-global-ized,
 * which should eliminate the need for doing locking here...  Needs a closer
 * look anyways. */
static int ns=0, id=1;

static int __dns_lookup(const char *name, int type, int nscount, char **nsip,
			   unsigned char **outpacket, struct resolv_answer *a,
			   int so_mark)
{
	int i, j, len, fd, pos, rc;
	struct timeval tv;
	fd_set fds;
	struct resolv_header h;
	struct resolv_question q;
	struct resolv_answer ma;
	bool first_answer = 1;
	unsigned retries = 0;
	unsigned char * packet = malloc(PACKETSZ);
	char *dns, *lookup = malloc(MAXDNAME);
	int variant = -1;
	struct sockaddr_in sa;
	int local_ns = -1, local_id = -1;
#ifdef __UCLIBC_HAS_IPV6__
	bool v6;
	struct sockaddr_in6 sa6;
#endif

	fd = -1;

	if (!packet || !lookup || !nscount)
	    goto fail;

	DPRINTF("Looking up type %d answer for '%s'\n", type, name);

	/* Mess with globals while under lock */
	__RG_MUTEX_LOCK(mylock);
	local_ns = ns % nscount;
	local_id = id;
	__RG_MUTEX_UNLOCK(mylock);

	while (retries < MAX_RETRIES) {
		if (fd != -1)
			close(fd);

		memset(packet, 0, PACKETSZ);

		memset(&h, 0, sizeof(h));

		++local_id;
		local_id &= 0xffff;
		h.id = local_id;
		__RG_MUTEX_LOCK(__resolv_lock);
		/* this is really __nameserver[] which is a global that
		   needs to hold __resolv_lock before access!! */
		dns = nsip[local_ns];
		__RG_MUTEX_UNLOCK(__resolv_lock);

		h.qdcount = 1;
		h.rd = 1;

		DPRINTF("encoding header\n", h.rd);

		i = __encode_header(&h, packet, PACKETSZ);
		if (i < 0)
			goto fail;

		strncpy(lookup,name,MAXDNAME);
		if (variant >= 0) {
			__RG_MUTEX_LOCK(__resolv_lock);
			if (variant < __searchdomains) {
				strncat(lookup,".", MAXDNAME);
				strncat(lookup,__searchdomain[variant], MAXDNAME);
			}
			__RG_MUTEX_UNLOCK(__resolv_lock);
		}
		DPRINTF("lookup name: %s\n", lookup);
		q.dotted = (char *)lookup;
		q.qtype = type;
		q.qclass = C_IN; /* CLASS_IN */

		j = __encode_question(&q, packet+i, PACKETSZ-i);
		if (j < 0)
			goto fail;

		len = i + j;

		DPRINTF("On try %d, sending query to port %d of machine %s\n",
				retries+1, NAMESERVER_PORT, dns);

#ifdef __UCLIBC_HAS_IPV6__
		__RG_MUTEX_LOCK(__resolv_lock);
		/* 'dns' is really __nameserver[] which is a global that
		   needs to hold __resolv_lock before access!! */
		v6 = inet_pton(AF_INET6, dns, &sa6.sin6_addr) > 0;
		__RG_MUTEX_UNLOCK(__resolv_lock);
		fd = socket(v6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
		fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif
		if (fd < 0) {
			retries++;
		    continue;
		}
		if (so_mark)
		{
		    setsockopt(fd, SOL_SOCKET, SO_MARK, (int *)&so_mark,
			sizeof(int));
		}

		/* Connect to the UDP socket so that asyncronous errors are returned */
#ifdef __UCLIBC_HAS_IPV6__
		if (v6) {
		    sa6.sin6_family = AF_INET6;
		    sa6.sin6_port = htons(NAMESERVER_PORT);
		    /* sa6.sin6_addr is already here */
		    rc = connect(fd, (struct sockaddr *) &sa6, sizeof(sa6));
		} else {
#endif
		    sa.sin_family = AF_INET;
		    sa.sin_port = htons(NAMESERVER_PORT);
		    __RG_MUTEX_LOCK(__resolv_lock);
		    /* 'dns' is really __nameserver[] which is a global that
		       needs to hold __resolv_lock before access!! */
		    sa.sin_addr.s_addr = inet_addr(dns);
		    __RG_MUTEX_UNLOCK(__resolv_lock);
		    rc = connect(fd, (struct sockaddr *) &sa, sizeof(sa));
#ifdef __UCLIBC_HAS_IPV6__
		}
#endif
		if (rc < 0) {
		        /* We always access 127.0.0.1, hence expect to succeed */
		        retries++;
			continue;
		}

		DPRINTF("Transmitting packet of length %d, id=%d, qr=%d\n",
				len, h.id, h.qr);

		send(fd, packet, len, 0);

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = REPLY_TIMEOUT;
		tv.tv_usec = 0;
		if (select(fd + 1, &fds, NULL, NULL, &tv) <= 0) {
		    DPRINTF("Timeout\n");

			/* timed out, so retry send and receive,
			 * to next nameserver on queue */
			goto tryall;
		}

		len = recv(fd, packet, 512, 0);
		if (len < HFIXEDSZ) {
			/* too short ! */
			goto again;
		}

		__decode_header(packet, &h);

		DPRINTF("id = %d, qr = %d\n", h.id, h.qr);

		if ((h.id != local_id) || (!h.qr)) {
			/* unsolicited */
			goto again;
		}


		DPRINTF("Got response %s\n", "(i think)!");
		DPRINTF("qrcount=%d,ancount=%d,nscount=%d,arcount=%d\n",
				h.qdcount, h.ancount, h.nscount, h.arcount);
		DPRINTF("opcode=%d,aa=%d,tc=%d,rd=%d,ra=%d,rcode=%d\n",
				h.opcode, h.aa, h.tc, h.rd, h.ra, h.rcode);

		if ((h.rcode) || (h.ancount < 1)) {
			/* negative result, not present */
			goto again;
		}

		pos = HFIXEDSZ;

		for (j = 0; j < h.qdcount; j++) {
			DPRINTF("Skipping question %d at %d\n", j, pos);
			i = __length_question(packet, pos);
			DPRINTF("Length of question %d is %d\n", j, i);
			if (i < 0)
				goto again;
			pos += i;
		}
		DPRINTF("Decoding answer at pos %d\n", pos);

		first_answer = 1;
		for (j=0;j<h.ancount;j++,pos += i)
			{
				i = __decode_answer(packet, pos, &ma);

				if (i<0) {
					DPRINTF("failed decode %d\n", i);
					goto again;
				}

				if ( first_answer )
					{
						ma.buf = a->buf;
						ma.buflen = a->buflen;
						ma.add_count = a->add_count;
						memcpy(a, &ma, sizeof(ma));
						if (a->atype != T_SIG && (0 == a->buf || (type != T_A && type != T_AAAA)))
							{
								break;
							}
						if (a->atype != type)
							{
								free(a->dotted);
								continue;
							}
						a->add_count = h.ancount - j - 1;
						if ((a->rdlength + sizeof(struct in_addr*)) * a->add_count > a->buflen)
							{
								break;
							}
						a->add_count = 0;
						first_answer = 0;
					}
				else
					{
						free(ma.dotted);
						if (ma.atype != type)
							{
								continue;
							}
						if (a->rdlength != ma.rdlength)
							{
								free(a->dotted);
								DPRINTF("Answer address len(%u) differs from original(%u)\n",
										ma.rdlength, a->rdlength);
								goto again;
							}
						memcpy(a->buf + (a->add_count * ma.rdlength), ma.rdata, ma.rdlength);
						++a->add_count;
					}
			}

		DPRINTF("Answer name = |%s|\n", a->dotted);
		DPRINTF("Answer type = |%d|\n", a->atype);

		close(fd);

		if (outpacket)
			*outpacket = packet;
		else
			free(packet);
		free(lookup);

		/* Mess with globals while under lock */
		__RG_MUTEX_LOCK(mylock);
		ns = local_ns;
		id = local_id;
		__RG_MUTEX_UNLOCK(mylock);

		return (len);				/* success! */

	tryall:
		/* if there are other nameservers, give them a go,
		   otherwise return with error */
		{
		    variant = -1;
			local_ns = (local_ns + 1) % nscount;
			if (local_ns == 0)
				retries++;

			continue;
		}

	again:
		/* if there are searchdomains, try them or fallback as passed */
		{
		    int sdomains;
		    __RG_MUTEX_LOCK(__resolv_lock);
		    sdomains=__searchdomains;
		    __RG_MUTEX_UNLOCK(__resolv_lock);

		    if (variant < sdomains - 1) {
				/* next search */
				variant++;
		    } else {
				/* next server, first search */
				local_ns = (local_ns + 1) % nscount;
				if (local_ns == 0)
					retries++;

				variant = -1;
		    }
		}
	}

 fail:
	if (fd != -1)
	    close(fd);
	if (lookup)
	    free(lookup);
	if (packet)
	    free(packet);
	h_errno = NETDB_INTERNAL;
	/* Mess with globals while under lock */
	if (local_ns != -1) {
	    __RG_MUTEX_LOCK(mylock);
	    ns = local_ns;
	    id = local_id;
	    __RG_MUTEX_UNLOCK(mylock);
	}
	return -1;
}
#endif

static void __open_nameservers(void)
{
	if (!*__nameserver)
	{
	    /* OpenRG resolver address */
	    __nameserver[0] = "127.0.0.1";
	    __nameservers = 1;
	}
}

static void __close_nameservers(void)
{
}

#ifdef L_gethostbyname

struct hostent *rg_gethostbyname(const char *name, int so_mark)
{
	static struct hostent h;
	static char buf[sizeof(struct in_addr) +
					sizeof(struct in_addr *)*2 +
					sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
	struct hostent *hp;

	rg_gethostbyname_r(name, &h, buf, sizeof(buf), &hp, &h_errno,
	    so_mark);

	return hp;
}
#endif



#ifdef L_res_init
/* We use __resolv_lock to guard access to global '_rg_res' */
struct __res_state _rg_res;

int rg_res_init(void)
{
	struct __res_state *rp = &(_rg_res);

	__RG_MUTEX_LOCK(__resolv_lock);	/* must be a recursive lock! */
	__close_nameservers();
	__open_nameservers();
	rp->retrans = RES_TIMEOUT;
	rp->retry = 4;
	rp->options = RES_INIT;
	rp->id = (u_int) random();
	rp->nsaddr.sin_addr.s_addr = INADDR_ANY;
	rp->nsaddr.sin_family = AF_INET;
	rp->nsaddr.sin_port = htons(NAMESERVER_PORT);
	rp->ndots = 1;
	/** rp->pfcode = 0; **/
	rp->_vcsock = -1;
	/** rp->_flags = 0; **/
	/** rp->qhook = NULL; **/
	/** rp->rhook = NULL; **/
	/** rp->_u._ext.nsinit = 0; **/

	if(__searchdomains) {
		int i;
		for(i=0; i<__searchdomains; i++) {
			rp->dnsrch[i] = __searchdomain[i];
		}
	}

	if(__nameservers) {
		int i;
		struct in_addr a;
		for(i=0; i<__nameservers; i++) {
			if (inet_aton(__nameserver[i], &a)) {
				rp->nsaddr_list[i].sin_addr = a;
				rp->nsaddr_list[i].sin_family = AF_INET;
				rp->nsaddr_list[i].sin_port = htons(NAMESERVER_PORT);
			}
		}
	}
	rp->nscount = __nameservers;
	__RG_MUTEX_UNLOCK(__resolv_lock);

	return(0);
}

void rg_res_close( void )
{
	__close_nameservers();
	memset(&_rg_res, 0, sizeof(_rg_res));
}

#endif


#ifdef L_res_query

#ifndef MIN
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#endif

int rg_res_query(const char *dname, int class, int type,
              unsigned char *answer, int anslen,
	      int so_mark)
{
	int i;
	unsigned char * packet = 0;
	struct resolv_answer a;
	int __nameserversXX;
	char ** __nameserverXX;

	__open_nameservers();
	if (!dname || class != 1 /* CLASS_IN */) {
		h_errno = NO_RECOVERY;
		return(-1);
	}

	memset((char *) &a, '\0', sizeof(a));

	__RG_MUTEX_LOCK(__resolv_lock);
	__nameserversXX=__nameservers;
	__nameserverXX=__nameserver;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	i = __dns_lookup(dname, type, __nameserversXX, __nameserverXX, &packet,
	    &a, so_mark);

	if (i < 0) {
		h_errno = TRY_AGAIN;
		return(-1);
	}

	free(a.dotted);

	if (a.atype == type) { /* CNAME*/
		int len = MIN(anslen, i);
		memcpy(answer, packet, len);
		if (packet)
			free(packet);
		return(len);
	}
	if (packet)
		free(packet);
	return i;
}

/*
 * Formulate a normal query, send, and retrieve answer in supplied buffer.
 * Return the size of the response on success, -1 on error.
 * If enabled, implement search rules until answer or unrecoverable failure
 * is detected.  Error code, if any, is left in h_errno.
 */
#define __TRAILING_DOT	(1<<0)
#define __GOT_NODATA	(1<<1)
#define __GOT_SERVFAIL	(1<<2)
#define __TRIED_AS_IS	(1<<3)
int rg_res_search(name, class, type, answer, anslen, so_mark)
	 const char *name;	/* domain name */
	 int class, type;	/* class and type of query */
	 u_char *answer;		/* buffer to put answer */
	 int anslen;		/* size of answer */
	 int so_mark;           /* skb->mark of created query */
{
	const char *cp, * const *domain;
	HEADER *hp = (HEADER *)(void *)answer;
	u_int dots;
	unsigned _state = 0;
	int ret, saved_herrno;
	u_long _res_options;
	unsigned _res_ndots;
	char **_res_dnsrch;

	__RG_MUTEX_LOCK(__resolv_lock);
	_res_options = _rg_res.options;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	if ((!name || !answer) || ((_res_options & RES_INIT) == 0 && rg_res_init() == -1)) {
		h_errno = NETDB_INTERNAL;
		return (-1);
	}

	h_errno = HOST_NOT_FOUND;	/* default, if we never query */
	dots = 0;
	for (cp = name; *cp; cp++)
		dots += (*cp == '.');

	if (cp > name && *--cp == '.')
		_state |= __TRAILING_DOT;

	/*
	 * If there are dots in the name already, let's just give it a try
	 * 'as is'.  The threshold can be set with the "ndots" option.
	 */
	saved_herrno = -1;
	__RG_MUTEX_LOCK(__resolv_lock);
	_res_ndots = _rg_res.ndots;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	if (dots >= _res_ndots) {
		ret = rg_res_querydomain(name, NULL, class, type, answer,
		    anslen, so_mark);
		if (ret > 0)
			return (ret);
		saved_herrno = h_errno;
		_state |= __TRIED_AS_IS;
	}

	/*
	 * We do at least one level of search if
	 *	- there is no dot and RES_DEFNAME is set, or
	 *	- there is at least one dot, there is no trailing dot,
	 *	  and RES_DNSRCH is set.
	 */
	__RG_MUTEX_LOCK(__resolv_lock);
	_res_options = _rg_res.options;
	_res_dnsrch = _rg_res.dnsrch;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	if ((!dots && (_res_options & RES_DEFNAMES)) ||
	    (dots && !(_state & __TRAILING_DOT) && (_res_options & RES_DNSRCH))) {
		bool done = 0;

		for (domain = (const char * const *)_res_dnsrch;
			 *domain && !done;
			 domain++) {

			ret = rg_res_querydomain(name, *domain, class, type,
						answer, anslen, so_mark);
			if (ret > 0)
				return (ret);

			switch (h_errno) {
				case NO_DATA:
					_state |= __GOT_NODATA;
					/* FALLTHROUGH */
				case HOST_NOT_FOUND:
					/* keep trying */
					break;
				case TRY_AGAIN:
					if (hp->rcode == SERVFAIL) {
						/* try next search element, if any */
						_state |= __GOT_SERVFAIL;
						break;
					}
					/* FALLTHROUGH */
				default:
					/* anything else implies that we're done */
					done = 1;
			}
			/*
			 * if we got here for some reason other than DNSRCH,
			 * we only wanted one iteration of the loop, so stop.
			 */
			__RG_MUTEX_LOCK(__resolv_lock);
			_res_options = _rg_res.options;
			__RG_MUTEX_UNLOCK(__resolv_lock);
			if (!(_res_options & RES_DNSRCH))
				done = 1;
		}
	}

	/*
	 * if we have not already tried the name "as is", do that now.
	 * note that we do this regardless of how many dots were in the
	 * name or whether it ends with a dot.
	 */
	if (!(_state & __TRIED_AS_IS)) {
		ret = rg_res_querydomain(name, NULL, class, type, answer,
		    anslen, so_mark);
		if (ret > 0)
			return (ret);
	}

	/*
	 * if we got here, we didn't satisfy the search.
	 * if we did an initial full query, return that query's h_errno
	 * (note that we wouldn't be here if that query had succeeded).
	 * else if we ever got a nodata, send that back as the reason.
	 * else send back meaningless h_errno, that being the one from
	 * the last DNSRCH we did.
	 */
	if (saved_herrno != -1)
		h_errno = saved_herrno;
	else if (_state & __GOT_NODATA)
		h_errno = NO_DATA;
	else if (_state & __GOT_SERVFAIL)
		h_errno = TRY_AGAIN;
	return (-1);
}
#undef __TRAILING_DOT
#undef __GOT_NODATA
#undef __GOT_SERVFAIL
#undef __TRIED_AS_IS
/*
 * Perform a call on res_query on the concatenation of name and domain,
 * removing a trailing dot from name if domain is NULL.
 */
int rg_res_querydomain(name, domain, class, type, answer, anslen, so_mark)
	 const char *name, *domain;
	 int class, type;	/* class and type of query */
	 u_char *answer;		/* buffer to put answer */
	 int anslen;		/* size of answer */
	 int so_mark;		/* skb->mark of created query */
{
	char nbuf[MAXDNAME];
	const char *longname = nbuf;
	size_t n, d;
	u_long _res_options;

	__RG_MUTEX_LOCK(__resolv_lock);
	_res_options = _rg_res.options;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	if ((!name || !answer) || ((_res_options & RES_INIT) == 0 && rg_res_init() == -1)) {
		h_errno = NETDB_INTERNAL;
		return (-1);
	}

#ifdef DEBUG
	__RG_MUTEX_LOCK(__resolv_lock);
	_res_options = _rg_res.options;
	__RG_MUTEX_UNLOCK(__resolv_lock);
	if (_res_options & RES_DEBUG)
		printf(";; res_querydomain(%s, %s, %d, %d)\n",
			   name, domain?domain:"<Nil>", class, type);
#endif
	if (domain == NULL) {
		/*
		 * Check for trailing '.';
		 * copy without '.' if present.
		 */
		n = strlen(name);
		if (n + 1 > sizeof(nbuf)) {
			h_errno = NO_RECOVERY;
			return (-1);
		}
		if (n > 0 && name[--n] == '.') {
			strncpy(nbuf, name, n);
			nbuf[n] = '\0';
		} else
			longname = name;
	} else {
		n = strlen(name);
		d = strlen(domain);
		if (n + 1 + d + 1 > sizeof(nbuf)) {
			h_errno = NO_RECOVERY;
			return (-1);
		}
		snprintf(nbuf, sizeof(nbuf), "%s.%s", name, domain);
	}
	return (rg_res_query(longname, class, type, answer, anslen, so_mark));
}

/* res_mkquery */
/* res_send */
/* dn_comp */
/* dn_expand */
#endif

#ifdef L_gethostbyname_r

int rg_gethostbyname_r(const char * name,
					struct hostent * result_buf,
					char * buf, size_t buflen,
					struct hostent ** result,
					int * h_errnop,
					int so_mark)
{
	struct in_addr *in;
	struct in_addr **addr_list;
	char **alias;
	unsigned char *packet;
	struct resolv_answer a;
	int i;
	int __nameserversXX;
	char ** __nameserverXX;

	__open_nameservers();
	*result=NULL;
	if (!name)
		return EINVAL;


	*h_errnop = NETDB_INTERNAL;
	if (buflen < sizeof(*in))
		return ERANGE;
	in=(struct in_addr*)buf;
	buf+=sizeof(*in);
	buflen-=sizeof(*in);

	if (buflen < sizeof(*addr_list)*2)
		return ERANGE;
	addr_list=(struct in_addr**)buf;
	buf+=sizeof(*addr_list)*2;
	buflen-=sizeof(*addr_list)*2;

	addr_list[0] = in;
	addr_list[1] = 0;

	if (buflen < sizeof(char *)*(ALIAS_DIM))
		return ERANGE;
	alias=(char **)buf;
	buf+=sizeof(char **)*(ALIAS_DIM);
	buflen-=sizeof(char **)*(ALIAS_DIM);

	if (buflen<256)
		return ERANGE;
	strncpy(buf, name, buflen);

	alias[0] = buf;
	alias[1] = NULL;

	/* First check if this is already an address */
	if (inet_aton(name, in)) {
	    result_buf->h_name = buf;
	    result_buf->h_addrtype = AF_INET;
	    result_buf->h_length = sizeof(*in);
	    result_buf->h_addr_list = (char **) addr_list;
	    result_buf->h_aliases = alias;
	    *result=result_buf;
	    *h_errnop = NETDB_SUCCESS;
	    return NETDB_SUCCESS;
	}

	for (;;) {

	    __RG_MUTEX_LOCK(__resolv_lock);
	    __nameserversXX=__nameservers;
	    __nameserverXX=__nameserver;
	    __RG_MUTEX_UNLOCK(__resolv_lock);
	    a.buf = buf;
	    a.buflen = buflen;
	    a.add_count = 0;
	    i = __dns_lookup(name, T_A, __nameserversXX, __nameserverXX, &packet, &a, so_mark);

	    if (i < 0) {
			*h_errnop = HOST_NOT_FOUND;
			DPRINTF("__dns_lookup\n");
			return TRY_AGAIN;
	    }

	    if ((a.rdlength + sizeof(struct in_addr*)) * a.add_count + 256 > buflen)
			{
				free(a.dotted);
				free(packet);
				*h_errnop = NETDB_INTERNAL;
				DPRINTF("buffer too small for all addresses\n");
				return ERANGE;
			}
	    else if(a.add_count > 0)
			{
				memmove(buf - sizeof(struct in_addr*)*2, buf, a.add_count * a.rdlength);
				addr_list = (struct in_addr**)(buf + a.add_count * a.rdlength);
				addr_list[0] = in;
				for (i = a.add_count-1; i>=0; --i)
					addr_list[i+1] = (struct in_addr*)(buf - sizeof(struct in_addr*)*2 + a.rdlength * i);
				addr_list[a.add_count + 1] = 0;
				buflen -= (((char*)&(addr_list[a.add_count + 2])) - buf);
				buf = (char*)&addr_list[a.add_count + 2];
			}

	    strncpy(buf, a.dotted, buflen);
	    free(a.dotted);

	    if (a.atype == T_A) { /* ADDRESS */
			memcpy(in, a.rdata, sizeof(*in));
			result_buf->h_name = buf;
			result_buf->h_addrtype = AF_INET;
			result_buf->h_length = sizeof(*in);
			result_buf->h_addr_list = (char **) addr_list;
#ifdef __UCLIBC_MJN3_ONLY__
#warning TODO -- generate the full list
#endif
			result_buf->h_aliases = alias; /* TODO: generate the full list */
			free(packet);
			break;
	    } else {
			free(packet);
			*h_errnop=HOST_NOT_FOUND;
			return TRY_AGAIN;
	    }
	}

	*result=result_buf;
	*h_errnop = NETDB_SUCCESS;
	return NETDB_SUCCESS;
}
#endif
#else

#include <stdio.h>
#include <netdb.h>
#include <resolv.h>
/* Simple versions of resolving functions that ignore the so_mark when the full
 * implementation (see above) cannot compile because of LIBC limitation (Used
 * for platforms compiled with LIBC_IN_TOOLCHAIN=y and uLibc version is not
 * compatible with our libc). */

struct hostent *rg_gethostbyname(const char *name, int so_mark)
{
    return gethostbyname(name);
}

int rg_gethostbyname_r(const char * name,
    struct hostent * result_buf,
    char * buf, size_t buflen,
    struct hostent ** result,
    int * h_errnop,
    int so_mark)
{
    return gethostbyname_r(name, result_buf, buf, buflen, result, h_errnop);
}

int rg_res_query(const char *dname, int class, int type,
    unsigned char *answer, int anslen, int so_mark)
{
    return res_query(dname, class, type, answer, anslen);
}

int rg_res_querydomain(const char *name, const char *domain, int class, 
    int type, unsigned char *answer, int anslen, int so_mark)
{
    return res_querydomain(name, domain, class, type, answer, anslen);
}

int rg_res_init(void)
{
    return res_init();
}

void rg_res_close(void)
{
    res_close();
}

int rg_res_search(const char *name, int class, int type, 
    unsigned char *answer, int anslen, int so_mark)
{
    return res_search(name, class, type, answer, anslen);
}

#endif

