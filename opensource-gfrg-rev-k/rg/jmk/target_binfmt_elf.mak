LINKAGE_MAP_FLAGS=-Wl,-Map $@.link.map
TRACE_FLAG=-Wl,--trace

$(sav_JMK_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  touch $@.link.map; \
	  perl $(JMK_ROOT)/pkg/build/app_rm_unneeded_syms.pl $(JMK_ROOT) $@ \
	    $(notdir $@) $(subst /,_,$(call Abs_Path_To_Rel,$@)) \
	    $(CC) $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(filter-out $(sav_JMK_LDFLAGS_REMOVE_$(call Abs_Path_To_Var,$@)), \
	      $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@))) $(LDFLAGS_ENVIR) \
	    $(call Get_Libs_Flags,$(sav_JMK_LIBS_$(call Abs_Path_To_Var,$@))) \
	    $(TRACE_FLAG) $(LINKAGE_MAP_FLAGS) -o $@, \
	  echo "Skipping JMK_TARGET($@:$^) since creating JPKG" \
	 )
	$(JMKE_LN) $@ $(DEBUG_PATH)/$(notdir $@)
