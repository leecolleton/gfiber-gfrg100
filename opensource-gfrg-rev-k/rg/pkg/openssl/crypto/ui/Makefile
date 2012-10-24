ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=ui.h ui_compat.h
JMK_INTERNAL_HEADERS=ui_locl.h

COMPATSRC=ui_compat.o
JMK_O_OBJS=ui_err.o ui_lib.o ui_openssl.o ui_util.o $(COMPATSRC)

$(call JMKE_INCLUDE_RULES)
