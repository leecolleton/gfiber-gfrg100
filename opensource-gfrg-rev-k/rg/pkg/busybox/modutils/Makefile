JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_modutils.a+=$(if $(CONFIG_INSMOD),insmod.o)
JMK_O_OBJS_modutils.a+=$(if $(CONFIG_LSMOD),lsmod.o)
JMK_O_OBJS_modutils.a+=$(if $(CONFIG_MODPROBE),modprobe.o)
JMK_O_OBJS_modutils.a+=$(if $(CONFIG_RMMOD),rmmod.o)

ifneq ($(strip $(JMK_O_OBJS_modutils.a)),)
  JMK_A_TARGET+=modutils.a
endif

include $(JMKE_MK)
