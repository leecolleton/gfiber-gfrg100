JMK_JPKG_EXPORTED_FILES+=$(call GET_FILE_FROM,$(JMK_EXPORT_HEADERS)) \
  $(JMK_CD_EXPORTED_FILES) $(foreach f,$(JMK_JPKG_EXPORTED_IF_EXIST),\
  $(if $(call VPATH_WILDCARD,$f),$f))
ifdef CONFIG_RG_JPKG_SRC
  JMK_JPKG_EXPORTED_FILES+=$(JMK_INTERNAL_HEADERS)
  JMK_JPKG_EXPORTED_DIR+=$(JMK_JPKG_EXPORTED_DIR_SRC)
endif

export_to_jpkg:
	@$(RG_SHELL_FUNCS) && \
	  $(foreach f,\
	  $(strip $(filter-out $(JMK_DONT_EXPORT),$(JMK_JPKG_EXPORTED_FILES))),\
	  rg_vpath_cp $f $(JPKG_CUR_DIR)/$f &&) true
ifneq ($(strip $(JMK_JPKG_EXPORTED_DIR)),)
	rm -rf $(addprefix $(PWD_JPKG)/,$(filter-out ., \
	    $(strip $(JMK_JPKG_EXPORTED_DIR))))
	cd $(JMKE_PWD_SRC) && (echo $(RG_SHELL_FUNCS) && \
	    find $(JMK_JPKG_EXPORTED_DIR) -not -regex \
	    '^\(\|.*/\)CVS/.*$$' -and -type f \
	    $(foreach f,$(JMK_DONT_EXPORT),-and -not -path "$f") | \
	    awk '{printf "rg_vpath_cp %s $(PWD_JPKG)/%s\n", $$1, $$1}') | \
	    bash -s$(if $(MAKEDEBUG),x)
endif

