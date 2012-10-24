/****************************************************************************
 *
 * rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_joint_ixp.c
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

#include <IxTypes.h>
#include <IxCryptoAcc.h> 

#include <ipsec_log.h>
#include <ipsec_xform.h>
#include <ipsec_rcv.h>
#include <ipsec_sa.h>
#include <ipsec_glue_mbuf.h>
#include <ipsec_ah.h>
#include <ipsec_hwaccel.h>

#include <util/conv.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "ipsec_alg.h"

static int debug = 0;
ipsec_module_param_int(debug);
static int excl = 0;
ipsec_module_param_int(excl);

typedef struct {
    ipsec_alg_complete_cb_t complete_cb;
    void *cb_ctx;
    IxCryptoAccStatus status;
    int data_offset;
} ixp_joint_ctx_t;

static ixp_joint_ctx_t contexts[IX_CRYPTO_ACC_MAX_ACTIVE_SA_TUNNELS];

#define IPSEC_DES_BLOCK_LENGTH 8

static void register_crypto_cb(u32 ctx_id, IX_MBUF *empty_mbuf,
    IxCryptoAccStatus status)
{
    if (empty_mbuf)
	ipsec_glue_mbuf_rel(empty_mbuf);

    contexts[ctx_id].status = status;
}

static void perform_cb(u32 ctx_id, IX_MBUF *src_mbuf, IX_MBUF *dst_mbuf,
    IxCryptoAccStatus status)
{
    ixp_joint_ctx_t *ctx;
    struct sk_buff *skb;

    ctx = &contexts[ctx_id];
#ifdef CONFIG_RG_OS_LINUX_26
    skb = IX_OSAL_MBUF_PRIV(src_mbuf);
#else /* old csr */
    skb = mbuf_swap_skb(src_mbuf, NULL);
#endif
    skb_push(skb, ctx->data_offset);
    ipsec_glue_mbuf_header_rel(src_mbuf);
    ctx->complete_cb(ctx->cb_ctx, status);
}

