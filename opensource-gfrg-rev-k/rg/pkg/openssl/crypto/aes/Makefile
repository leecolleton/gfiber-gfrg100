ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=aes.h
JMK_INTERNAL_HEADERS=aes_locl.h

AES_ENC=aes_core.o aes_cbc.o
JMK_O_OBJS=aes_misc.o aes_ecb.o aes_cfb.o aes_ofb.o aes_ctr.o aes_ige.o \
	   aes_wrap.o $(AES_ENC)

$(call JMKE_INCLUDE_RULES)

