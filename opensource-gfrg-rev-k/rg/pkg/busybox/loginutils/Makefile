JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_ADDGROUP),addgroup.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_ADDUSER),adduser.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_GETTY),getty.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_LOGIN),login.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_PASSWD),passwd.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_SU),su.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_SULOGIN),sulogin.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_VLOCK),vlock.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_DELUSER),deluser.o)
JMK_O_OBJS_loginutils.a+=$(if $(CONFIG_DELGROUP),delgroup.o)

ifneq ($(CONFIG_LOGIN)$(CONFIG_PASSWD)$(CONFIG_SU)$(CONFIG_SULOGIN)$(CONFIG_VLOCK),)
  JMK_LDLIBS_loginutils.a:=lcrypt $(JMK_LDLIBS_loginutils.a)
endif

ifneq ($(strip $(JMK_O_OBJS_loginutils.a)),)
  JMK_A_TARGET+=loginutils.a
endif

include $(JMKE_MK)
