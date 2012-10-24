include $(wildcard $(JMKE_PWD_BUILD)/*.d)
include $(JMK_ROOT)/jmk/flags_subdir.mak
include $(JMK_ROOT)/jmk/multiple_subdir.mak
include $(JMK_ROOT)/jmk/export_subdir.mak
include $(JMK_ROOT)/jmk/lang_subdir.mak
include $(JMK_ROOT)/jmk/jpkg_subdir.mak
include $(JMK_ROOT)/jmk/dirs_subdir.mak
include $(JMK_ROOT)/jmk/targets_subdir.mak
ifdef CONFIG_RG_DLM
  include $(JMK_ROOT)/jmk/dlm_subdir.mak
endif
