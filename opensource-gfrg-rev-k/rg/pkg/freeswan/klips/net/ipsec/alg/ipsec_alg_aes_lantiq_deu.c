/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_aes_lantiq_deu.c * 
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
 * special case: ipsec core modular with this static algo inside:
 * must avoid MODULE magic for this file
 */
#if CONFIG_IPSEC_MODULE && CONFIG_IPSEC_ALG_AES
#undef MODULE
#endif

#include <linux/module.h>
#include <linux/init.h>

#include <kos/kos.h>

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/string.h>
#include <ipsec_log.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "ipsec_alg.h"

static int debug=0;
kos_module_param_int(debug);
static int test=0;
kos_module_param_int(test);
static int excl=0;
kos_module_param_int(excl);
static int keyminbits=0;
kos_module_param_int(keyminbits);
static int keymaxbits=0;
kos_module_param_int(keymaxbits);

#define ESP_AES			12	/* truely _constant_  :)  */

/* 128, 192 or 256 */
#define ESP_AES_KEY_SZ_MIN	16 	/* 128 bit secret key */
#define ESP_AES_KEY_SZ_MAX	32 	/* 256 bit secret key */
#define ESP_AES_CBC_BLKLEN	16	/* AES-CBC block size */

/* from Lantiq DEU driver */
#define AES_MAX_KEY_SIZE    32
#define CTR_RFC3686_NONCE_SIZE    4

struct aes_ctx {
    int key_length;
    u32 buf[AES_MAX_KEY_SIZE];
    u8 nonce[CTR_RFC3686_NONCE_SIZE];
};

#ifdef DANUBE_CRYPTO_DEBUG
#define DEBUG_PRINT(fmt, p...) \
    ipsec_log("crypto debug(aes): %s:" fmt "\n", __FUNCTION__, ##p)
#else
#define DEBUG_PRINT(fmt, p...)
#endif

/* Lantiq DEU driver functions */
extern int ifx_deu_aes_set_key(void *c, const u8 *in_key, unsigned int key_len, 
    u32 *flags);
extern void ifx_deu_aes_cbc(void *ctx, uint8_t *dst, const uint8_t *src,
    uint8_t *iv, size_t nbytes, int encdec, int inplace);

static int _aes_deu_set_key(struct ipsec_alg_enc *alg, __u8 *key_e,
    const __u8 *key, size_t keysize)
{
    u32 dummy_flags = 0;
    struct aes_ctx *ctx = (struct aes_ctx *)key_e;

    DEBUG_PRINT("aes_deu_set_key(alg=%p, key_e=%p, key=%p keysize=%d)\n",
	alg, key_e, key, keysize);

    return ifx_deu_aes_set_key(ctx, key, keysize, &dummy_flags);
}

static int _aes_deu_cbc_encrypt(struct ipsec_alg_enc *alg, __u8 *key_e,
    __u8 *in, int ilen, const __u8 *iv, int encrypt)
{
    struct aes_ctx *ctx = (struct aes_ctx *)key_e;
    __u8 iv_buf[ESP_AES_CBC_BLKLEN];

    DEBUG_PRINT("aes_deu_cbc_encrypt(alg=%p, key_e=%p, in=%p ilen=%d "
	"iv=%p encrypt=%d)\n", alg, key_e, in, ilen, iv, encrypt);

    memcpy(iv_buf, iv, ESP_AES_CBC_BLKLEN);
    ifx_deu_aes_cbc(ctx, in, in, iv_buf, ilen, encrypt, 1);
    return ilen;
}

static struct ipsec_alg_enc ipsec_alg_AES_deu = {
    ixt_version:	IPSEC_ALG_VERSION,
    ixt_module:		THIS_MODULE,
    ixt_refcnt:		ATOMIC_INIT(0),
    ixt_name:		"aes_deu",
    ixt_alg_type:	IPSEC_ALG_TYPE_ENCRYPT,
    ixt_alg_id: 	ESP_AES,
    ixt_blocksize:	ESP_AES_CBC_BLKLEN,
    ixt_keyminbits:	ESP_AES_KEY_SZ_MIN*8,
    ixt_keymaxbits:	ESP_AES_KEY_SZ_MAX*8,
    ixt_e_keylen:	ESP_AES_KEY_SZ_MAX,
    ixt_e_ctx_size:	sizeof(struct aes_ctx),
    ixt_e_set_key:	_aes_deu_set_key,
    ixt_e_cbc_encrypt:	_aes_deu_cbc_encrypt,
};
	
IPSEC_ALG_MODULE_INIT(ipsec_aes_init)
{
    int ret, test_ret;

    if (keyminbits)
	ipsec_alg_AES_deu.ixt_keyminbits = keyminbits;
    if (keymaxbits)
    {
	ipsec_alg_AES_deu.ixt_keymaxbits = keymaxbits;
	if (keymaxbits*8 > ipsec_alg_AES_deu.ixt_keymaxbits)
	    ipsec_alg_AES_deu.ixt_e_keylen = keymaxbits*8;
    }
    if (excl)
	ipsec_alg_AES_deu.ixt_state |= IPSEC_ALG_ST_EXCL;
    ret = register_ipsec_alg_enc(&ipsec_alg_AES_deu);
    if (debug)
    {
	ipsec_log("ipsec_aes_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
	    ipsec_alg_AES_deu.ixt_alg_type,
	    ipsec_alg_AES_deu.ixt_alg_id,
	    ipsec_alg_AES_deu.ixt_name,
	    ret);
    }
    if (ret==0 && test)
    {
	test_ret=ipsec_alg_test(
	    ipsec_alg_AES_deu.ixt_alg_type,
	    ipsec_alg_AES_deu.ixt_alg_id,
	    test);
	ipsec_log("ipsec_aes_init(alg_type=%d alg_id=%d): test_ret=%d\n",
	    ipsec_alg_AES_deu.ixt_alg_type,
	    ipsec_alg_AES_deu.ixt_alg_id,
	    test_ret);
    }
    return ret;
}

IPSEC_ALG_MODULE_EXIT(ipsec_aes_fini)
{
    unregister_ipsec_alg_enc(&ipsec_alg_AES_deu);
}

