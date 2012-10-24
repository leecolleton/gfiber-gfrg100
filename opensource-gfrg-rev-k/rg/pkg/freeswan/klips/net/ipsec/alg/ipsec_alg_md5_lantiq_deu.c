/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_md5_lantiq_deu.c * 
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
#if CONFIG_IPSEC_MODULE && CONFIG_IPSEC_ALG_MD5
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

#define AH_MD5					2
#define MD5_HMAC_BLOCK_SIZE			64	/* 512 bit data block */
#define MD5_HMAC_DIGEST_SIZE			16	/* 128 bit digest */

/* from the Lantiq DEU driver */
#define MD5_BLOCK_WORDS     16
#define MD5_HASH_WORDS      4
#define MD5_HMAC_DBN_TEMP_SIZE  1024 // size in dword, needed for dbn workaround 
#define MAX_HASH_KEYLEN 64

struct md5_hmac_hw_ctx {
    u8 key[MAX_HASH_KEYLEN];    
    u32 hash[MD5_HASH_WORDS];
    u32 block[MD5_BLOCK_WORDS];
    u64 byte_count;
    u32 dbn;
    unsigned int keylen;
};

struct md5_hw_ctx {
    int started;
    u32 hash[MD5_HASH_WORDS];
    u32 block[MD5_BLOCK_WORDS];
    u64 byte_count;
};

/* Our context */
struct md5_ctx {
    u8 key[MD5_HMAC_BLOCK_SIZE]; 
    int key_len;
};

static DEFINE_SPINLOCK(md5_lock);

extern int deu_md5_hmac_setkey(void *ctx, const u8 *key, unsigned int keylen);
extern int deu_md5_hmac_init(void *ctx);
extern int deu_md5_hmac_update(void *ctx, const u8 *data, unsigned int len);
extern int deu_md5_hmac_final(void *ctx, u8 *out);
extern int deu_md5_init(void *ctx);
extern int deu_md5_update(void *ctx, const u8 *data, unsigned int len);
extern int deu_md5_final(void *ctx, u8 *out);

#ifdef DEU_AUTH_DEBUG
#define DEBUG_PRINT(fmt, p...) \
    ipsec_log("auth debug(md5): %s:" fmt "\n", __FUNCTION__, ##p)
#else
#define DEBUG_PRINT(fmt, p...) do {} while (0)
#endif

static int _md5_hmac_deu_set_key(struct ipsec_alg_auth *alg, __u8 *key_a,
    const __u8 *key, int key_len) 
{
    struct md5_ctx *ctx = (struct md5_ctx *)key_a;

    DEBUG_PRINT("alg=%p, key_a=%p, key=%p, key_len=%d", alg, key_a, key,
	key_len);

    /* If the key is too big we use the digest of the key */
    if (key_len > MD5_HMAC_BLOCK_SIZE)
    {
	struct md5_hw_ctx tmp;

	spin_lock_bh(&md5_lock);
	deu_md5_init(&tmp);
	deu_md5_update(&tmp, key, key_len);
	deu_md5_final(&tmp, ctx->key);
	spin_unlock_bh(&md5_lock);
	ctx->key_len = MD5_HMAC_DIGEST_SIZE;
    }
    else
    {
	memcpy(ctx->key, key, key_len);
	ctx->key_len = key_len;
    }
    return 0;
}

static int _md5_hmac_deu_hash(struct ipsec_alg_auth *alg, __u8 *key_a,
    const __u8 *data, int data_len, __u8 *hash, int hash_len) 
{
    struct md5_ctx *ctx = (struct md5_ctx *)key_a;
    static struct md5_hmac_hw_ctx hw_ctx;
    u8 digest[MD5_HMAC_DIGEST_SIZE];
    int len = hash_len;
    
    DEBUG_PRINT("alg=%p, key_a=%p, data=%p, data_len=%d, hash=%p, hash_len=%d",
	alg, key_a, data, data_len, hash, hash_len);
    
    spin_lock_bh(&md5_lock);
    deu_md5_hmac_setkey(&hw_ctx, ctx->key, ctx->key_len);
    deu_md5_hmac_init(&hw_ctx);
    deu_md5_hmac_update(&hw_ctx, data, data_len);
    deu_md5_hmac_final(&hw_ctx, digest);
    spin_unlock_bh(&md5_lock);

    /* Clip if necessary */
    if (len > MD5_HMAC_DIGEST_SIZE)
	len = MD5_HMAC_DIGEST_SIZE;

    memcpy(hash, digest, len);
    return 0;
}

static struct ipsec_alg_auth ipsec_alg_MD5_deu = {
    ixt_version:	IPSEC_ALG_VERSION,
    ixt_module:		THIS_MODULE,
    ixt_refcnt:		ATOMIC_INIT(0),
    ixt_name:		"md5_deu",
    ixt_alg_type:	IPSEC_ALG_TYPE_AUTH,
    ixt_alg_id: 	AH_MD5,
    ixt_blocksize:	MD5_HMAC_BLOCK_SIZE,
    ixt_keyminbits:	128,
    ixt_keymaxbits:	128,
    ixt_a_keylen:	128/8,
    ixt_a_ctx_size:	sizeof(struct md5_ctx),
    ixt_a_hmac_set_key:	_md5_hmac_deu_set_key,
    ixt_a_hmac_hash:	_md5_hmac_deu_hash,
};

IPSEC_ALG_MODULE_INIT(ipsec_md5_init)
{
    ipsec_log("DEU-ipsec driver : register MD5 hash algorithm\n");
    return register_ipsec_alg_auth(&ipsec_alg_MD5_deu);
}

IPSEC_ALG_MODULE_EXIT(ipsec_md5_fini)
{
    unregister_ipsec_alg_auth(&ipsec_alg_MD5_deu);
}

