ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../..
  include $(JMK_ROOT)/jmk/env_root.mak
endif

ifndef CONFIG_LSP_DIST
  JMK_SUBDIRS+=lib atmt

  JMK_SUBDIRS+=$(if $(CONFIG_ATM_PVC_SCAN),pvc_scan)
  JMK_SUBDIRS+=$(if $(CONFIG_RG_OAM_PING),oam_ping)
endif

JMK_LEGACY_SUBDIRS+=$(if $(CONFIG_LINUX_OAM),oam)

$(call JMKE_INCLUDE_RULES)
