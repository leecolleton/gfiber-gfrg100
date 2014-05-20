# legacy subdirs we should visit
cur_legacy_subdirs := \
  $(filter $(legacy_subdirs_filter),$(addsuffix /,$(call Abs_Path_To_Rel, \
    $(sav_SUBDIRS))))

cur_legacy_subdirs_targets := \
  $(addprefix $(JMKE_BUILDDIR)/,$(addsuffix __subdir, $(cur_legacy_subdirs)))

# convert directory name to legacy directory target name
# $1 - path
Legacy_Dir_Target = $(call Rel_Path_To_Abs,$1,$(JMKE_BUILDDIR)/)/__subdir

# $1 - object / library path
# return true (non-empty string) if the object is under a legacy subdir under
# the current directory
Is_Cur_Legacy_Obj = \
  $(filter $(addsuffix %,$(cur_legacy_subdirs)), \
    $(call Abs_Path_To_Rel,$(dir $1)))

legacy_objs := $(sort \
  $(foreach t,$(all_targets_var), \
    $(foreach o,$(_L_OBJS_$t) $(sav_JMK_LIBS_$t) $(sav_JMK_LOCAL_LIBS_$t), \
      $(if $(call Is_Cur_Legacy_Obj,$o),$o))))

# returns the legacy subdir needed for the subdir $1
# e.g. in order to make a needed L_OBJ under $(JMKE_BUILDDIR)/pkg/bridge/main
# we need to make-C into $(JMKE_BUILDDIR)/pkg/bridge
Subdir_Target_Dir=$(strip \
  $(foreach d,$(cur_legacy_subdirs),$(findstring $(JMKE_BUILDDIR)/$d,$1)))
# dependency rule - each object / library depends on its subdir's phony target
# note that adding phony target to the dependency means we will always try to
# compile this object and therefore always link the using target. but this is
# good because this is our best way to get the object's library's dependencies.
# Note: The ';' causing this to be an empty rule and not only a dependency.
# This is important so make will find an explicit rule and not try to do
# something implicit.
$(foreach o, $(legacy_objs), \
  $(eval $o: $(call Subdir_Target_Dir,$(dir $o))__subdir ; ))

# $1 - path
Legacy_Make_Subdir = \
  $(MAKE) -f $(call BUILD2SRC,$1)/Makefile -C $1 JMK_ROOT=$(JMK_ROOT)

$(cur_legacy_subdirs_targets): prelegacy_targets
	$(call Legacy_Make_Subdir,$(@:%__subdir=%))

# Note: All legacy targets depend on _all_ prelegacy_dirs targets in order to
# avoid a lot of complicated dependancies between legacy subdirs and each of
# the prelegacy_dirs libs
prelegacy_dirs = pkg/util pkg/zlib pkg/openssl pkg/license
prelegacy_targets: \
  $(filter $(addprefix $(JMKE_BUILDDIR)/,$(addsuffix %,$(prelegacy_dirs))), \
    $(if $(or $(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY),$(CONFIG_RG_JPKG_SRC)), \
      $(LOCAL_TARGETS), \
      $(ALL_TARGETS)))

# legacy dir dependencies
# $1 - dependent
# $2 - prerequisite
# need to check that $2/ is really a used subdir since it may be
# JMK_JPKG_EXPORTED_DIR
Dir_Dep = \
  $(if $(filter $2/,$(cur_legacy_subdirs)), \
    $(eval $(JMKE_BUILDDIR)/$1/__subdir: $(JMKE_BUILDDIR)/$2/__subdir))

# Dir_Dep_File is in legacy_env.mak

include $(JMK_ROOT)/jmk/legacy_dependencies.mak

# this allows us to avoid double running of make-C to legacy subdirs and allows
# dependencies between the legacy subdirs
legacy_all: $(cur_legacy_subdirs_targets)

legacy_clean legacy_run_unittests legacy_xmldocs legacy_ramdisk:
	$(foreach d,$(cur_legacy_subdirs), \
	  $(MAKE) -f $(JMK_ROOT)/$d/Makefile -C $(JMKE_BUILDDIR)/$d \
	    JMK_ROOT=$(JMK_ROOT) $(@:legacy_%=%) &&) true

# for position in running see - config_root_end.mak
legacy_archconfig:
	$(foreach d,$(cur_legacy_subdirs), \
	  $(MAKE) -C $(JMK_ROOT)/$d $(@:legacy_%=%) &&) true

clean: legacy_clean
run_unittests: legacy_run_unittests
xmldocs: legacy_xmldocs

# TODO: Will be nice to have os as a normal subdir and without special care 
# in the engine for os.
ifeq ($(IN_RGSRC_ROOT)-$(CONFIG_RG_OS_LINUX)-$(JMKE_DOING_MAKE_CONFIG),y-y-)
  ifndef OPENRG_IMG
    OPENRG_IMG=make_os
  endif
  $(OPENRG_IMG):
	$(MAKE) -C os
  ifneq ($(CONFIG_RG_GPL),)
	@echo "GFIBER GPL build has been completed."
  endif

  ifeq ($(CONFIG_RG_GPL)$(CONFIG_RG_JPKG_SRC),)
    legacy_make_os_modules:
	$(MAKE) -C os modules
  endif
  
  ifeq ($(CONFIG_RG_GPL)$(CONFIG_RG_JPKG),)
    make_os_ramdisk:	
	$(MAKE) -C os ramdisk
    make_libc_ramdisk:
	$(foreach d,$(JMKE_PKG_LIBC), \
	  $(MAKE) -C $d ramdisk ; )
  endif
endif

internal_phony_targets+=legacy_archconfig legacy_clean legacy_run_unittests \
  legacy_xmldocs legacy_all prelegacy_targets $(cur_legacy_subdirs_targets) \
  legacy_ramdisk make_os_ramdisk make_libc_ramdisk
