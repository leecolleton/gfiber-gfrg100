/****************************************************************************
 * Copyright  (C) 2000 - 2008 Jungo Ltd. All Rights Reserved.
 * 
 *  rg/pkg/freeswan/klips/net/ipsec/alg/ipsec_alg_3des_lantiq_deu.c * 
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
#if CONFIG_IPSEC_MODULE && CONFIG_IPSEC_ALG_3DES
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
static int esp_id=0;
kos_module_param_int(esp_id);

#define ESP_3DES		3

#define ESP_3DES_CBC_BLKLEN	8	/* 64 bit blocks */
#define ESP_3DES_KEY_SZ		8*3 	/* 3DES */
#define DES3_EDE_EXPKEY_WORDS	(3 * 32)

/* From Lantiq DEU driver */
struct des3_ede_ctx {
	int controlr_M;
	int key_length;
	u8 iv[ESP_3DES_CBC_BLKLEN];
	u32 expkey[DES3_EDE_EXPKEY_WORDS];
};

#ifdef DANUBE_CRYPTO_DEBUG
#define DEBUG_PRINT(fmt, p...) \
    ipsec_log("crypto debug(3des): %s:" fmt "\n", __FUNCTION__, ##p)
#else
#define DEBUG_PRINT(fmt, p...)
#endif

/* Lantiq DEU driver functions */
extern int ifx_deu_des3_ede_setkey(void *ctx, const u8 *key, 
    unsigned int keylen);
extern void ifx_deu_des_cbc(void *ctx, uint8_t *dst, const uint8_t *src,
    uint8_t *iv, size_t nbytes, int encdec, int inplace);

static int _3des_deu_set_key(struct ipsec_alg_enc *alg, __u8 *key_e,
    const __u8 *key, size_t keysize)
{
    struct des3_ede_ctx *ctx = (struct des3_ede_ctx *)key_e;

    DEBUG_PRINT("3des_deu_set_key(alg=%p, key_e=%p, key=%p keysize=%d)\n",
	alg, key_e, key, keysize);

    return ifx_deu_des3_ede_setkey(ctx, key, keysize);
}

static int _3des_deu_cbc_encrypt(struct ipsec_alg_enc *alg, __u8 *key_e,
    __u8 *in, int ilen, const __u8 *iv, int encrypt)
{
    struct des3_ede_ctx *ctx = (struct des3_ede_ctx *)key_e;
    __u8 iv_buf[ESP_3DES_CBC_BLKLEN] = {};

    DEBUG_PRINT("3des_deu_cbc_encrypt(alg=%p, key_e=%p, in=%p ilen=%d "
	"iv=%p encrypt=%d)\n", alg, key_e, in, ilen, iv, encrypt);

    /* DEU changes the iv, therefore we need to clone it */
    *((__u32 *)&(iv_buf)) = ((__u32 *)(iv))[0];
    *((__u32 *)&(iv_buf) + 1) = ((__u32 *)(iv))[1];
    ifx_deu_des_cbc(ctx, in, in, iv_buf, ilen, encrypt, 1);
    return ilen;
}

static struct ipsec_alg_enc ipsec_alg_3DES_deu = {
    ixt_version:	IPSEC_ALG_VERSION,
    ixt_module:		THIS_MODULE,
    ixt_refcnt:		ATOMIC_INIT(0),
    ixt_name:		"3des_deu",
    ixt_alg_type:	IPSEC_ALG_TYPE_ENCRYPT,
    ixt_alg_id: 	ESP_3DES,
    ixt_blocksize:	ESP_3DES_CBC_BLKLEN,
    ixt_keyminbits:	ESP_3DES_KEY_SZ*7, /*  7bits key+1bit parity  */
    ixt_keymaxbits:	ESP_3DES_KEY_SZ*7, /*  7bits key+1bit parity  */
    ixt_e_keylen:	ESP_3DES_KEY_SZ,
    ixt_e_ctx_size:	sizeof(struct des3_ede_ctx),
    ixt_e_set_key:	_3des_deu_set_key,
    ixt_e_cbc_encrypt:	_3des_deu_cbc_encrypt,
};
	
IPSEC_ALG_MODULE_INIT(ipsec_3des_init)
{
    int ret, test_ret;

    if (esp_id)
	ipsec_alg_3DES_deu.ixt_alg_id = esp_id;
    if (excl)
	ipsec_alg_3DES_deu.ixt_state |= IPSEC_ALG_ST_EXCL;
    ret = register_ipsec_alg_enc(&ipsec_alg_3DES_deu);
    if (debug)
    {
	ipsec_log("ipsec_3des_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
	    ipsec_alg_3DES_deu.ixt_alg_type,
	    ipsec_alg_3DES_deu.ixt_alg_id,
	    ipsec_alg_3DES_deu.ixt_name,
	    ret);
    }
    if (ret==0 && test)
    {
	test_ret=ipsec_alg_test(
	    ipsec_alg_3DES_deu.ixt_alg_type,
	    ipsec_alg_3DES_deu.ixt_alg_id, 
	    test);
	ipsec_log("ipsec_3des_init(alg_type=%d alg_id=%d): test_ret=%d\n",
	    ipsec_alg_3DES_deu.ixt_alg_type, 
	    ipsec_alg_3DES_deu.ixt_alg_id, 
	    test_ret);
    }
    return ret;
}

IPSEC_ALG_MODULE_EXIT(ipsec_3des_fini)
{
    unregister_ipsec_alg_enc(&ipsec_alg_3DES_deu);
}

