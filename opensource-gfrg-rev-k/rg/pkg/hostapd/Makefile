JMK_ROOT=../..
include $(JMK_ROOT)/envir.mak

JMK_SUBDIRS=main


ifdef CONFIG_HOSTAPD_V_0_8
  JMK_SUBDIRS+=hostapd-0.8.x
  JMK_LINK_DIRS+=hostapd-0.8.x
  JMK_JPKG_EXPORTED_DIR_SRC+=hostapd-0.8.x
else
  JMK_SUBDIRS+=hostapd
  JMK_LINK_DIRS+=hostapd
  JMK_JPKG_EXPORTED_DIR_SRC+=hostapd
endif

include $(JMKE_MK)

