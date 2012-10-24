ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=cast.h
JMK_INTERNAL_HEADERS=cast_s.h cast_lcl.h

CAST_ENC=c_enc.o
JMK_O_OBJS=c_skey.o c_ecb.o $(CAST_ENC) c_cfb64.o c_ofb64.o

$(call JMKE_INCLUDE_RULES)

