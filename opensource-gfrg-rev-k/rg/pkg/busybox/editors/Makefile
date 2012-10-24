JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_editors.a+=$(if $(CONFIG_AWK),awk.o)
JMK_O_OBJS_editors.a+=$(if $(CONFIG_PATCH),patch.o)
JMK_O_OBJS_editors.a+=$(if $(CONFIG_SED),sed.o)
JMK_O_OBJS_editors.a+=$(if $(CONFIG_VI),vi.o)

ifneq ($(strip $(JMK_O_OBJS_editors.a)),)
  JMK_A_TARGET+=editors.a
endif

include $(JMKE_MK)
