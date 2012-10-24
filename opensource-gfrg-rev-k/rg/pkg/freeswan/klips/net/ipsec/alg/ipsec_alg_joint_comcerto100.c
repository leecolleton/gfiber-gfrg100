/****************************************************************************
 *
 * rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_joint_comcerto100.c
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

#include <linux/version.h>

#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/string.h>


#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/skbuff.h>
#include <asm/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/moduleparam.h>
#include <linux/uio.h>
#include <linux/dmapool.h>
#include <net/ipv6.h>
#include <net/ip.h>
#include <ipsec_xform.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "ipsec_sa.h"
#include "ipsec_alg.h"
#include <vendor/elp_common.h>
#include "ipsec_compat.h"

static int debug = 0;
ipsec_module_param_int(debug);
static int excl = 0;
ipsec_module_param_int(excl);

int code2code(code2code_t *enc_alg, int enc_id)
{
	int i=0;

	do{
		if(enc_alg[i][0] == enc_id)
			return enc_alg[i][1];
		i++;
	} while(enc_alg[i][0]!=-1);
	
	return enc_alg[i][1];
}


static int joint_comcerto100_callback(struct cryptop * crp)
{
	ipsec_alg_complete_cb_t complete_cb = (ipsec_alg_complete_cb_t)crp->complete_cb;
	void *cb_ctx = crp->cb_ctx;

	kfree(crp);
	complete_cb(cb_ctx,0);
	return 0;
}

static int joint_comcerto100_set_key(__u8 ipsec_authalg, __u8 ipsec_encalg,
    __u8 *j_ctx, const __u8 *key_a, int keylen_a, const __u8 *key_e,
    int keylen_e, int dir_in)
{
	int rc=0;
	u32 spi = *(u32*)j_ctx;
	u32 *context =(u32*)j_ctx;
	u32 sid = 0;
	int enc_id=0;
	int auth_id=0;
	struct cryptoini e_cri;
	struct cryptoini a_cri;
	struct cryptoini s_cri;
	struct elp_softc *sc = (struct elp_softc *)elp_hardware();
	u8 proto = keylen_e == -1 ? IPPROTO_AH : IPPROTO_ESP;

	code2code_t enc_alg[] = {
	        {ESP_DES, CRYPTO_DES_CBC},
	        {ESP_3DES, CRYPTO_3DES_CBC},
	        {ESP_AES, CRYPTO_AES_CBC},
	        {ESP_NULL, CRYPTO_NULL_CBC},
	        {-1, 0}
	};

#if 0
	code2code_t block_len[] = {
	        {ESP_DES, DES_BLOCK_LEN},
	        {ESP_3DES, DES3_BLOCK_LEN},
	        {ESP_AES, EALG_MAX_BLOCK_LEN},
	        {ESP_NULL, 1},
	        {-1, 0}
	};
#endif

	code2code_t auth_alg[] = {
	        {AH_MD5, CRYPTO_MD5_HMAC},
		{AH_SHA, CRYPTO_SHA1_HMAC},
		{-1, CRYPTO_NULL_HMAC}
	};


/*
*
*	register algo session
*/
	if (proto == IPPROTO_ESP) {
	/* encryption first */
	
		enc_id = ipsec_encalg;
		e_cri.cri_alg = code2code(enc_alg, enc_id);
		e_cri.cri_klen = keylen_e*8;
		e_cri.cri_key = key_e;

	/* the authentication */	
		auth_id = ipsec_authalg;
		a_cri.cri_alg = code2code(auth_alg, auth_id);
		a_cri.cri_klen = keylen_a*8;
		a_cri.cri_key = key_a;

	/*
	*	Chain both request
	*/
		e_cri.cri_next = &a_cri;
		a_cri.cri_next = 0;
		rc = elp_newsession(sc, &sid, &e_cri);
	} else if(proto == IPPROTO_AH) {
		/* the authentication */	
		auth_id = ipsec_authalg;
		a_cri.cri_alg = code2code(auth_alg, auth_id);
		a_cri.cri_klen = keylen_a*8;
		a_cri.cri_key = key_a;
		a_cri.cri_next = 0;
		rc = elp_newsession(sc, &sid, &a_cri);
	}

	if(rc)
	{
		printk("elp_newsession error %x\n",rc);
		return -1;
	}

