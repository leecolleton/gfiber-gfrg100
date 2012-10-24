JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_shell.a+=$(if $(CONFIG_ASH),ash.o)
JMK_O_OBJS_shell.a+=$(if $(CONFIG_HUSH),hush.o)
JMK_O_OBJS_shell.a+=$(if $(CONFIG_LASH),lash.o)
JMK_O_OBJS_shell.a+=$(if $(CONFIG_MSH),msh.o)
JMK_O_OBJS_shell.a+=$(if $(CONFIG_FEATURE_COMMAND_EDITING),cmdedit.o)

ifneq ($(strip $(JMK_O_OBJS_shell.a)),)
  JMK_A_TARGET+=shell.a
endif

include $(JMKE_MK)
