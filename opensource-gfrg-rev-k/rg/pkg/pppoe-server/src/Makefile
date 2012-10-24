JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_PPPOES
JMK_SUBDIRS+=libevent

JMK_TARGET=pppoe pppoe-server

JMK_O_OBJS_pppoe-server=pppoe-server.o if.o debug.o common.o md5.o
JMK_O_OBJS_pppoe=pppoe.o if.o debug.o common.o ppp.o discovery.o

JMK_RAMDISK_ETC_FILES=/ppp/pppoe-server-options

# Configuration file path
PPPOESERVER_PPPD_OPTIONS=/etc/ppp/pppoe-server-options

# Program paths
PPPOE_PATH=/bin/pppoe
PPPD_PATH=/bin/pppd
PLUGIN_PATH=/etc/ppp/plugin

PATHS='-DPPPOE_PATH="$(PPPOE_PATH)"' '-DPPPD_PATH="$(PPPD_PATH)"' '-DPLUGIN_PATH="$(PLUGIN_PATH)"' '-DPPPOE_SERVER_OPTIONS="$(PPPOESERVER_PPPD_OPTIONS)"'

VERSION=3.5
JMK_CFLAGS+= $(PATHS) -DVERSION=$(VERSION)
JMK_LDFLAGS+=-Llibevent
JMK_LDLIBS:=-levent $(JMK_LDLIBS)

JMK_CD_EXPORTED_FILES+=pppoe-server-options
endif

ifdef CONFIG_RG_PPPOE_RELAY
  JMK_MOD_TARGET=pppoe_relay.o
  JMK_RAMDISK_MODULES_FILES+=$(JMK_MOD_TARGET)
  JMK_OX_OBJS_pppoe_relay.o=relay.o
  JMK_INTERNAL_HEADERS+=relay.h pppoe.h

  ifeq ($(CONFIG_RG_PPPOE_RELAY),y)
    JMK_MOD_2_STAT+=$(JMK_MOD_TARGET)
  endif
endif

include $(JMKE_MK)
