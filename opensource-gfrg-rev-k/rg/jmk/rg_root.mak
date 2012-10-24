# For all subdirs, env_subdir.mak and rg_subdir.mak are being called from the
# subdirs engine. For the root Makefile we need to call them manually.
include $(JMK_ROOT)/jmk/rg_subdir.mak

include $(JMK_ROOT)/jmk/subdirs_root.mak

include $(JMK_ROOT)/jmk/sanity_check.mak

# preprocessing of input before lists creation, can't use lists in these files
include $(JMK_ROOT)/jmk/osap_root_end.mak
include $(JMK_ROOT)/jmk/multiple_root_end.mak

# lists and direct access vars creation
include $(JMK_ROOT)/jmk/list_vars_root_end.mak

# Note: From here on, included files can not add to JMK_xxx.
# For example, adding new targets to JMK_TARGET will cause unexpected behavior.

include $(JMK_ROOT)/jmk/flags_root_end.mak
include $(JMK_ROOT)/jmk/export_root_end.mak
include $(JMK_ROOT)/jmk/log_entity_root_end.mak
include $(JMK_ROOT)/jmk/sec_perm_root_end.mak
include $(JMK_ROOT)/jmk/jpkg_target_root_end.mak
ifdef CONFIG_RG_JPKG
  include $(JMK_ROOT)/jmk/jpkg_root_end.mak
endif
include $(JMK_ROOT)/jmk/lang_root_end.mak
include $(JMK_ROOT)/jmk/test_root_end.mak
include $(JMK_ROOT)/jmk/legacy_root_end.mak
include $(JMK_ROOT)/jmk/mod2stat_root_end.mak
include $(JMK_ROOT)/jmk/ramdisk_root_end.mak
ifdef CONFIG_RG_DLM
  include $(JMK_ROOT)/jmk/dlm_root_end.mak
endif

ifeq ($(JMKE_DOING_MAKE_CONFIG),y)
  include $(JMK_ROOT)/jmk/config_root_end.mak
endif

include $(JMK_ROOT)/jmk/general_dep_root_end.mak
include $(JMK_ROOT)/jmk/compile_root_end.mak
include $(JMK_ROOT)/jmk/clean_root_end.mak

# delete pre-defined rules
.SUFFIXES:
