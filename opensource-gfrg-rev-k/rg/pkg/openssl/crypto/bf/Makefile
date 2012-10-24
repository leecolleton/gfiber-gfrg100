ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=blowfish.h
JMK_INTERNAL_HEADERS=bf_pi.h bf_locl.h

BF_ENC=bf_enc.o
JMK_O_OBJS=bf_skey.o bf_ecb.o $(BF_ENC) bf_cfb64.o bf_ofb64.o

$(call JMKE_INCLUDE_RULES)

