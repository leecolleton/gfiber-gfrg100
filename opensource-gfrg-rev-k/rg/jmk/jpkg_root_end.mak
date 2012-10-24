sav_JMK_JPKG_EXPORTED_FILES += \
  $(export_headers_srcs) $(sav_JMK_CD_EXPORTED_FILES) \
  $(wildcard $(sav_JMK_JPKG_EXPORTED_IF_EXIST)) \
  $(if $(CONFIG_RG_JPKG_SRC),$(sav_JMK_INTERNAL_HEADERS))

jpkg_exported := $(sort \
  $(filter-out $(sav_JMK_DONT_EXPORT), \
    $(sav_JMK_JPKG_EXPORTED_FILES) \
    $(if $(strip $(sav_JMK_JPKG_EXPORTED_DIR)), \
      $(shell find $(sav_JMK_JPKG_EXPORTED_DIR) -not -regex '^\(\|.*/\)CVS/.*$$' \
	-and -type f -and -not -regex '.*\#.*'))))

# export_to_jpkg

# $1 - source
# $2 - destination
Jpkg_Export = \
  $(eval $2: $1 jpkg_rm_exported_dir ; \
    $(RG_SHELL_FUNCS) && rg_vpath_cp $1 $2)

$(foreach f,$(jpkg_exported), \
  $(call Jpkg_Export,$f,$(call Abs_Path_To_Jpkg,$f)))
export_to_jpkg: $(call Abs_Path_To_Jpkg,$(jpkg_exported))

jpkg_rm_exported_dir:
	$(if $(strip $(sav_JMK_JPKG_EXPORTED_DIR)), \
	  rm -rf $(call Abs_Path_To_Jpkg,$(sav_JMK_JPKG_EXPORTED_DIR)))

internal_phony_targets+=jpkg_rm_exported_dir
