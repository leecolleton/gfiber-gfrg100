# Language files
ifdef CONFIG_RG_LANG
CSV_LANG_FILES=$(call VPATH_WILDCARD,*_lang.csv)

H_LANG_FILES=$(if $(CONFIG_RG_JPKG_SRC),,$(patsubst %.csv,%.h,$(CSV_LANG_FILES)))

# No need for *_lang.[co] files in a binary jpkg distribution
JMKE_O_LANG_FILES=$(if $(CONFIG_RG_JPKG),,$(patsubst %.csv,%.o,$(CSV_LANG_FILES)))
C_LANG_FILES=$(if $(CONFIG_RG_JPKG),,$(patsubst %.csv,%.c,$(CSV_LANG_FILES)))

JMK_EXPORT_HEADERS+=$(C_LANG_FILES) $(H_LANG_FILES)

# Only the _lang.csv file needs to be exported to JPKG.
JMK_DONT_EXPORT+=$(H_LANG_FILES) $(C_LANG_FILES)

JMK_JPKG_EXPORTED_IF_EXIST+=$(CSV_LANG_FILES)
endif
