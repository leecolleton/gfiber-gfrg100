JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_init.a+=$(if $(CONFIG_HALT),halt.o)
JMK_O_OBJS_init.a+=$(if $(CONFIG_INIT),init.o)
JMK_O_OBJS_init.a+=$(if $(CONFIG_MESG),mesg.o)
JMK_O_OBJS_init.a+=$(if $(CONFIG_POWEROFF),poweroff.o)
JMK_O_OBJS_init.a+=$(if $(CONFIG_REBOOT),reboot.o)

JMK_O_OBJS_init.a+=$(if $(CONFIG_HALT)$(CONFIG_INIT)$(CONFIG_POWEROFF)$(CONFIG_REBOOT),init_shared.o)

ifneq ($(strip $(JMK_O_OBJS_init.a)),)
  JMK_A_TARGET+=init.a
endif

include $(JMKE_MK)
