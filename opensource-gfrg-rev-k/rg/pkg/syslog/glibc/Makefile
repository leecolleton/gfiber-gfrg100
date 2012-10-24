ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS_DIR=syslog/glibc
JMK_EXPORT_HEADERS+=ulibc_syslog.h

JMK_A_TARGET=libulibc_syslog.a
JMK_O_OBJS_$(JMK_A_TARGET)=ulibc_syslog.o
JMK_EXPORT_LIBS+=$(JMK_A_TARGET)

$(call JMKE_INCLUDE_RULES)
