ifdef ALL_MULTIPLE_OBJS

ALL_CLEAN_FILES+=$(ALL_MULTIPLE_OBJS)

# the wildcard is used for binary jpkg trees, where there are no sources
# $1 - object
# $2 - base name
_Wildcard_Multiple_Src= \
  $(if $(wildcard $2.c), \
    $(eval _ALL_MULTIPLE_OBJS_RENAMED_SRCS+=$(1:%.o=%.c)) \
    $(eval _ALL_MULTIPLE_OBJS_ORIG_SRCS+=$2.c), \
    $(if $(wildcard $2.cpp), \
      $(eval _ALL_MULTIPLE_OBJS_RENAMED_SRCS+=$(1:%.o=%.cpp)) \
      $(eval _ALL_MULTIPLE_OBJS_ORIG_SRCS+=$2.cpp)))

$(foreach o,$(ALL_MULTIPLE_OBJS), \
  $(call _Wildcard_Multiple_Src, $o, \
    $(call BUILD2SRC,$(dir $o))$(basename $(basename $(notdir $o)))))

ifdef CONFIG_RG_JPKG_SRC
  # We want to export the original src files in the src packages, and not the 
  # renamed ones.
  sav_JMK_JPKG_EXPORTED_FILES+=$(_ALL_MULTIPLE_OBJS_ORIG_SRCS)
  sav_JMK_DONT_EXPORT+=$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS)
else # CONFIG_RG_JPKG_SRC
  sav_JMK_ARCHCONFIG_LAST_TASKS+=$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS)
endif

$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS):
	$(JMKE_LN_NOFAIL) $(call BUILD2SRC,$(@D))/$(basename $(*F)).$(suffix $@)
	@echo "/* Note - autoconf.h is included automatically by LINUX_CFLAGS" > $@
	@echo " * there is no protection here against double inclusion of autoconf.h" >> $@
	@echo " * when working on vendor code that needs MULTIPLE compilation," >> $@
	@echo " * clean the code from any '#include <linux/autoconf.h>' */" >> $@
	@echo "#ifdef __KERNEL__" >> $@
	@echo "#define _RG_CONFIG_H_ /* prevent use of rg_config.h */">> $@
	@echo "#else" >> $@
	@echo "#include <rg_config.h>" >> $@
	@echo "#endif" >> $@
	@echo "#define _LINUX_CONFIG_H /* prevent use of linux/config.h */">> $@
	@for config in \
	  $(sav_JMK_COMPILE_MULTIPLE_CONFIGS_$(call Abs_Path_To_Dir_Var, $@)) \
	  ; do \
	  echo "#undef $$config" >> $@ ; \
	done
	@echo "#define CONFIG_$(subst .,,$(suffix $(basename $@)))" >> $@
	@echo "#include \"$(notdir $(basename $(basename $@)))$(suffix $@)\"" \
	    >> $@

endif # ALL_MULTIPLE_OBJS
