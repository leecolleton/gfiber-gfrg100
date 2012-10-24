ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS+=linux-2.6 rg
JMK_SUBDIRS+=$(if $(CONFIG_MARVELL_MMP),mmp)
JMK_LINK_DIRS+=linux-2.6
JMK_JPKG_EXPORTED_DIR_SRC+=linux-2.6

JMK_SUBDIRS+=$(if $(CONFIG_HW_PON_WAN),gpon_app)
JMK_LINK_DIRS+=$(if $(CONFIG_HW_PON_WAN),gpon_app)
JMK_JPKG_EXPORTED_DIR_SRC+=$(if $(CONFIG_HW_PON_WAN),gpon_app)

JMK_EXPORT_HEADERS_DIR=vendor
JMK_EXPORT_HEADERS+=flash_layout_db88F6560bp.c__c/flash_layout.c

$(call JMKE_INCLUDE_RULES)
