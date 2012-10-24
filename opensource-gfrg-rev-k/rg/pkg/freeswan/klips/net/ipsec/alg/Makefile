JMK_ROOT=../../../../../..
include $(JMK_ROOT)/envir.mak

COMMON_CFLAGS+=-I.. -I../libfreeswan -I../libcrypto/include -I../libcrypto \
	-Wall -Wpointer-arith -Wstrict-prototypes

ifndef CONFIG_RG_GPL
ifeq ($(CONFIG_ADI_6843),y)
  ifneq ($(CONFIG_RG_OS_LINUX_26),y)
    VENDOR_SUFFIX_ENC=_adi
  endif
endif
ifeq ($(CONFIG_IPSEC_USE_MPC_CRYPTO),y)
  VENDOR_SUFFIX_ENC=_mpc
endif

ifeq ($(CONFIG_IPSEC_USE_DANUBE_CRYPTO),y)
  VENDOR_SUFFIX_ENC=_danube
# as soon as B45763 is resolved. Need to compare performance before setting.
# VENDOR_SUFFIX_AUTH=_danube
endif

ifeq ($(CONFIG_IPSEC_USE_LANTIQ_DEU_CRYPTO),y)
  VENDOR_SUFFIX_ENC=_lantiq_deu
  VENDOR_SUFFIX_AUTH=_lantiq_deu
endif

ifeq ($(CONFIG_IPSEC_USE_CENTILLIUM_CRYPTO),y)
  VENDOR_SUFFIX_ENC=_centillium
# Authentication isn't supported B47455
# VENDOR_SUFFIX_AUTH=_centillium
  JMK_INTERNAL_HEADERS+=centillium_ipsec_utils.h 
  JMK_MOD_TARGET+=centillium_ipsec_util_mod.o 
  JMK_OX_OBJS_centillium_ipsec_util_mod.o+=centillium_ipsec_utils.o
endif

ifeq ($(CONFIG_CADENCE_IPSEC2),y)
  VENDOR_SUFFIX_ENC=_cadence
  VENDOR_SUFFIX_AUTH=_cadence
  JMK_MOD_TARGET+=cadence_ipsec2.o
  JMK_O_OBJS_cadence_ipsec2.o=ipsec_alg_cadence.o cadence_ipsec2.o \
    cadence_ipsec2_itf.o cadence_ipsec2_reverse.o
endif

ifeq ($(CONFIG_IPSEC_USE_OCTEON_CRYPTO)$(CONFIG_IPSEC_ALG_JOINT),ym)
  VENDOR_SUFFIX_JOINT=_octeon
  JMK_O_OBJS_ipsec_joint.o+=cavium_api.o
  JMK_MOD_CFLAGS+=-DOCTEON_IPSEC -DCONFIG_CAVIUM_OPTIM
  COMMON_CFLAGS+=-DOCTEON_IPSEC
endif


ifeq ($(CONFIG_ELLIPTIC_IPSEC)$(CONFIG_IPSEC_ALG_JOINT),ym)
  VENDOR_SUFFIX_JOINT=_comcerto100
endif

ifeq ($(CONFIG_IPSEC_USE_IXP4XX_CRYPTO)$(CONFIG_IPSEC_ALG_JOINT),ym)
  VENDOR_SUFFIX_JOINT=_ixp
  JMK_O_OBJS_ipsec_joint.o+=ipsec_ixp_glue_mbuf.o
  JMK_OX_OBJS_ipsec_joint.o+=ipsec_ixp_hwaccel.o

  JMK_LINKS=ipsec_ixp_glue_mbuf.c ipsec_ixp_hwaccel.c
  $(JMK_LINKS):
	$(JMKE_LN) $(RG_INCLUDE_DIR)/freeswan/klips/net/ipsec/c/$@ $@

  ifdef CONFIG_RG_OS_LINUX_26
    include $(JMK_ROOT)/vendor/intel/ixp4xx/csr/envir.subdirs.mak
  else
    IXP425_SW := $(JMK_ROOT)/vendor/intel/ixp425/csr
    JMK_MOD_CFLAGS+=-I$(IXP425_SW)/include -I$(IXP425_SW)/linux
  endif
endif

