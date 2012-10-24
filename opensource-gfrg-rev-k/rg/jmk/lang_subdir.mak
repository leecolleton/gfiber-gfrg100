ifdef CONFIG_RG_LANG

  cur_csv_lang_files:=$(CSV_LANG_FILES)
  C_LANG_FILES:=$(if $(CONFIG_RG_JPKG),,$(call SRC2BUILD, \
    $(cur_csv_lang_files:%.csv=%.c)))
  H_LANG_FILES:=$(if $(CONFIG_RG_JPKG_SRC),,$(call SRC2BUILD, \
    $(cur_csv_lang_files:%.csv=%.h)))

  ifneq ($(C_LANG_FILES)$(H_LANG_FILES),)
    ifndef CONFIG_RG_GPL
      JMK_ARCHCONFIG_FIRST_TASKS+=$(H_LANG_FILES) $(C_LANG_FILES) 
    endif

    JMK_EXPORT_HEADERS+=$(C_LANG_FILES) $(H_LANG_FILES)

    # Only the _lang.csv file needs to be exported to JPKG.
    JMK_DONT_EXPORT+=$(H_LANG_FILES) $(C_LANG_FILES)

    JMK_AUTOGEN_SRC+=$(H_LANG_FILES) $(C_LANG_FILES)
  endif

  JMK_JPKG_EXPORTED_FILES+=$(cur_csv_lang_files)
endif
