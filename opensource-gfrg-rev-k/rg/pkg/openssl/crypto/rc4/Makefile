ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=rc4.h
JMK_INTERNAL_HEADERS=rc4_locl.h

RC4_ENC=rc4_enc.o rc4_skey.o
JMK_O_OBJS=$(RC4_ENC) rc4_utl.o

ifdef CONFIG_RG_PPP_MPPE
  SRC=rc4_enc.c rc4_skey.c rc4_locl.h
  JMK_CD_EXPORTED_FILES+=$(SRC)
endif

$(call JMKE_INCLUDE_RULES)