static int joint_ixp_set_key(__u8 ipsec_authalg, __u8 ipsec_encalg,
    __u8 *j_ctx, const __u8 *key_a, int keylen_a, const __u8 *key_e,
    int keylen_e, int dir_in)
{
    IxCryptoAccCtx crypto_acc_ctx = {};
    IxCryptoAccStatus reg_status;
    IX_MBUF *mbuf_primary_chain = NULL;
    IX_MBUF *mbuf_secondary_chain = NULL;
    int enc_id, auth_id;
    unsigned long jiffies_start;
    ixp_joint_ctx_t *ctx;
    code2code_t enc_alg[] = {
	{ESP_DES, IX_CRYPTO_ACC_CIPHER_DES},
	{ESP_3DES, IX_CRYPTO_ACC_CIPHER_3DES},
	{ESP_AES, IX_CRYPTO_ACC_CIPHER_AES},
	{-1, IX_CRYPTO_ACC_CIPHER_NULL}
    };
    code2code_t block_len[] = {
	{ESP_DES, IPSEC_DES_BLOCK_LENGTH},
	{ESP_3DES, IPSEC_DES_BLOCK_LENGTH},
	{ESP_AES, IX_CRYPTO_ACC_AES_BLOCK_128},
	{-1, 0}
    };
    code2code_t cipher_mode[] = {
	{ESP_DES, IX_CRYPTO_ACC_MODE_CBC},
	{ESP_3DES, IX_CRYPTO_ACC_MODE_CBC},
	{ESP_AES, IX_CRYPTO_ACC_MODE_CBC},
	{-1, IX_CRYPTO_ACC_MODE_NULL}
    };
    code2code_t iv_len[] = {
	{ESP_DES, IX_CRYPTO_ACC_DES_IV_64},
	{ESP_3DES, IX_CRYPTO_ACC_DES_IV_64},
	{ESP_AES, IX_CRYPTO_ACC_AES_CBC_IV_128},
	{-1, 0}
    };
    code2code_t auth_alg[] = {
	{AH_MD5, IX_CRYPTO_ACC_AUTH_MD5},
	{AH_SHA, IX_CRYPTO_ACC_AUTH_SHA1},
	{-1, IX_CRYPTO_ACC_AUTH_NULL}
    };
    code2code_t digest_len[] = {
	{AH_MD5, AHHMAC_HASHLEN},
	{AH_SHA, AHHMAC_HASHLEN},
	{-1, 0}
    };

    if (!ipsec_hwaccel_ready())
	return -1;

    crypto_acc_ctx.useDifferentSrcAndDestMbufs = FALSE;

    enc_id = ipsec_encalg;
    crypto_acc_ctx.cipherCtx.cipherAlgo = code2code(enc_alg, enc_id);
    if ((crypto_acc_ctx.cipherCtx.cipherKeyLen = keylen_e))
	memcpy(crypto_acc_ctx.cipherCtx.key.cipherKey, key_e, keylen_e);
    crypto_acc_ctx.cipherCtx.cipherBlockLen = code2code(block_len, enc_id);
    crypto_acc_ctx.cipherCtx.cipherMode = code2code(cipher_mode, enc_id);
    crypto_acc_ctx.cipherCtx.cipherInitialVectorLen = code2code(iv_len, enc_id);

    auth_id = ipsec_authalg;
    crypto_acc_ctx.authCtx.authAlgo = code2code(auth_alg, auth_id);
    crypto_acc_ctx.authCtx.authDigestLen = code2code(digest_len, auth_id);
    if ((crypto_acc_ctx.authCtx.authKeyLen = keylen_a))
	memcpy(crypto_acc_ctx.authCtx.key.authKey, key_a, keylen_a);

    if (crypto_acc_ctx.cipherCtx.cipherAlgo == IX_CRYPTO_ACC_CIPHER_NULL)
    {
	if (crypto_acc_ctx.authCtx.authAlgo == IX_CRYPTO_ACC_AUTH_NULL)
	    return -1;
	crypto_acc_ctx.operation = dir_in ? IX_CRYPTO_ACC_OP_AUTH_CHECK :
	    IX_CRYPTO_ACC_OP_AUTH_CALC;
    }
    else
    {
	if (crypto_acc_ctx.authCtx.authAlgo == IX_CRYPTO_ACC_AUTH_NULL)
	{
	    crypto_acc_ctx.operation = dir_in ? IX_CRYPTO_ACC_OP_DECRYPT :
		IX_CRYPTO_ACC_OP_ENCRYPT;
	}
	else
	{
	    crypto_acc_ctx.operation = dir_in ? IX_CRYPTO_ACC_OP_AUTH_DECRYPT :
		IX_CRYPTO_ACC_OP_ENCRYPT_AUTH;
	}
    }

    if (IX_CRYPTO_ACC_OP_ENCRYPT != crypto_acc_ctx.operation &&
	IX_CRYPTO_ACC_OP_DECRYPT != crypto_acc_ctx.operation)
    {
	if (ipsec_glue_mbuf_get(&mbuf_primary_chain))
	    return -1;
	if (ipsec_glue_mbuf_get(&mbuf_secondary_chain))
	    goto Error;
    }

    reg_status = ipsec_hwaccel_register(&crypto_acc_ctx, mbuf_primary_chain,
	mbuf_secondary_chain, register_crypto_cb, perform_cb, (u32 *)j_ctx);
    memset(&crypto_acc_ctx, 0, sizeof(crypto_acc_ctx));
    if (IX_CRYPTO_ACC_STATUS_SUCCESS != reg_status)
	goto Error;
    ctx = &contexts[*(u32 *)j_ctx];

    /* wait till register cb is done */
    jiffies_start = jiffies;
    while (ctx->status == IX_CRYPTO_ACC_STATUS_WAIT)
    {
	if (jiffies - jiffies_start > HZ * 10)
	    goto Error;
	schedule();
    }

    return ctx->status;
Error:
    ipsec_glue_mbuf_rel(mbuf_primary_chain);
    ipsec_glue_mbuf_rel(mbuf_secondary_chain);
    return -1;
}