ifeq ($(CONFIG_IPSEC_USE_FEROCEON_CRYPTO)$(CONFIG_IPSEC_ALG_JOINT),ym)
  VENDOR_SUFFIX_JOINT=_feroceon
  FEROCEON_PATH=$(JMK_ROOT)/vendor/marvell/feroceon/linux-2.6
  JMK_MOD_CFLAGS+=-I$(FEROCEON_PATH)/arch/arm/plat-feroceon/mv_hal/cesa \
    -I$(FEROCEON_PATH)/arch/arm/plat-feroceon/common \
    -I$(FEROCEON_PATH)/arch/arm/mach-feroceon-kw2/kw2_family \
    -I$(FEROCEON_PATH)/arch/arm/mach-feroceon-kw2/config \
    -I$(FEROCEON_PATH) -DNO_KERNEL_CRYPTO_FRAMEWORK
endif

endif #CONFIG_RG_GPL

JMK_MOD_O_TARGET=ipsec_alg_static.o
JMK_O_OBJS_$(JMK_MOD_O_TARGET)=ipsec_alg_static_init.o

ifdef CONFIG_IPSEC_ALG_JOINT
  ifneq ($(VENDOR_SUFFIX_JOINT),)
    JMK_MOD_TARGET+=ipsec_joint.o
    JMK_O_OBJS_ipsec_joint.o+=ipsec_alg_joint$(VENDOR_SUFFIX_JOINT).o
  endif
else
  ifeq ($(CONFIG_IPSEC_ALG_MD5),m)
    JMK_MOD_TARGET+=ipsec_md5.o
    JMK_O_OBJS_ipsec_md5.o=ipsec_alg_md5$(VENDOR_SUFFIX_AUTH).o
    ifeq ($(VENDOR_SUFFIX_AUTH),)
      JMK_L_OBJS_ipsec_md5.o=libmd5/libmd5.o
    endif
    JMK_SUBDIRS+=libmd5
  endif
  
  ifeq ($(CONFIG_IPSEC_ALG_SHA1),m)
    JMK_MOD_TARGET+=ipsec_sha1.o
    JMK_O_OBJS_ipsec_sha1.o=ipsec_alg_sha1$(VENDOR_SUFFIX_AUTH).o
    ifeq ($(VENDOR_SUFFIX_AUTH),)
      JMK_L_OBJS_ipsec_sha1.o=libsha1/libsha1.o
    endif
    JMK_SUBDIRS+=libsha1
  endif
  
  ifeq ($(CONFIG_IPSEC_ALG_1DES),m)
    JMK_MOD_TARGET+=ipsec_1des.o
    JMK_O_OBJS_ipsec_1des.o=ipsec_alg_1des$(VENDOR_SUFFIX_ENC).o
    ifeq ($(VENDOR_SUFFIX_ENC),)
      JMK_L_OBJS_ipsec_1des.o=../libdes/libdes.o
    endif
  endif
  
  ifeq ($(CONFIG_IPSEC_ALG_3DES),m)
    JMK_MOD_TARGET+=ipsec_3des.o
    JMK_O_OBJS_ipsec_3des.o=ipsec_alg_3des$(VENDOR_SUFFIX_ENC).o
    ifeq ($(VENDOR_SUFFIX_ENC),)
      JMK_L_OBJS_ipsec_3des.o=../libdes/libdes.o
    endif
  endif
  
  ifeq ($(CONFIG_IPSEC_ALG_AES),m)
    JMK_MOD_TARGET+=ipsec_aes.o
  # AES HW ENC for ADI does not work. When works - remove this if
    ifeq ($(CONFIG_ADI_6843),y)
      JMK_O_OBJS_ipsec_aes.o=ipsec_alg_aes.o
      JMK_L_OBJS_ipsec_aes.o=libaes/libaes.o
    else
      JMK_O_OBJS_ipsec_aes.o=ipsec_alg_aes$(VENDOR_SUFFIX_ENC).o
    endif
    ifeq ($(VENDOR_SUFFIX_ENC),)
      JMK_L_OBJS_ipsec_aes.o=libaes/libaes.o
    endif
    JMK_SUBDIRS+=libaes
  endif
  
  ifeq ($(CONFIG_IPSEC_ALG_NULL),m)
    JMK_MOD_TARGET+=ipsec_null.o
    JMK_O_OBJS_ipsec_null.o=ipsec_alg_null.o
  endif
endif

ifeq ($(CONFIG_IPSEC_DEBUG),y)
  JMK_MOD_CFLAGS+=-g
endif

JMK_CFLAGS_$(JMK_MOD_O_TARGET)+=$(COMMON_CFLAGS)
JMK_MOD_CFLAGS+=$(COMMON_CFLAGS)

JMK_RAMDISK_MODULES_FILES+=$(JMK_MOD_TARGET)

include $(JMKE_MK)

