# Language files
ifdef CONFIG_RG_LANG
ifdef JMKE_IS_BUILDDIR

ifndef CONFIG_RG_GPL
  JMK_ARCHCONFIG_LAST_TASKS+=$(H_LANG_FILES) $(C_LANG_FILES) 
  JMK_FIRST_TASKS+=$(H_LANG_FILES) $(C_LANG_FILES) 
endif

%_lang.c: $(JMKE_PWD_SRC)/%_lang.csv
	$(LANG_COMPILER) --require-pos fmtcheck $^
	$(LANG_COMPILER) $(LANG_COMPILER_OPT) csv2c $(notdir $(patsubst %_lang.csv,%,$^)) $^ $@

$(JMKE_PWD_BUILD)/%_lang.h %_lang.h: $(JMKE_PWD_SRC)/%_lang.csv
	$(LANG_COMPILER) $(LANG_COMPILER_OPT) csv2h $(notdir $(patsubst %_lang.csv,%,$^)) $^ $@

endif
endif
