/* LICENCE_CODE JGPL */

#include <net/ip.h>
#include <ipsec_xform.h>
#define INLINE inline
#include <feroceon_cesa.h>
#include <util/conv.h>

/* Check if __exit is defined, if not null it */
#ifndef __exit
#define __exit
#endif

#include "ipsec_sa.h"
#include "ipsec_alg.h"
#include "ipsec_compat.h"
#include <crypto/ocf/cryptodev.h>

static int debug = 0;
ipsec_module_param_int(debug);
static int excl = 0;
ipsec_module_param_int(excl);

#define ERROR_PRINT(FMT, ARGS...) \
    printk("%s: ERROR: " FMT "\n", __FUNCTION__ , ## ARGS)

#ifdef DEBUG
#define DEBUG_PRINT(FMT, ARGS...) \
    printk("%s:%d: " FMT "\n", __FUNCTION__, __LINE__ , ## ARGS)
#define DEBUG_MEM_DUMP(prefix, data, len) do { \
    int i; \
    printk("%s: %s", __FUNCTION__, prefix); \
    for (i = 0; i < len; i++) \
        printk("%02X", data[i]); \
    printk("\n"); \
} while(0)
#else
#define DEBUG_PRINT(FMT, ARGS...)
#define DEBUG_MEM_DUMP(prefix, data, len)
#endif

static int joint_feroceon_complete_cb(struct cryptop * crp)
{
	ipsec_alg_complete_cb_t complete_cb = crp->complete_cb;
	void *cb_ctx = crp->cb_ctx;

	crypto_freereq(crp);
	complete_cb(cb_ctx, 0);
	return 0;
}

static int joint_feroceon_set_key(__u8 ipsec_authalg, __u8 ipsec_encalg,
    __u8 *j_ctx, const __u8 *key_a, int keylen_a, const __u8 *key_e,
    int keylen_e, int dir_in)
{
	int rc = 0;
	u32 *context = (u32*)j_ctx;
	u32 sid = 0;
	int enc_id = 0;
	int auth_id = 0;
	struct cryptoini e_cri;
	struct cryptoini a_cri;
	device_t sc = NULL; /* Currently not in use. */
	u8 proto = keylen_e == -1 ? IPPROTO_AH : IPPROTO_ESP;

	code2code_t enc_alg[] = {
	    {ESP_DES, CRYPTO_DES_CBC},
	    {ESP_3DES, CRYPTO_3DES_CBC},
	    {ESP_AES, CRYPTO_AES_CBC},
	    {ESP_NULL, CRYPTO_NULL_CBC},
	    {-1, 0}
	};

	code2code_t auth_alg[] = {
	    {AH_MD5, CRYPTO_MD5_HMAC},
	    {AH_SHA, CRYPTO_SHA1_HMAC},
	    {-1, CRYPTO_NULL_HMAC}
	};

/*
*
*	register algo session
*/
	if (proto == IPPROTO_ESP) 
	{
	    /* encryption first */
	    enc_id = ipsec_encalg;
	    e_cri.cri_alg = code2code(enc_alg, enc_id);
	    e_cri.cri_klen = keylen_e * 8;
	    e_cri.cri_key = (__u8 *)key_e; /* Freeswan does not change it.  */

	    /* the authentication */	
	    auth_id = ipsec_authalg;
	    a_cri.cri_alg = code2code(auth_alg, auth_id);
	    a_cri.cri_klen = keylen_a * 8;
	    a_cri.cri_key = (__u8 *)key_a; /* Freeswan does not change it.  */

	    /* Chain both request */
	    e_cri.cri_next = &a_cri;
	    a_cri.cri_next = 0;
	    rc = cesa_ocf_newsession(sc, &sid, &e_cri);
	}
	else if(proto == IPPROTO_AH)
	{
	    /* the authentication */	
	    auth_id = ipsec_authalg;
	    a_cri.cri_alg = code2code(auth_alg, auth_id);
	    a_cri.cri_klen = keylen_a * 8;
	    a_cri.cri_key = (__u8 *)key_a; /* Freeswan does not change it.  */
	    a_cri.cri_next = 0;
	    rc = cesa_ocf_newsession(sc, &sid, &a_cri);
	}
	if(rc)
	{
	    printk("cesa_ocf_newsession error %x\n", rc);
	    *context = 0;
	    return -1;
	}

	*context = sid;
	return 0;
}

/****************************************************************************
 *  ip_hdr_len   enc_offset                                       auth_len
 *     20B+         16B         20B+                 2 + pad        12B
 * ,-----------,------------,-----------,---------,-------------,----------,
 * | IP header | ESP header | IP header | Payload | ESP trailer | ESP Auth |
 * '-----------'------------'-----------'---------'-------------'----------'
 *                   ^--IV--^------------Encrypted--------------^
 *             ^-----------------------Authenticated------------^
 * ^           ^     ^      ^                                   ^
 * skb->data   data  iv_off enc_offset                          digestOffset
 * ^--------------------------------skb->len-------------------------------^
 ****************************************************************************/
static int joint_feroceon_auth_and_encrypt(struct sk_buff *skb,
    struct ipsec_alg_enc *ixt_e, __u8 *j_ctx, const __u8 *data, int len,
    int enc_offset, int auth_len, const __u8 *iv, int encrypt,
    ipsec_alg_complete_cb_t complete_cb, void *cb_ctx)
{
#define ESP_IV_OFFSET 8
    int ret = 1;
    u32* context = (u32*)j_ctx;
    device_t sc = NULL; /* Currently not in use. */
    int crypto_len = len - enc_offset - auth_len;
    int ip_hdr_len = data - skb->data;

    DEBUG_PRINT("session = %u, skb = %p, data = %p, len = %d, iv = %p, "
	"enc_offset = %d, auth_len = %d, encrypt = %d, complete_cb = %p, "
	"cb_ctx = %p (skb->len = %d, skb->data= %p)", *context, skb, data, len,
	iv, enc_offset, auth_len, encrypt, complete_cb, cb_ctx, skb->len,
	skb->data);

    if (context)
    {
	struct cryptop *crp = crypto_getreq(2);
	struct cryptodesc *crde, *crda;

	if (crp)
	{
	    /* Cesa OCF does not care for the order of the descriptors. */
	    crde = crp->crp_desc;
	    crda = crde->crd_next;

	    crda->crd_skip = ip_hdr_len;
	    crda->crd_flags = 0;
	    crda->crd_len = enc_offset + crypto_len;
	    crda->crd_inject = ip_hdr_len + enc_offset + crypto_len;
	    crda->crd_alg = CRYPTO_GROUP_AUTH;

	    crde->crd_skip = ip_hdr_len + enc_offset;
	    crde->crd_flags = (encrypt ? CRD_F_IV_EXPLICIT |
		CRD_F_IV_PRESENT_NO_PUT_ASIDE | CRD_F_ENCRYPT : 0);
	    crde->crd_len = crypto_len;
	    crde->crd_inject = ip_hdr_len + ESP_IV_OFFSET;
	    crde->crd_alg = CRYPTO_GROUP_ENC;

	    crp->crp_ilen = skb->len;
	    crp->crp_sid = *context;
	    crp->crp_flags |= CRYPTO_F_SKBUF;
	    crp->crp_buf = (caddr_t)skb;
	    crp->crp_callback = joint_feroceon_complete_cb;
	    crp->complete_cb = (void*)complete_cb;
	    crp->cb_ctx = cb_ctx;
	    ret = cesa_ocf_process(sc, crp, 0);
	    if (ret)
		crypto_freereq(crp);
	}
    }
    return ret;
}

static void joint_feroceon_destroy_ctx(__u8 *j_ctx)
{
    device_t sc = NULL; /* Currently not in use. */
    u_int64_t tid = *(u32 *)j_ctx;

    DEBUG_PRINT("Closing session %u", (u32)tid);

    cesa_ocf_freesession(sc, tid);
    kfree(j_ctx);
}

static struct ipsec_alg_joint ipsec_alg_joint_feroceon = {
    ixt_version:		IPSEC_ALG_VERSION,
    ixt_module:			THIS_MODULE,
    ixt_refcnt:			ATOMIC_INIT(0),
    ixt_name:			"joint_feroceon",
    ixt_alg_type:		IPSEC_ALG_TYPE_JOINT,
    ixt_alg_id: 		0,
    ixt_j_ctx_size:		sizeof(u32), /* Session ID, although u_int64_t in 
					      * cesa_ocf_freesession(). */
    ixt_j_set_key:		joint_feroceon_set_key,
    ixt_j_auth_and_encrypt:	joint_feroceon_auth_and_encrypt,
    ixt_j_destroy_ctx:		joint_feroceon_destroy_ctx,
    ixt_j_is_support_ah_proto:	1,
    ixt_j_is_full_process_orig_pkt:	0,
};

IPSEC_ALG_MODULE_INIT(ipsec_joint_init)
{
    int ret;
    extern void ipsec_log(char *format, ...);

    if (excl)
	ipsec_alg_joint_feroceon.ixt_state |= IPSEC_ALG_ST_EXCL;

    ret = register_ipsec_alg((struct ipsec_alg *)&ipsec_alg_joint_feroceon);

    if (debug)
    {
	ipsec_log("ipsec_joint_init(alg_type=%d alg_id=%d name=%s): ret=%d\n",
	    ipsec_alg_joint_feroceon.ixt_alg_type,
	    ipsec_alg_joint_feroceon.ixt_alg_id,
	    ipsec_alg_joint_feroceon.ixt_name, ret);
    }

    if (ret)
	return ret;

    return 0;
}

IPSEC_ALG_MODULE_EXIT(ipsec_joint_fini)
{
    unregister_ipsec_alg((struct ipsec_alg*)&ipsec_alg_joint_feroceon);
    return;
}
