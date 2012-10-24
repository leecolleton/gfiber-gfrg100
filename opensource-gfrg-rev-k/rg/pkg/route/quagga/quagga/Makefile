ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_INTERNAL_HEADERS+=config.h

JMK_CD_EXPORTED_FILES+=common.mak

ifneq ($(CONFIG_RG_BGP)$(CONFIG_RG_OSPF),)
  JMK_SUBDIRS=lib zebra
endif

ifneq ($(CONFIG_RG_BGP),)
  JMK_SUBDIRS+=bgpd
endif

ifneq ($(CONFIG_RG_OSPF),)
  JMK_SUBDIRS+=ospfd
endif

$(call JMKE_INCLUDE_RULES)
