MOD_2_STAT_LINKS:=$(addprefix $(STATIC_MOD_DIR),$(filter-out $(JMK_MOD_O_TARGET),$(JMK_MOD_2_STAT)))

$(MOD_2_STAT_LINKS):
	$(JMKE_LN) $(CUR_DIR)/$(notdir $@) $(dir $@)$(JMK_MOD_2_STAT_PREFIX_$(notdir $@))$(notdir $@)