/*
*	register a packet session now.
*/
	if (proto == IPPROTO_AH) {
		s_cri.cri_alg = CRYPTO_AH;
	} else if (proto == IPPROTO_ESP) {
		s_cri.cri_alg = CRYPTO_ESP4_RFC4303;
	} else {
		BUG();
	}
	
	
	s_cri.crip_basealg = sid;
	s_cri.crip_spivalue=spi;
	s_cri.cri_next = 0;
	s_cri.crip_byte_hard_lifetime = 0; //sa_p->ips_life.ipl_hard;
	s_cri.crip_byte_soft_lifetime = 0; //sa_p->ips_life.ipl_soft;
	if(dir_in)
		s_cri.cri_flags &= ~CRYPTO_ENCRYPT;
	else
		s_cri.cri_flags |= CRYPTO_ENCRYPT;

	rc = elp_newsession(sc, &sid, &s_cri);
	if(rc)
	{
		printk("elp_newsession error2 %x\n",rc);
		return -1;
	}

	*context=sid;
	return 0;
}

static int joint_comcerto100_auth_and_encrypt(struct sk_buff *skb,
    struct ipsec_alg_enc *ixt_e, __u8 *j_ctx, const __u8 *data, int len,
    int enc_offset, int auth_len, const __u8 *iv, int encrypt,
    ipsec_alg_complete_cb_t complete_cb, void *cb_ctx)
{
	int ret = 1;
	u32* context=(u32*)j_ctx;

	struct elp_softc *sc = (struct elp_softc *)elp_hardware();

	if(context)
	{
		struct cryptop *crp = kmalloc(sizeof(struct cryptop), GFP_ATOMIC);
		if(crp)
		{
			memset(crp, 0, sizeof(struct cryptop));
			crp->crp_sid = *context;
			crp->crp_flags |= CRYPTO_F_SKBUF;
			crp->crp_buf = (caddr_t)skb;
			crp->crp_callback = joint_comcerto100_callback;
			crp->complete_cb = (void*)complete_cb;
			crp->cb_ctx = cb_ctx;
			ret = elp_process(sc, crp, 0);
			if(ret)
				kfree(crp);
		}
		
	}
	return ret;
}

static void joint_comcerto100_destroy_ctx(__u8 *j_ctx)
{
	struct elp_softc *sc = (struct elp_softc *)elp_hardware();
	u32* context=(u32*)j_ctx;

	elp_freesession(sc,*context);
	kfree(j_ctx);
}

static struct ipsec_alg_joint ipsec_alg_joint_comcerto100 = {
    ixt_version:		IPSEC_ALG_VERSION,
    ixt_module:			THIS_MODULE,
    ixt_refcnt:			ATOMIC_INIT(0),
    ixt_name:			"joint_100",
    ixt_alg_type:		IPSEC_ALG_TYPE_JOINT,
    ixt_alg_id: 		0,
    ixt_j_ctx_size:		sizeof(u32), /* the external context is the id
					      * and not ixp_joint_ctx_t */
    ixt_j_set_key:		joint_comcerto100_set_key,
    ixt_j_auth_and_encrypt:	joint_comcerto100_auth_and_encrypt,
    ixt_j_destroy_ctx:		joint_comcerto100_destroy_ctx,
    ixt_j_is_support_ah_proto:	1,
    ixt_j_is_full_process_orig_pkt:	1,
};

IPSEC_ALG_MODULE_INIT(ipsec_joint_init)
{
	int ret;
	extern void ipsec_log(char *format, ...);

	if (excl)
		ipsec_alg_joint_comcerto100.ixt_state |= IPSEC_ALG_ST_EXCL;

	ret = register_ipsec_alg((struct ipsec_alg *)&ipsec_alg_joint_comcerto100);


	if (debug)
	{
		ipsec_log("ipsec_joint_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
		ipsec_alg_joint_comcerto100.ixt_alg_type, ipsec_alg_joint_comcerto100.ixt_alg_id,
		ipsec_alg_joint_comcerto100.ixt_name, ret);
	}

	if (ret)
		return ret;

    return 0;
}

IPSEC_ALG_MODULE_EXIT(ipsec_joint_fini)
{
    unregister_ipsec_alg((struct ipsec_alg*)&ipsec_alg_joint_comcerto100);
    return;
}
