# This file creates all the dependencies between variable contents

# $1 - target in var format
# $2 - boolean - include OX_OBJS?
_DEP_OBJS=$(_O_OBJS_$1) $(if $2,$(_OX_OBJS_$1)) \
  $(if $(sav_JMK_JPKG_TARGET_$1),$(_L_OBJS_$1))
# $1 - target
# $2 - include OX_OBJS? (y/empty)
DEP_OBJS=$(call _DEP_OBJS,$(call Abs_Path_To_Var,$1),$2)

# Create the $(JMK_TARGET) logic
ifdef BINFRMT_FILE
  include $(JMK_ROOT)/jmk/$(BINFRMT_FILE)
endif

$(foreach t,$(filter-out $(sav_JMK_A_TARGET),$(NORMAL_TARGETS)), \
  $(eval $(t:.so=_pic.a): $(call DEP_OBJS,$t,y)))

# $1 - library path
# if the given library with .so instead of .a exists in sav_JMK_SO_TARGET or
# exists as a file - use the .so instead.
# the need for checking existance of the file is for make in internal
# directories: e.g. pkg/main/openrg uses libraries from pkg/util, when we run
# make inside pkg/main, pkg/util/Makefile was not loaded and therefore the
# libraries are not in sav_JMK_SO_TARGET, but we still want this mechanism to
# work.
Try_A_To_SO= \
  $(or $(so_target_$(call Abs_Path_To_Var,$1)), \
    $(wildcard $(1:%.a=%.so)), \
    $1)

# $1 - list to filter-out from %__local_%
# output - filtered list
Filter_Out__local_ = $(strip \
  $(foreach t,$1, \
    $(if $(filter-out __local_%,$(notdir $t)),$t)))

# $1 - target
# $2 - libs list variable name
# $3 - if not empty - try to convert .a to .so
Libs_Dep = \
  ${if ${debug_libs_a_to_so},${warning JMK: A TO SO: before: $2=[${$2}]}} \
  $(if $($2), \
    $(if $3, $(eval $2 := $(foreach l,$($2),$(call Try_A_To_SO,$l)))) \
    $(eval $1: $(call Filter_Out__local_, $($2)))) \
  ${if ${debug_libs_a_to_so},${warning JMK: A TO SO: after: $2=[${$2}]}}

$(foreach t,$(NORMAL_TARGETS), \
  $(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$t)), \
    $(call Libs_Dep,$t,sav_JMK_LIBS_$(call Abs_Path_To_Var,$t),y)))

$(foreach t,$(LOCAL_TARGETS), \
  $(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$t)), \
    $(call Libs_Dep,$t,sav_JMK_LIBS_$(call Abs_Path_To_Var,$t)) \
    $(call Libs_Dep,$t,sav_JMK_LOCAL_LIBS_$(call Abs_Path_To_Var,$t))))

$(foreach t,$(sav_JMK_SO_TARGET), \
  $(eval $t: $(dir $t)$(patsubst lib%.so,lib%_pic.a,$(notdir $t))))

_GET_SRC=$(firstword $(wildcard $1.S $2.S $1.cpp $2.cpp $1.cc $2.cc $1.c $2.c))
# $1 - base file name (no extension) with full build path
GET_SRC=$(call _GET_SRC,$1,$(call BUILD2SRC,$1))

$(foreach o, $(mod_objs), \
  $(eval $o: \
    $(call GET_SRC, $(patsubst %_mod_24.o,%.o,$(patsubst %_mod_26.o,%.o,$o)))))

$(foreach t, \
  $(A_TARGETS) $(sav_JMK_LOCAL_TARGET) $(sav_JMK_LOCAL_O_TARGET) \
    $(sav_JMK_LOCAL_CXX_TARGET), \
  $(eval $t: $(call DEP_OBJS,$t,)))

# Adding the special objects for JMK_O_TARGET.
# Warning: JMK_sO_OBJS aren't for multiple targets!
$(foreach t,$(sav_JMK_O_TARGET), \
  $(eval $t: $(sav_JMK_sO_OBJS_$(call Abs_Path_To_Var,$t))))

# we want to precompile only if the %.s file is in the JMK_sO_OBJS variable.
$(foreach v, $(filter sav_JMK_sO_OBJS_%,$(.VARIABLES)), \
  $(foreach o, $($v), \
    $(eval $o: $(call BUILD2SRC,$(o:%.o=%.s)))))

