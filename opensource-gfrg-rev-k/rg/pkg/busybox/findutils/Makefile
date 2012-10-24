JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_findutils.a+=$(if $(CONFIG_FIND),find.o)
JMK_O_OBJS_findutils.a+=$(if $(CONFIG_GREP),grep.o)
JMK_O_OBJS_findutils.a+=$(if $(CONFIG_XARGS),xargs.o)

ifneq ($(strip $(JMK_O_OBJS_findutils.a)),)
  JMK_A_TARGET+=findutils.a
endif

include $(JMKE_MK)
