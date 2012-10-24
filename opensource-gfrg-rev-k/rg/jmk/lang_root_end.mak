ifdef CONFIG_RG_LANG

lang_deps=$(filter-out $(LANG_COMPILER),$(JMKE_DEPS))

# add compilation rule for c/h language file
# $1 - target
Add_C_Lang_Rule = \
  $(eval $1: $(call BUILD2SRC,$(1:%c=%csv)) $(LANG_COMPILER) ; \
    $(LANG_COMPILER) --require-pos fmtcheck $$(lang_deps) && \
    $(LANG_COMPILER) $(LANG_COMPILER_OPT) csv2c $$(patsubst %_lang.csv,%,$$(lang_deps)) $$(lang_deps) $$@)

Add_H_Lang_Rule = \
  $(eval $1: $(call BUILD2SRC,$(1:%h=%csv)) $(LANG_COMPILER) ; \
    $(LANG_COMPILER) $(LANG_COMPILER_OPT) csv2h $$(patsubst %_lang.csv,%,$$(lang_deps)) $$(lang_deps) $$@)

$(foreach f,$(sav_JMK_AUTOGEN_SRC), \
  $(if $(filter %_lang.c, $f),$(call Add_C_Lang_Rule,$f), \
    $(if $(filter %_lang.h, $f),$(call Add_H_Lang_Rule,$f))))

endif
