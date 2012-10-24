ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../..
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_LEGACY_SUBDIRS+=madwifi_org

JMK_LEGACY_SUBDIRS+=$(if $(CONFIG_RG_DEV_IF_AR8316_HW_SWITCH),rg)
JMK_LEGACY_SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_HW_AR5212),madwifi)

# XXX - Hack for Fibertec:
# Do not build pb42 for Fibertec (HW(fibertec) == DB88F6560BP). However, do
# include pb42 if DIST=JPKG_SRC.
ifneq ($(CONFIG_RG_HW), "DB88F6560BP")
ifneq ($(CONFIG_RG_ATHEROS_HW_AR5416),)
ifneq ($(CONFIG_RG_JPKG)-$(CONFIG_RG_ATHEROS_FUSION),-y)
  JMK_LEGACY_SUBDIRS+=pb42
endif
endif
endif

JMK_SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_FUSION),fusion)

$(call JMKE_INCLUDE_RULES)
