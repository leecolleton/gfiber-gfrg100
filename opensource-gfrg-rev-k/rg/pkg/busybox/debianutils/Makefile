JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_MKTEMP),mktemp.o)
JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_PIPE_PROGRESS),pipe_progress.o)
JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_READLINK),readlink.o)
JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_RUN_PARTS),run_parts.o)
JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_START_STOP_DAEMON),start_stop_daemon.o)
JMK_O_OBJS_debianutils.a+=$(if $(CONFIG_WHICH),which.o)

ifneq ($(strip $(JMK_O_OBJS_debianutils.a)),)
  JMK_A_TARGET+=debianutils.a
endif

include $(JMKE_MK)
