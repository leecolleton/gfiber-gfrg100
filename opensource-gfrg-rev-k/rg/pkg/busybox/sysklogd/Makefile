JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_sysklogd.a+=$(if $(CONFIG_KLOGD),klogd.o)
JMK_O_OBJS_sysklogd.a+=$(if $(CONFIG_LOGGER),logger.o)
JMK_O_OBJS_sysklogd.a+=$(if $(CONFIG_LOGREAD),logread.o)
JMK_O_OBJS_sysklogd.a+=$(if $(CONFIG_SYSLOGD),syslogd.o)

ifneq ($(strip $(JMK_O_OBJS_sysklogd.a)),)
  JMK_A_TARGET+=sysklogd.a
endif

include $(JMKE_MK)
