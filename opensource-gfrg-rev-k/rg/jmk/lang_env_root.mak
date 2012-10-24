ifdef CONFIG_RG_LANG
  CSV_LANG_FILES=$(wildcard $(JMKE_PWD_SRC)/*_lang.csv)

  # No need for *_lang.[co] files in a binary jpkg distribution
  JMKE_O_LANG_FILES=$(if $(CONFIG_RG_JPKG),,$(call SRC2BUILD, \
    $(CSV_LANG_FILES:%.csv=%.o)))
endif
