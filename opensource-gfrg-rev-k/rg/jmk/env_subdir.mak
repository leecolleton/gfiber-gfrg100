include $(JMK_ROOT)/jmk/config_env_subdir.mak
include $(JMK_ROOT)/jmk/build_dir_env_subdir.mak

JMK_JPKG_EXPORTED_FILES+=Makefile
JMK_JPKG_EXPORTED_IF_EXIST+=envir.subdirs.mak

# XXX some flags may be overridden, so we need to reload defaults on every dir
include $(wildcard $(CONFIG_FILE))
include $(JMK_ROOT)/jmk/envir_subdirs_subdir.mak

include $(JMK_ROOT)/jmk/flags_env_subdir.mak