static int joint_ixp_auth_and_encrypt(struct sk_buff *skb,
    struct ipsec_alg_enc *ixt_e, __u8 *j_ctx, const __u8 *data, int len,
    int enc_offset, int auth_len, const __u8 *iv, int encrypt,
    ipsec_alg_complete_cb_t complete_cb, void *cb_ctx)
{
    int ret = 0;
    IX_MBUF *src_mbuf;
    ixp_joint_ctx_t *ctx = &contexts[*(u32 *)j_ctx];

    ctx->complete_cb = complete_cb;
    ctx->cb_ctx = cb_ctx;

    /* get mbuf */
    if (ipsec_glue_mbuf_header_get(&src_mbuf))
	return -1;

    /* attach mbuf to skb */
    ctx->data_offset = data - skb->data;
    skb_pull(skb, ctx->data_offset);
#ifdef CONFIG_RG_OS_LINUX_26
    IX_OSAL_MBUF_PRIV(src_mbuf) = skb;
    IX_OSAL_MBUF_MDATA(src_mbuf) = skb->data;
    IX_OSAL_MBUF_MLEN(src_mbuf) = IX_OSAL_MBUF_PKT_LEN(src_mbuf) = skb->len;
#else /* old csr */
    mbuf_swap_skb(src_mbuf, skb);
#endif
    IX_MBUF_NEXT_PKT_IN_CHAIN_PTR(src_mbuf) = NULL;

    /* call crypto perform */
    if (IX_CRYPTO_ACC_STATUS_SUCCESS != ipsec_hwaccel_perform(*(u32 *)j_ctx,
	src_mbuf, NULL, 0, len - auth_len, enc_offset,
	len - enc_offset - auth_len, len - auth_len, (u8 *)iv))
    {
	ret = -1;
	ipsec_glue_mbuf_header_rel(src_mbuf);
	skb_push(skb, ctx->data_offset);
    }
    return ret;
}

static void joint_ixp_destroy_ctx(__u8 *j_ctx)
{
    u32 ctx_id = *(u32 *)j_ctx;

    ipsec_hwaccel_unregister(ctx_id);
    contexts[ctx_id].status = IX_CRYPTO_ACC_STATUS_WAIT;
    kfree(j_ctx);
}

static struct ipsec_alg_joint ipsec_alg_joint_ixp = {
    ixt_version:		IPSEC_ALG_VERSION,
    ixt_module:			THIS_MODULE,
    ixt_refcnt:			ATOMIC_INIT(0),
    ixt_name:			"joint_ixp",
    ixt_alg_type:		IPSEC_ALG_TYPE_JOINT,
    ixt_alg_id: 		0,
    ixt_j_ctx_size:		sizeof(u32), /* the external context is the id
					      * and not ixp_joint_ctx_t */
    ixt_j_set_key:		joint_ixp_set_key,
    ixt_j_auth_and_encrypt:	joint_ixp_auth_and_encrypt,
    ixt_j_destroy_ctx:		joint_ixp_destroy_ctx,
    ixt_j_is_support_ah_proto:	1,
    ixt_j_is_full_process_orig_pkt:	0,
};
	
IPSEC_ALG_MODULE_INIT(ipsec_joint_init)
{
    int ret, i;

    if (excl)
	ipsec_alg_joint_ixp.ixt_state |= IPSEC_ALG_ST_EXCL;
    ret = register_ipsec_alg((struct ipsec_alg *)&ipsec_alg_joint_ixp);
    if (debug)
    {
	ipsec_log("ipsec_joint_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
	    ipsec_alg_joint_ixp.ixt_alg_type, ipsec_alg_joint_ixp.ixt_alg_id,
	    ipsec_alg_joint_ixp.ixt_name, ret);
    }
    if (ret)
	return ret;

    for (i = 0; i < IX_CRYPTO_ACC_MAX_ACTIVE_SA_TUNNELS; i++)
	contexts[i].status = IX_CRYPTO_ACC_STATUS_WAIT;
    return 0;
}

IPSEC_ALG_MODULE_EXIT(ipsec_joint_fini)
{
    unregister_ipsec_alg((struct ipsec_alg*)&ipsec_alg_joint_ixp);
    return;
}
