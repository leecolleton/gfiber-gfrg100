/****************************************************************************
 *
 * rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_joint_octeon.c
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

#include "ipsec_compat.h"

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/string.h>

#include <util/conv.h>

#include "ipsec_alg.h"
#include <ipsec_xform.h>
#include <ipsec_log.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "cavium_api.h"

static int debug = 0;
ipsec_module_param_int(debug);
static int excl = 0;
ipsec_module_param_int(excl);

#define MAX_CIPHER_KEY_LENGTH 32
#define MAX_AUTH_KEY_LENGTH 64
#define MAX_ACTIVE_TUNNELS 1000

typedef struct {
    uint8_t auth_alg_id;
    uint8_t enc_alg_id;
    uint8_t *auth_key;
    int auth_key_len;
    uint8_t *enc_key;
    int enc_key_len;
    unsigned int not_first;
    unsigned char hmac_inner[24];
    unsigned char hmac_outer[24];
    int active;
} octeon_joint_ctx_t;

static octeon_joint_ctx_t contexts[MAX_ACTIVE_TUNNELS];

static int first_free_context(void)
{
    int i;
    
    for (i = 0;i < MAX_ACTIVE_TUNNELS;i++)
    {
	if (!contexts[i].active)
	    return i;
    }

    return -1;
}

static int joint_octeon_set_key(__u8 ipsec_authalg, __u8 ipsec_encalg,
    __u8 *j_ctx, const __u8 *key_a, int keylen_a, const __u8 *key_e,
    int keylen_e, int dir_in)
{
    octeon_joint_ctx_t *ctx;
    int i;
    uint64_t *j_ctx_64 = (uint64_t *)j_ctx;

    if ((i = first_free_context()) == -1)
    {
	printk("No Free Tunnels\n");
	return -1;
    }

    *j_ctx_64 = (uint64_t)i;
    ctx = &contexts[i];
   
    ctx->active = 1;    

    ctx->auth_alg_id = ipsec_authalg;
    ctx->enc_alg_id = ipsec_encalg;
    
    if ((ctx->auth_key_len = keylen_a) &&
	(ctx->auth_key = kmalloc(keylen_a, GFP_KERNEL)))
    {
	memcpy(ctx->auth_key, key_a, keylen_a);
    }
    if ((ctx->enc_key_len = keylen_e) &&
	(ctx->enc_key = kmalloc(keylen_e, GFP_KERNEL)))
    {
	memcpy(ctx->enc_key, key_e, keylen_e);
    }
    ctx->not_first = 0;

    return 0;
}

static int joint_octeon_auth_and_encrypt(struct sk_buff *skb,
    struct ipsec_alg_enc *ixt_e, __u8 *j_ctx, const __u8 *data, int len,
    int enc_offset, int auth_len, const __u8 *iv, int encrypt,
    ipsec_alg_complete_cb_t complete_cb, void *cb_ctx)
{
    int ret=0;
    octeon_joint_ctx_t *ctx;
    
    ctx = &contexts[*(uint64_t *)j_ctx];
    
    if (!ctx->not_first) {
	cav_calc_hash(((ctx->auth_alg_id == AH_SHA) ? 1 : 0),
	    (__u64 *)ctx->auth_key,
	    (__u8 *)ctx->hmac_inner, 
	    (__u8 *)ctx->hmac_outer);
	ctx->not_first = 1;
    }

    if (encrypt)
	len -= auth_len;
    switch(ctx->enc_alg_id)
    {
    case ESP_DES : 
    case ESP_3DES:
	switch(ctx->auth_alg_id)
	{
	case AH_MD5:
	    ret = (encrypt ? DES_CBC_md5_encrypt : DES_CBC_md5_decrypt)
		(data, data, len, (__u8 *)ctx->enc_key, ctx->enc_key_len*8, iv,
		(__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	case AH_SHA:
	    ret = (encrypt ? DES_CBC_sha1_encrypt : DES_CBC_sha1_decrypt)
		(data, data, len, (__u8 *)ctx->enc_key, ctx->enc_key_len*8, iv,
		(__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	default:	
	    printk("\n Unsupported HMAC algorithm \n"); 
	    ret = -1;
	}
	break;
	
    case ESP_AES:
	switch(ctx->auth_alg_id)
	{
	case AH_MD5:
	    ret = (encrypt ? AES_CBC_md5_encrypt : AES_CBC_md5_decrypt)
		(data, data, len, (__u8 *)ctx->enc_key, ctx->enc_key_len*8, iv,
		(__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	case AH_SHA:
	    ret = (encrypt ? AES_CBC_sha1_encrypt : AES_CBC_sha1_decrypt)
		(data, data, len, (__u8 *)ctx->enc_key, ctx->enc_key_len*8, iv,
		(__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	default:	
	    printk("\n Unsupported HMAC algorithm \n"); 
	    ret = -1;
	}
	break;
	
    case ESP_NULL:
	switch(ctx->auth_alg_id)
	{
	case AH_MD5:
	    ret = (encrypt ? NULL_md5_encrypt : NULL_md5_decrypt)(data, data,
		len, (__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	case AH_SHA:
	    ret = (encrypt ? NULL_sha1_encrypt : NULL_sha1_decrypt)(data, data,
		len, (__u64 *)ctx->hmac_inner, (__u64 *)ctx->hmac_outer);
	    break;
	default:	
	    printk("\n Unsupported HMAC algorithm \n"); 
	    ret = -1;
	}
	break;
    default:
	ret = -1;
    }
    return ret;
}

static void joint_octeon_destroy_ctx(__u8 *j_ctx)
{
    octeon_joint_ctx_t *ctx = &contexts[*(uint64_t *)j_ctx];

    ctx->active = 0;
    kfree(ctx->auth_key);
    kfree(ctx->enc_key);
    kfree(j_ctx);
}

static struct ipsec_alg_joint ipsec_alg_joint_octeon = {
    ixt_version:		IPSEC_ALG_VERSION,
    ixt_module:			THIS_MODULE,
    ixt_refcnt:			ATOMIC_INIT(0),
    ixt_name:			"joint_octeon",
    ixt_alg_type:		IPSEC_ALG_TYPE_JOINT,
    ixt_alg_id: 		0,
    ixt_j_ctx_size:		sizeof(u32), /* the external context is the id
					      * and not ixp_joint_ctx_t */
    ixt_j_set_key:		joint_octeon_set_key,
    ixt_j_auth_and_encrypt:	joint_octeon_auth_and_encrypt,
    ixt_j_destroy_ctx:		joint_octeon_destroy_ctx,
    ixt_j_is_support_ah_proto:	0,
    ixt_j_is_full_process_orig_pkt:	0,
};
	
IPSEC_ALG_MODULE_INIT(ipsec_joint_init)
{
    int ret;

    if (excl)
	ipsec_alg_joint_octeon.ixt_state |= IPSEC_ALG_ST_EXCL;
    ret = register_ipsec_alg((struct ipsec_alg *)&ipsec_alg_joint_octeon);
    if (debug)
    {
	ipsec_log("ipsec_joint_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
	    ipsec_alg_joint_octeon.ixt_alg_type,
	    ipsec_alg_joint_octeon.ixt_alg_id, ipsec_alg_joint_octeon.ixt_name,
	    ret);
    }
    if (ret)
	return ret;

    return 0;
}

IPSEC_ALG_MODULE_EXIT(ipsec_joint_fini)
{
    unregister_ipsec_alg((struct ipsec_alg*)&ipsec_alg_joint_octeon);
    return;
}
