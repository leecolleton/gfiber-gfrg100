JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_DYN_LINK

JMK_RAMDISK_LIB_FILES=$(wildcard *0.9.29.so)
RAMDISK_LINKS=$(foreach f,$(wildcard *so*),$(if $(shell readlink $f),$f))
JMK_RAMDISK_LAST_TASKS+=ramdisk_links
ramdisk_links:
	$(foreach l,$(RAMDISK_LINKS),$(call RAMDISK_LN_FUNC,$(JMKE_CRAMFS_RUNTIME_MOUNT_POINT)/lib/$(shell readlink $l),$(JMKE_RAMDISK_RW_DIR)/lib/$l) &&) true
endif

include $(JMKE_MK)

