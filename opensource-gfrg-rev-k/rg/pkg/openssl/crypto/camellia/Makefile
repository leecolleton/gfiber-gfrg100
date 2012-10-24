ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=camellia.h
JMK_INTERNAL_HEADERS=cmll_locl.h

CMLL_ENC=camellia.o cmll_misc.o cmll_cbc.o
JMK_O_OBJS=cmll_ecb.o cmll_ofb.o cmll_cfb.o cmll_ctr.o cmll_utl.o $(CMLL_ENC)

$(call JMKE_INCLUDE_RULES)

