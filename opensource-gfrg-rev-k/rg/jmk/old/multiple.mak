ifdef JMK_COMPILE_MULTIPLE_CONFIGS

# Checking that at least one of the configs in the list is set 
IS_ANY_MULTIPLE_CONFIG_SET=$(strip \
  $(foreach conf,$(filter-out  $(JMK_COMPILE_MULTIPLE_CONFIGS_SKIP),\
	$(JMK_COMPILE_MULTIPLE_CONFIGS)),\
    $(if $($(conf)),\
      y\
    )\
  )\
)

ifeq ($(IS_ANY_MULTIPLE_CONFIG_SET),) 

# None of the configs in JMK_COMPILE_MULTIPLE_CONFIGS is set - add to JMK_O_OBJS
# the multiple objs as they are

# add to JMK_O_OBJS_ the multiple objs
$(foreach t,$(ALL_TARGETS),$(eval \
    JMK_O_OBJS_$t:=$(JMK_O_OBJS_$t) $(JMK_O_OBJS_MULTIPLE_$t) $(JMK_O_OBJS_MULTIPLE)))

# add to JMK_OX_OBJS_ the multiple objs
$(foreach t,$(ALL_TARGETS),$(eval \
    JMK_OX_OBJS_$t:=$(JMK_OX_OBJS_$t) $(JMK_OX_OBJS_MULTIPLE_$t) $(JMK_OX_OBJS_MULTIPLE)))

else #ifeq (IS_ANY_MULTIPLE_CONFIG_SET,)

# helper function for loops
# $1 - empty for JMK_O_OBJS , or
#      X     for JMK_OX_OBJS
# $2 - target name
_O_OR_OX_MULTIPLE_INDEX=$(call DBGFR, \
    $(if $(JMK_O$1_OBJS_MULTIPLE_$2)$(JMK_O$1_OBJS_MULTIPLE),\
        $(foreach conf,$(filter-out  $(JMK_COMPILE_MULTIPLE_CONFIGS_SKIP),\
	               $(JMK_COMPILE_MULTIPLE_CONFIGS)),\
	    $(if $($(conf)),\
	        $(JMK_O$1_OBJS_MULTIPLE_$2:%.o=%.$(conf:CONFIG_%=%).o)\
	        $(JMK_O$1_OBJS_MULTIPLE:%.o=%.$(conf:CONFIG_%=%).o)\
	    )\
        ),\
    )\
,_O_OR_OX_MULTIPLE_INDEX, $1, $2)

# create renamed objs for all configs
# e.g. for CONFIG_ABC - 
# a.o => a.ABC.o
$(foreach t,$(ALL_TARGETS),\
    $(eval _O_OBJS_MULTIPLE_RENAMED_$t:=$(call _O_OR_OX_MULTIPLE_INDEX,,$t)))

# create renamed X_objs for all configs
$(foreach t,$(ALL_TARGETS),\
    $(eval _OX_OBJS_MULTIPLE_RENAMED_$t:=$(call _O_OR_OX_MULTIPLE_INDEX,X,$t)))

# add to JMK_O_OBJS_ the renamed objs
$(foreach t,$(ALL_TARGETS),\
    $(eval JMK_O_OBJS_$t:=$(JMK_O_OBJS_$t) $(_O_OBJS_MULTIPLE_RENAMED_$t)))

# add to JMK_OX_OBJS_ the renamed objs
$(foreach t,$(ALL_TARGETS),\
    $(eval JMK_OX_OBJS_$t:=$(JMK_OX_OBJS_$t) $(_OX_OBJS_MULTIPLE_RENAMED_$t)))

# list of all of the multiple objects we use
_ALL_MULTIPLE_OBJS=$(foreach target,$(ALL_TARGETS),\
  $(_O_OBJS_MULTIPLE_RENAMED_$(target))\
  $(_OX_OBJS_MULTIPLE_RENAMED_$(target))\
)

