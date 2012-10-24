JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_A_TARGET+=procps.a

JMK_O_OBJS_procps.a+=$(if $(CONFIG_FREE),free.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_KILL),kill.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_PIDOF),pidof.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_PS),ps.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_RENICE),renice.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_SYSCTL),sysctl.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_TOP),top.o)
JMK_O_OBJS_procps.a+=$(if $(CONFIG_UPTIME),uptime.o)

include $(JMKE_MK)
