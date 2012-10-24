/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_sha1_lantiq_deu.c * 
 * 
 * This file is Jungo's confidential and proprietary property. 
 * This file may not be copied, 
 * distributed or otherwise used in any way without 
 * the express prior approval of Jungo Ltd. 
 * For information contact info@jungo.com
 * 
 * 
 */

#include <linux/version.h>

/*	
 *	special case: ipsec core modular with this static algo inside:
 *	must avoid MODULE magic for this file
 */
#if CONFIG_IPSEC_MODULE && CONFIG_IPSEC_ALG_SHA1
#undef MODULE
#endif

#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <string.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "ipsec_alg.h"
#include <ipsec_log.h>

#define AH_SHA				3
#define SHA1_HMAC_BLOCK_SIZE		64 /* 512 bit block */
#define SHA1_HMAC_DIGEST_SIZE		20 /* 160 bit digest */

/* from the Lantiq DEU driver */
#define SHA1_HMAC_DBN_TEMP_SIZE 1024
#define SHA1_HMAC_BLOCK_SIZE    64
#define SHA1_HMAC_MAX_KEYLEN 64

struct sha1_hmac_hw_ctx {
    int keylen;

    u8 buffer[SHA1_HMAC_BLOCK_SIZE];
    u8 key[SHA1_HMAC_MAX_KEYLEN];
    u32 state[5];
    u32 dbn;
    u64 count;
};

struct sha1_hw_ctx {
    int started;
    u64 count;
    u32 hash[5];
    u32 state[5];
    u8 buffer[64];
};

/* Our context */
struct sha1_ctx {
    u8 key[SHA1_HMAC_BLOCK_SIZE]; 
    int key_len;
};

static DEFINE_SPINLOCK(sha1_lock);

extern int deu_sha1_init(void *ctx);
extern int deu_sha1_update(void *ctx, const u8 *data, unsigned int len);
extern int deu_sha1_final(void *ctx, u8 *out);
extern int deu_sha1_hmac_setkey(void *ctx, const u8 *key, unsigned int keylen);
extern int deu_sha1_hmac_init(void *ctx);
extern int deu_sha1_hmac_update(void *ctx, const u8 *data, unsigned int len);
extern int deu_sha1_hmac_final(void *ctx, u8 *out);

#ifdef DEU_AUTH_DEBUG
#define DEBUG_PRINT(fmt, p...) \
    ipsec_log("auth debug(sha1): %s:" fmt "\n", __FUNCTION__, ##p)
#else
#define DEBUG_PRINT(fmt, p...) do {} while (0)
#endif

static int _sha1_hmac_deu_set_key(struct ipsec_alg_auth *alg, __u8 *key_a,
    const __u8 *key, int key_len) 
{
    struct sha1_ctx *ctx = (struct sha1_ctx *)key_a;

    DEBUG_PRINT("alg=%p, key_a=%p, key=%p, key_len=%d", alg, key_a, key,
	key_len);

    /* If the key is too big we use the digest of the key */
    if (key_len > SHA1_HMAC_BLOCK_SIZE)
    {
	struct sha1_hw_ctx tmp;

	spin_lock_bh(&sha1_lock);
	deu_sha1_init(&tmp);
	deu_sha1_update(&tmp, key, key_len);
	deu_sha1_final(&tmp, ctx->key);
	spin_unlock_bh(&sha1_lock);
	ctx->key_len = SHA1_HMAC_DIGEST_SIZE;
    }
    else
    {
	memcpy(ctx->key, key, key_len);
	ctx->key_len = key_len;
    }
    return 0;
}

static int _sha1_hmac_deu_hash(struct ipsec_alg_auth *alg, __u8 *key_a,
    const __u8 *data, int data_len, __u8 *hash, int hash_len) 
{
    struct sha1_ctx *ctx = (struct sha1_ctx *)key_a;
    static struct sha1_hmac_hw_ctx hw_ctx;
    u8 digest[SHA1_HMAC_DIGEST_SIZE];
    int len = hash_len;
    
    DEBUG_PRINT("alg=%p, key_a=%p, data=%p, data_len=%d, hash=%p, hash_len=%d",
	alg, key_a, data, data_len, hash, hash_len);
    
    spin_lock_bh(&sha1_lock);
    deu_sha1_hmac_setkey(&hw_ctx, ctx->key, ctx->key_len);
    deu_sha1_hmac_init(&hw_ctx);
    deu_sha1_hmac_update(&hw_ctx, data, data_len);
    deu_sha1_hmac_final(&hw_ctx, digest);
    spin_unlock_bh(&sha1_lock);

    /* Clip if necessary */
    if (len > SHA1_HMAC_DIGEST_SIZE)
	len = SHA1_HMAC_DIGEST_SIZE;

    memcpy(hash, digest, len);
    return 0;
}

static struct ipsec_alg_auth ipsec_alg_SHA1_deu = {
    ixt_version:	IPSEC_ALG_VERSION,
    ixt_module:		THIS_MODULE,
    ixt_refcnt:		ATOMIC_INIT(0),
    ixt_name:		"sha1_deu",
    ixt_alg_type:	IPSEC_ALG_TYPE_AUTH,
    ixt_alg_id: 	AH_SHA,
    ixt_blocksize:	SHA1_HMAC_BLOCK_SIZE,
    ixt_keyminbits:	160,
    ixt_keymaxbits:	160,
    ixt_a_keylen:	160/8,
    ixt_a_ctx_size:	sizeof(struct sha1_ctx),
    ixt_a_hmac_set_key:	_sha1_hmac_deu_set_key,
    ixt_a_hmac_hash:	_sha1_hmac_deu_hash,
};

IPSEC_ALG_MODULE_INIT(ipsec_sha1_init)
{
    ipsec_log("DEU-ipsec driver : register SHA1 hash algorithm\n");
    return register_ipsec_alg_auth(&ipsec_alg_SHA1_deu);
}

IPSEC_ALG_MODULE_EXIT(ipsec_sha1_fini)
{
    unregister_ipsec_alg_auth(&ipsec_alg_SHA1_deu);
}