ALL_CLEAN_FILES+=$(_ALL_MULTIPLE_OBJS)

# the wildcard is used for binary jpkg trees, where there're no sources
_ALL_MULTIPLE_OBJS_RENAMED_SRCS_C=$(foreach obj,$(_ALL_MULTIPLE_OBJS),\
    $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(basename $(obj))).c),\
      $(obj:%.o=%.c),\
    )\
)
_ALL_MULTIPLE_OBJS_RENAMED_SRCS_CPP=$(foreach obj,$(_ALL_MULTIPLE_OBJS),\
    $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(basename $(obj))).cpp),\
      $(obj:%.o=%.cpp),\
    )\
)
_ALL_MULTIPLE_OBJS_RENAMED_SRCS=$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS_C) \
  $(_ALL_MULTIPLE_OBJS_RENAMED_SRCS_CPP)

#_ALL_MULTIPLE_OBJS_RENAMED_SRCS=$(foreach obj,$(_ALL_MULTIPLE_OBJS),\
    $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(basename $(obj))).c),\
      $(obj:%.o=%.c),\
      $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(basename $(obj))).cpp),\
        $(obj:%.o=%.cpp),\
        $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(basename $(obj))).s),\
          $(obj:%.o=%.s),\
	)\
      )\
    )\
)

ifdef CONFIG_RG_JPKG_SRC

# We want to export the original src files in the src packages, and not the 
# renamed ones.

# extract the orig source file names from the renamed objects
_ALL_MULTIPLE_OBJS_ORIG_SRCS=$(foreach obj,$(_ALL_MULTIPLE_OBJS),\
    $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(obj:%.o=%)).c),\
      $(basename $(obj:%.o=%)).c,\
      $(if $(wildcard $(JMKE_PWD_SRC)/$(basename $(obj:%.o=%)).cpp),\
        $(basename $(obj:%.o=%)).cpp,\
        $(error Internal error: missing source file for $(obj).)\
      )\
    )\
)

JMK_JPKG_EXPORTED_FILES+=$(_ALL_MULTIPLE_OBJS_ORIG_SRCS)
JMK_DONT_EXPORT+=$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS)

else # CONFIG_RG_JPKG_SRC

JMK_ARCHCONFIG_LAST_TASKS+=$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS)
endif

ifdef JMKE_IS_BUILDDIR

$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS_C): %.c:
	$(JMKE_LN_NOFAIL) $(JMKE_PWD_SRC)/$(basename $*).c 
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
	@for config in $(JMK_COMPILE_MULTIPLE_CONFIGS) ; do \
	    echo "#undef $$config" >> $@ ; \
	done
	@echo "#define CONFIG_$(subst .,,$(suffix $*))" >> $@
	@echo "#include \"$(basename $*).c\"" >> $@

$(_ALL_MULTIPLE_OBJS_RENAMED_SRCS_CPP): %.cpp:
	$(JMKE_LN_NOFAIL) $(JMKE_PWD_SRC)/$(basename $*).cpp
	@echo "#ifdef __KERNEL__" > $@
	@echo "#ifdef CONFIG_RG_LINUX_24" >> $@
	@echo "#include <linux/config.h>" >> $@
	@echo "#endif" >> $@
	@echo "#else" >> $@
	@echo "#include <rg_config.h>" >> $@
	@echo "#define _LINUX_CONFIG_H /* prevent use of linux/config.h */">> $@
	@echo "#endif" >> $@
	@for config in $(JMK_COMPILE_MULTIPLE_CONFIGS) ; do \
	    echo "#undef $$config" >> $@ ; \
	done
	@echo "#define CONFIG_$(subst .,,$(suffix $*))" >> $@
	@echo "#include \"$(basename $*).cpp\"" >> $@

endif # JMKE_IS_BUILDDIR

endif # ifeq (IS_ANY_MULTIPLE_CONFIG_SET,)

endif # JMK_COMPILE_MULTIPLE_CONFIGS
