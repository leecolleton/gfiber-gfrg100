JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_ADJTIMEX),adjtimex.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_CROND),crond.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_CRONTAB),crontab.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_DC),dc.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_DEVFSD),devfsd.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_HDPARM),hdparm.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_LAST),last.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_MAKEDEVS),makedevs.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_MT),mt.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_RX),rx.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_STRINGS),strings.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_TIME),time.o)
JMK_O_OBJS_miscutils.a+=$(if $(CONFIG_WATCHDOG),watchdog.o)

ifneq ($(strip $(JMK_O_OBJS_miscutils.a)),)
  JMK_A_TARGET+=miscutils.a
endif

include $(JMKE_MK)
