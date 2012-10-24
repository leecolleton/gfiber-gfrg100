ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../..
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS_m+=common
JMK_SUBDIRS_$(CONFIG_RG_OS_LINUX)+=linux
JMK_SUBDIRS_$(CONFIG_RG_ONE_MODULE)+=one_module

$(call JMKE_INCLUDE_RULES)
