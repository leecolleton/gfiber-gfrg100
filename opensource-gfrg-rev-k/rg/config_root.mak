ifdef ROOT_CONFIG_SECOND_PHASE
  ${if ${debug_create_config},${warning JMK: create_config SECOND phase}}
  include $(JMK_ROOT)/config_root_second_phase.mak
else
  ${if ${debug_create_config},${warning JMK: create_config FIRST phase}}
  include $(JMK_ROOT)/config_root_first_phase.mak
endif

$(MKDIR_SUBDIRS):
	$(JMKE_MKDIR) $@
