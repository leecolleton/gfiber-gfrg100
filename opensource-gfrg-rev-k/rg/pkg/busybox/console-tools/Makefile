JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_CHVT),chvt.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_CLEAR),clear.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_DEALLOCVT),deallocvt.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_DUMPKMAP),dumpkmap.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_LOADFONT),loadfont.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_LOADKMAP),loadkmap.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_OPENVT),openvt.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_RESET),reset.o)
JMK_O_OBJS_console-tools.a+=$(if $(CONFIG_SETKEYCODES),setkeycodes.o)

ifneq ($(strip $(JMK_O_OBJS_console-tools.a)),)
  JMK_A_TARGET+=console-tools.a
endif

include $(JMKE_MK)
