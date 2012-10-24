ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_RAMDISK_ETC_FILES=nsswitch.conf protocols services resolv.conf hosts

ifdef CONFIG_LSP_DIST
  JMK_RAMDISK_ETC_FILES+=passwd
endif

# Check if we have ld.so.cache for this arch
ifdef CONFIG_ULIBC
  ifdef CONFIG_DYN_LINK
    LD_SO_FILE=ld.so.cache.$(ARCH)$(ENDIANESS_SUFFIX)
    ifneq ($(wildcard $(LD_SO_FILE)),)
      JMK_CD_EXPORTED_FILES+=$(LD_SO_FILE)
      JMK_RAMDISK_FILES+=$(LD_SO_FILE)__/etc/ld.so.cache
    endif
  endif
endif

JMK_CD_EXPORTED_FILES+=passwd fstab $(JMK_RAMDISK_ETC_FILES)
JMK_RAMDISK_RW_FILES+=fstab__/etc/fstab

JMK_RAMDISK_LAST_TASKS+=ramdisk_conf_files
ramdisk_conf_files:
	$(call RAMDISK_LN_FUNC,$(JMKE_CRAMFS_RUNTIME_MOUNT_POINT)/etc/ld.so.cache,$(JMKE_RAMDISK_RW_DIR)/etc/ld.so.cache)

$(call JMKE_INCLUDE_RULES)
