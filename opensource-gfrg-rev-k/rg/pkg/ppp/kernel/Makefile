ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../..
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS_DIR=ppp/kernel
JMK_EXPORT_HEADERS+=be_api.h be_pppoe.h be_pppoh.h be_chardev.h be_pptp.h \
		    if_ppp.h be_l2tp.h
JMK_EXPORT_HEADERS+=rg_ppp_defs.h ppp-comp.h 
JMK_INTERNAL_HEADERS+=ppp_mppe.h if_pppvar.h
JMK_EXPORT_AS_SRC+=kernel.c ppp_compress.c

JMK_LOG_ENTITY_be_pptp.o=LOG_ENTITY_PPTP
JMK_LOG_ENTITY_be_l2tp.o=LOG_ENTITY_L2TP

CRYPTO_PATH=$(JMK_ROOT)/pkg/openssl/crypto

CRYPTO_FILES=md32_common.h rc4/rc4.h rc4/rc4_locl.h
CRYPTO_CODE_FILES+=rc4/rc4_enc.c rc4/rc4_skey.c
CRYPTO_FILES+=$(CRYPTO_CODE_FILES)
CRYPTO_LINKS=$(notdir $(CRYPTO_FILES))

ifeq ($(CONFIG_RG_ONE_MODULE)$(CONFIG_RG_PPP_COMMON),mm)
JMK_MOD_O_TARGET=ppp.o
else
JMK_MOD_TARGET=ppp.o
JMK_RAMDISK_MODULES_FILES+=ppp.o
endif

JMK_O_OBJS_ppp.o=be_chardev.o kernel.o ppp_compress.o
JMK_OX_OBJS_ppp.o+=if_ppp.o if_ppp_pppoa.o if_ppp_pppos.o
JMK_EXPORT_AS_SRC+=if_ppp_pppoa.c if_ppp_pppos.c

ifdef CONFIG_RG_PPPOE
JMK_O_OBJS_ppp.o+=be_pppoe.o
endif

ifdef CONFIG_RG_PPTPC
JMK_O_OBJS_ppp.o+=be_pptp.o
endif
ifneq ($(CONFIG_RG_L2TPC)$(CONFIG_RG_L2TPS),)
  JMK_O_OBJS_ppp.o+=be_l2tp.o
endif

ifdef CONFIG_RG_PPPOA
JMK_MOD_TARGET+=be_pppoa_mod.o
JMK_RAMDISK_MODULES_FILES+=be_pppoa_mod.o
JMK_OX_OBJS_be_pppoa_mod.o=be_pppoa.o
endif

ifdef CONFIG_RG_PPPOS
JMK_MOD_TARGET+=be_pppos_mod.o
JMK_RAMDISK_MODULES_FILES+=be_pppos_mod.o
JMK_OX_OBJS_be_pppos_mod.o=be_pppos.o
endif

ifeq ($(CONFIG_RG_PPP_COMMON),y)
JMK_MOD_2_STAT+=ppp.o
endif

ifeq ($(CONFIG_RG_PPPOA),y)
JMK_MOD_2_STAT+=be_pppoa_mod.o
endif

ifeq ($(CONFIG_RG_PPPOS),y)
JMK_MOD_2_STAT+=be_pppos_mod.o
endif

# PPP compressors
ifneq ($(CONFIG_RG_PPP_DEFLATE)$(CONFIG_RG_PPP_BSDCOMP)$(CONFIG_RG_PPP_MPPE),)
JMK_MOD_CFLAGS+=-DPPP_COMPRESS
endif

ifneq ($(CONFIG_RG_PPP_DEFLATE)$(CONFIG_RG_PPP_BSDCOMP),)
ifdef CONFIG_RG_PPP_DEFLATE
  JMK_MOD_CFLAGS+=-DPPP_COMPRESS_DEFLATE
  JMK_O_OBJS_ppp.o+=ppp-deflate.o
endif
ifdef CONFIG_RG_PPP_BSDCOMP
  JMK_MOD_CFLAGS+=-DPPP_COMPRESS_BSDCOMP
  JMK_O_OBJS_ppp.o+=bsd-comp.o
endif
endif

ifdef CONFIG_RG_PPP_MPPE
JMK_O_OBJS_ppp.o+=ppp_mppe.o
ifneq ($(CRYPTO_CODE_FILES),)
  JMK_O_OBJS_ppp.o+=$(notdir $(CRYPTO_CODE_FILES:%.c=%.o))
endif
JMK_MOD_CFLAGS+=-DPPP_COMPRESS_MPPE -I$(CRYPTO_PATH)
JMK_ARCHCONFIG_FIRST_TASKS+=$(JMKE_PWD_BUILD)/crypto_links
endif

ifdef CONFIG_RG_HSS
JMK_O_OBJS_ppp.o+=be_pppoh.o
endif

ifdef CONFIG_RG_GPL
JMK_MOD_TARGET_PPPoA:=$(filter be_pppoa_mod.o,$(JMK_MOD_TARGET))
JMK_MOD_TARGET_PPPoS:=$(filter be_pppos_mod.o,$(JMK_MOD_TARGET))
JMK_MOD_TARGET:=$(JMK_MOD_TARGET_PPPoA) $(JMK_MOD_TARGET_PPPoS)
JMK_MOD_O_TARGET=
JMK_MOD_2_STAT=
endif

$(call JMKE_INCLUDE_RULES)

$(JMKE_PWD_BUILD)/crypto_links: $(addprefix $(CRYPTO_PATH)/, $(CRYPTO_FILES))
	@echo "Generating MPPE openssl/crypto links:"
	@for f in $(JMKE_DEPS) ; do \
	  $(JMKE_LN) $$f $(@D) ; \
	  echo $$f ; \
	done

ctags:
	ctags -R $(JMK_ROOT)/os/linux $(JMK_ROOT)/pkg/kos $(JMK_ROOT)/pkg/include .