# All compiled objs depend on the makefile that creates them
$(foreach o,$(ALL_OBJS),\
  $(eval $o: $(call BUILD2SRC,$(dir $o)/Makefile)))

ifdef CONFIG_RG_JPKG_SRC
  ALL_DEPS+=$(sav_JMK_LOCAL_A_TARGET) $(sav_JMK_LOCAL_O_TARGET) \
    $(sav_JMK_LOCAL_TARGET) $(ALL_OBJS)
else
  ifdef CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY
    ALL_DEPS+=$(ALL_LOCAL_OBJS) $(LOCAL_TARGETS)
  else
    ALL_DEPS+=$(ALL_TARGETS) $(MOD_2_STAT_LINKS)
  endif
endif

# $1 - targets to add
# $2 - phony target to mark all targets in $1 are done
# if all you need is a phony target - keep $1 empty
Add_Dep = \
  $(if $1, \
    $(eval $1: $(ALL_DEPS)) \
    $(eval $2: $1) \
    , \
    $(eval $2: $(ALL_DEPS))) \
  $(eval ALL_DEPS+=$2) \
  $(eval internal_phony_targets+=$2)

ifdef CONFIG_RG_JPKG
  $(call Add_Dep,$(sav_JMK_JPKG_FIRST_TASKS),jpkg_first_tasks)
  $(call Add_Dep,,export_to_jpkg)
  $(call Add_Dep,$(sav_JMK_JPKG_LAST_TASKS),jpkg_last_tasks)
endif

ifdef CONFIG_RG_JSLINT
  ifndef CONFIG_RG_JPKG_SRC
    ifneq ($(sav_JMK_JS_FILES),)
      $(JMKE_PWD_BUILD)/.jslint: $(sav_JMK_JS_FILES)
      ALL_DEPS+=$(JMKE_PWD_BUILD)/.jslint
    endif
  endif
endif

ifeq ($(IN_RGSRC_ROOT),y)
  ifdef CONFIG_RG_OS
    # XXX: Ramdisk logic should not be protected with CONFIG_RG_OS,
    # More than that we need to fix the ramdisk logic and not delete it
    # each make.
    internal_phony_targets+=clean_ramdisk_dir
    ALL_DEPS+=clean_ramdisk_dir
    ifdef CONFIG_RG_DLM
      internal_phony_targets+=clean_dlm_dir
      ALL_DEPS+=clean_dlm_dir
    endif
    $(call Add_Dep,,legacy_make_os_modules)
  endif
endif

ifeq ($(CONFIG_RG_JPKG)$(CONFIG_RG_GPL),)
  $(call Add_Dep,make_os_ramdisk make_libc_ramdisk legacy_all $(ALL_RAMDISK_FILES),legacy_ramdisk)
else
  $(call Add_Dep,,legacy_all)
endif

ifdef IN_RGSRC_ROOT
  ifeq ($(CONFIG_RG_JPKG)$(CONFIG_RG_GPL),)
    $(call Add_Dep,$(ALL_RAMDISK_DIRS) $(ALL_RAMDISK_TASKS), \
      make_ramdisk_img)
  endif

  $(call Add_Dep,,$(OPENRG_IMG))

  ifdef CONFIG_RG_RMT_UPDATE
    ifeq ($(CONFIG_RG_JPKG)$(CONFIG_RG_GPL),)
      $(call Add_Dep,,$(OPENRG_RMT_UPDATE_IMG))
      $(call Add_Dep,,$(OPENRG_RMT_UPDATE_RSA_IMG))
      ifdef CONFIG_RG_DLM
        $(call Add_Dep,,$(OPENRG_RMT_UPDATE_DLM))
        $(call Add_Dep,,$(OPENRG_RMT_UPDATE_RSA_DLM))
      endif
    endif
  endif
endif

all: $(ALL_DEPS)

.PHONY: $(internal_phony_targets) all

# phony targets are added as a dependency here and in config.mak.
# in order to get the list of non-jmk dependencies users should use this:
JMKE_DEPS=$(filter-out $(internal_phony_targets) %.a %.so,$^)
JMKE_NEW_DEPS=$(filter-out $(internal_phony_targets) %.a %.so,$?)
JMKE_DEPS_WITH_LIBS=$(filter-out $(internal_phony_targets),$^)
JMKE_NEW_DEPS_WITH_LIBS=$(filter-out $(internal_phony_targets),$?)
