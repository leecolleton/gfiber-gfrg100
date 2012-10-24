# names of variables to save on dir change: (JMK_*)

# e.g. JMK_SO_TARGET=x in pkg/y is saved as 
# sav_JMK_SO_TARGET += $(JMKE_BUILDDIR)/pkg/y/x
target_vars=JMK_SO_TARGET JMK_OSAP_SO_TARGET JMK_LOCAL_TARGET JMK_TARGET \
  JMK_LOCAL_O_TARGET JMK_O_TARGET JMK_LOCAL_A_TARGET JMK_A_TARGET \
  JMK_MOD_TARGET JMK_MOD_O_TARGET JMK_LOCAL_CXX_TARGET JMK_OTHER_TARGETS \
  JMK_OTHER_OBJS JMK_OSAP_MODULE_TARGET

# variables for the targets of the current directory's makefile
# e.g. in pkg/z we have JMK_TARGET=x y ; JMK_LDFLAGS=--static is saved as:
# sav_JMK_LDFLAGS_pkg!z!x += --static
# sav_JMK_LDFLAGS_pkg!z!y += --static
dir_target_vars=JMK_LDFLAGS JMK_SO_CFLAGS JMK_SO_LDFLAGS
# add PWD_BUILD as prefix to sav_JMK_*_<target>
# e.g. in pkg/z we have JMK_TARGET=x y ;
# JMK_L_OBJS=a.o ../b.o $(JMKE_BUILDDIR)/pkg/c/c.o is saved as
# sav_JMK_L_OBJS_pkg!z!x += $(JMKE_BUILDDIR)/pkg/z/a.o $(JMKE_BUILDDIR)/pkg/b.o 
#   $(JMKE_BUILDDIR)/pkg/c/c.o
# sav_JMK_L_OBJS_pkg!z!y += $(JMKE_BUILDDIR)/pkg/z/a.o $(JMKE_BUILDDIR)/pkg/b.o 
#   $(JMKE_BUILDDIR)/pkg/c/c.o
dir_target_objs=JMK_O_OBJS JMK_OX_OBJS JMK_L_OBJS JMK_sO_OBJS \
  JMK_MOD_24_O_OBJS JMK_MOD_26_O_OBJS JMK_LIBS JMK_LOCAL_LIBS \
  JMK_OSAP_MODULE_COMPONENTS
# dir_target vars that don't accumulate but rather should be set only for
# targets where there is no specific setting
# e.g. in pkg/z we have JMK_TARGET=x y ; JMK_LOG_ENTITY=a ; JMK_LOG_ENTITY_x=b
# is saved as
# sav_JMK_LOG_ENTITY_pkg!z!x = b
# sav_JMK_LOG_ENTITY_pkg!z!y = a
dir_target_defaults=JMK_LOG_ENTITY JMK_SEC_PERM JMK_OSAP_MODULE_SCRIPT

# per dir variables
# e.g. JMK_EXPORT_HEADERS_DIR of pkg/main is saved as:
# sav_JMK_EXPORT_HEADERS_DIR_pkg!main! = main
dir_flag_vars=JMK_CFLAGS JMK_LOCAL_CFLAGS JMK_MOD_CFLAGS \
  JMK_RG_ULIBC_CFLAGS JMK_RG_GLIBC_CFLAGS JMK_RG_LINUX_CFLAGS \
  JMK_LOCAL_LDFLAGS JMK_sOFLAGS JMK_CXXFLAGS JMK_YFLAGS JMK_LEXFLAGS
dir_export_vars=JMK_EXPORT_HEADERS_DIR JMK_EXPORT_HEADERS_FLAT \
  JMK_EXPORT_HEADERS
dir_vars=$(dir_flag_vars) $(dir_export_vars) \
  JMK_JPKG_TARGETS_ALL JMK_JPKG_BIN_TARGETS_ALL JMK_COMPILE_MULTIPLE_CONFIGS

# global variables
# e.g. JMK_ARCHCONFIG_FIRST_TASKS=x is saved as
# sav_JMK_ARCHCONFIG_FIRST_TASKS += x
global_vars=JMK_ARCHCONFIG_FIRST_TASKS JMK_ARCHCONFIG_JPKG_FIRST_TASKS \
  JMK_ARCHCONFIG_LAST_TASKS JMK_ARCHCONFIG_JPKG_LAST_TASKS \
  JMK_JPKG_FIRST_TASKS JMK_JPKG_LAST_TASKS JMK_FIRST_CLEAN JMK_DIST_LOG_ENTITY \
  JMK_DIST_LOG_ENTITY_EXCLUDE JMK_RAMDISK_TASKS JMK_RAMDISK_NO_STRIP \
  JMK_DLM_TARGET

# e.g. JMK_JS_FILES=j.js in pkg/z is saved as
# sav_JMK_JS_FILES += $(JMK_ROOT)/pkg/z/j.js
global_src_objs=JMK_JS_FILES JMK_CSS_FILES JMK_SQL_FILES \
  JMK_INTERNAL_HEADERS JMK_CD_EXPORTED_FILES cp_JMK_EXPORT_HEADERS \
  JMK_DONT_EXPORT JMK_JPKG_EXPORTED_FILES JMK_JPKG_EXPORTED_IF_EXIST \
  JMK_JPKG_EXPORTED_DIR JMK_LINK_DIRS JMK_EXPORT_AS_SRC

global_ramdisk_objs=JMK_RAMDISK_KERNEL_MODULES JMK_RAMDISK_FILES \
  JMK_RAMDISK_BIN_FILES JMK_RAMDISK_ETC_FILES JMK_RAMDISK_LIB_FILES \
  JMK_RAMDISK_VAR_FILES JMK_RAMDISK_MODULES_FILES JMK_RAMDISK_SWF_FILES \
  JMK_RAMDISK_MODULES_PERMANENT_FILES JMK_RAMDISK_IMG_FILES \
  JMK_RAMDISK_JS_FILES JMK_MODULES_DONT_STRIP JMK_RAMDISK_RW_FILES \
  JMK_RAMDISK_DIRS JMK_RAMDISK_CGI_FILES JMK_RAMDISK_PLUGINS_FILES

# e.g. JMK_CLEAN=x.o in pkg/z is saved as
# sav_JMK_CLEAN += $(JMKE_BUILDDIR)/pkg/z/x.o
global_objs=$(global_ramdisk_objs) JMK_MOD_2_STAT JMK_CLEAN JMK_AUTOGEN_SRC \
  JMK_RUN_UNITTEST JMK_RUN_UNITTEST_DATA JMK_CREATE_LOCAL JMK_EXPORT_LIBS \
  JMK_A_TARGET_PIC

# per target objs: contents are expected to be file names
# e.g. in pkg/z we have JMK_TARGET=y ; JMK_O_OBJS_y=a.o saved as
# sav_JMK_O_OBJS_pkg!z!y += $(JMKE_BUILDDIR)/pkg/z/a.o
per_target_obj_prefixes=JMK_O_OBJS_ JMK_OX_OBJS_ JMK_L_OBJS_ \
  JMK_MOD_24_O_OBJS_ JMK_MOD_26_O_OBJS_ JMK_LIBS_ JMK_OSAP_MODULE_COMPONENTS_ \
  JMK_OSAP_MODULE_SCRIPT_

# per target and per object:
# JMK_LOG_ENTITY_
# JMK_SEC_PERM_ 
# JMK_CFLAGS_REMOVE_
# JMK_CFLAGS_
# JMK_JPKG_TARGET_ 
# in the current implementation it's enough to save them only per object.

# per target vars: contents are expected to be non-files
# e.g. in pkg/z we have JMK_TARGET=y JMK_LDFLAGS_y=--static is saved as
# sav_JMK_LDFLAGS_pkg!z!y += --static
per_target_var_prefixes=JMK_JPKG_TARGET_BIN_ JMK_SO_CFLAGS_ JMK_LDFLAGS_ \
  JMK_LDFLAGS_REMOVE_ JMK_SO_LDFLAGS_ JMK_FLAT_STACK_SIZE_

# Note that if we have two vars here such that one's name is a prefix to the
# other's (e.g. JMK_CFLAGS_ & JMK_CFLAGS_REMOVE_) - then the longer prefix must
# come first. see Save_Per_Obj_Vars
# e.g. in pkg/z JMK_CFLAGS_x.o = -DXXX is saved as
# sav_JMK_CFLAGS_pkg!z!x.o += -DXXX
per_obj_var_prefixes=JMK_CFLAGS_REMOVE_ JMK_CFLAGS_ JMK_LOG_ENTITY_ \
  JMK_SEC_PERM_ JMK_JPKG_BIN_LIBCS_ JMK_MOD_2_STAT_PREFIX_ JMK_JPKG_TARGET_

# e.g. JMK_DLM_ETC_FILES_x = y in pkg/libdisko is saved as
# sav_JMK_DLM_ETC_FILES_x += $(JMKE_BUILDDIR)/pkg/libdisko/y
global_obj_prefixes=JMK_DLM_FILES_ JMK_DLM_BIN_FILES_ JMK_DLM_ETC_FILES_ \
  JMK_DLM_LIB_FILES_ JMK_DLM_VAR_FILES_ JMK_DLM_MODULES_FILES_ \
  JMK_DLM_SWF_FILES_ JMK_DLM_JS_FILES_ JMK_DLM_CGI_FILES_ JMK_DLM_IMG_FILES_ 

# variables saved only internally in subdirs_root.mak
# all subdirs that we pass through are saved in sav_SUBDIRS elsewhere
depth_vars=JMK_SUBDIRS CUR_DIR

# no need to save - only clean
dont_save_vars=JMK_WARN2ERR JMK_LINUX_CFLAGS JMK_LINKS JMK_O_OBJS_MULTIPLE \
  JMK_OX_OBJS_MULTIPLE JMK_O_OBJS_MULTIPLE_ JMK_OX_OBJS_MULTIPLE_ \
  JMK_COMPILE_MULTIPLE_CONFIGS_SKIP JMK_RGTV_MAKEFLAGS \
  JMK_JPKG_EXPORTED_DIR_SRC JMK_CLEAN_SUBDIRS JMK_DOCS_SUBDIRS \
  JMK_ARCHCONFIG_SUBDIRS JMK_RUN_UNITTESTS_SUBDIRS JMK_NEED_VPATH \
  JMK_TEST_TARGETS JMK_SUBDIRS_m JMK_SUBDIRS_y JMK_LEGACY_SUBDIRS \
  RAMDISK_2_DLM obj-y obj-m

# global vars/prefixes that are not saved or cleaned when moving through the
# dirs
# e.g. JMK_DLM_VERSION_x=5 is used in jmk/* as JMK_DLM_VERSION_x
# do_nothing_vars = JMK_DLM_VERSION_ JMK_DLM_DESC_

# path handling functions

Non_Empty_Dir=$(if $1,$1,./)

# $1 - absolute path
# return - path relative to root (remove JMK_ROOT or JMKE_BUILDDIR)
_Abs_Path_To_Rel=$(patsubst $(JMK_ROOT)/%,%,$(patsubst $(JMKE_BUILDDIR)/%,%,$1))
Abs_Path_To_Rel=$(call Non_Empty_Dir,$(call _Abs_Path_To_Rel,$1))

# convert relative or absolute path to absolute path
# $1 - list of paths
# $2 - prefix
Rel_Path_To_Abs=$(abspath $(foreach o,$1,$(if $(filter /%,$o),$o,$2$o)))

# convert path to var-name - slashes are not allowed - use exclamations instead
# $1 - path
Path_To_Var=$(subst /,!,$1)

# convert sav_JMK_EXPORT_HEADERS_DIR_pkg!main! to pkg/main/
# $1 - var name (sav_JMK_EXPORT_HEADERS_DIR_pkg!main!)
# $2 - var name prefix (sav_JMK_EXPORT_HEADERS_DIR_)
Var_To_Path=$(subst !,/,$(1:$2%=%))

Abs_Path_To_Var=$(call Path_To_Var,$(call Abs_Path_To_Rel,$1))
Abs_Path_To_Dir_Var=$(call Abs_Path_To_Var,$(dir $1))

# current working directory relative to JMK_ROOT
Cwd_From_Root=$(call Abs_Path_To_Rel,$(CUR_DIR)/)
cwd_from_root:=$(Cwd_From_Root)
cwd_from_root_var=$(call Path_To_Var,$(cwd_from_root))

# save variables

# accumulate local variable into a saving variable
# - init accumulating var as simple if it doesn't exist yet
#   this causes later use of += to have immediate evaluation.
# - add saved var's contents into the saving var
# for regular Accumulate_Var
# - empty saved var so its current value won't appear in next directory
# Accumulate_Var_Dont_Del is used when we want to use the var's contents again
# (caller is responsible for cleaning)
# $1 - the accumulating var name
# $2 - the source var name
# $3 - prefix for each value (assuming that a prefix can only be an absolute
#      path)
Accumulate_Var_Dont_Del= \
  $(if $($2), \
    $(if $($1),,$(eval $1:=)) \
    $(if $3, \
      $(eval $1+=$(call Rel_Path_To_Abs,$($2),$3)), \
      $(eval $1+=$($2))))
Accumulate_Var= \
  $(call Accumulate_Var_Dont_Del,$1,$2,$3) \
  $(eval $2=)

Save_Dir_Vars= \
  $(foreach v, $(dir_vars), \
    $(if $($v), \
      $(eval sav_$v_$(cwd_from_root_var):=$($v)) \
      $(eval $v=)))

Save_Global_Vars= \
  $(foreach v, $(global_src_objs), \
    $(call Accumulate_Var,sav_$v,$v,$(JMK_ROOT)/$(cwd_from_root))) \
  $(foreach v, $(global_objs), \
    $(call Accumulate_Var,sav_$v,$v,$(JMKE_BUILDDIR)/$(cwd_from_root))) \
  $(foreach v, $(global_vars), \
    $(call Accumulate_Var,sav_$v,$v,))

Save_Target_Vars= \
  $(foreach v, $(target_vars), \
    $(call Accumulate_Var,sav_$v,$v,$(JMKE_BUILDDIR)/$(cwd_from_root)))

# Save_Per_Target_* must be called before Save_Target_Vars
# works only on locally defined targets
Save_Per_Target_Vars= \
  $(foreach v, $(target_vars), \
    $(foreach t, $($v), \
      $(foreach p, $(per_target_var_prefixes), \
        $(call Accumulate_Var,sav_$p$(cwd_from_root_var)$t,$p$t,)) \
      $(foreach p, $(per_target_obj_prefixes), \
        $(call Accumulate_Var,sav_$p$(cwd_from_root_var)$t,$p$t, \
	  $(JMKE_BUILDDIR)/$(cwd_from_root)))))

# $1 - target file name
# $2 - target in var format
_Save_Dir_Target_Vars = \
  $(foreach p, $(dir_target_vars), \
    $(call Accumulate_Var_Dont_Del,sav_$p_$2,$p,)) \
  $(foreach p, $(dir_target_defaults), \
    $(if $($p_$1)$(sav_$p_$2),, \
      $(call Accumulate_Var_Dont_Del,sav_$p_$2,$p,))) \
  $(foreach p, $(dir_target_objs), \
    $(call Accumulate_Var_Dont_Del,sav_$p_$2,$p, \
      $(JMKE_BUILDDIR)/$(cwd_from_root)))

# here the same variable is saved for several targets. so we empty the vars'
# contents only in the end.
Save_Dir_Target_Vars= \
  $(foreach v, $(target_vars), \
    $(foreach t, $($v), \
      $(call _Save_Dir_Target_Vars,$(notdir $t),$(strip \
	$(if $(filter /%,$t), \
	  $(call Abs_Path_To_Var,$t), \
	  $(cwd_from_root_var)$t))))) \
  $(foreach p, $(dir_target_vars) $(dir_target_defaults) $(dir_target_objs), \
    $(eval $p=))

# The trivial way to implement this is go on all objs of all targets and 
# save it.
# In this implementation we are searching in all variable list for our prefix.
# This make the code simpler and doing the per-objs and per-target work for that
# variables at the same time, but it also add the need for variables that are a
# prefix of another variables (JMK_CFLAGS_REMOVE_ JMK_CFLAGS_), the longer
# prefix must come first.
Save_Per_Obj_Vars= \
  $(foreach p, $(per_obj_var_prefixes), \
    $(foreach v, $(filter $p%,$(.VARIABLES)), \
      $(call Accumulate_Var,sav_$p$(cwd_from_root_var)$(v:$p%=%),$v,)))

Save_Global_Obj_Prefixes= \
    $(foreach p, $(global_obj_prefixes), \
      $(foreach v, $(filter $p%,$(.VARIABLES)), \
	$(call Accumulate_Var,sav_$v,$v,$(JMKE_BUILDDIR)/$(cwd_from_root))))

Save_Depth_Vars= \
  $(eval depth:=$(depth)x) \
  $(foreach v, $(depth_vars), \
    $(eval sav_$v_$(depth):=$($v)) \
    $(eval $v=))

# go back up one level
Cd_Up= \
  $(eval depth:=$(depth:%x=%)) \
  $(eval CUR_DIR:=$(sav_CUR_DIR_$(depth)))

Clean_Dont_Save_Vars = \
  $(foreach v,$(dont_save_vars), \
    $(eval $v=))

# Order is important
# For example: Save_Per_Target_Vars assumes that target_vars were not saved yet
# (and therefore not deleted yet)
Save_Vars= \
  $(call Save_Per_Obj_Vars) \
  $(call Save_Global_Vars) \
  $(call Save_Dir_Target_Vars) \
  $(call Save_Per_Target_Vars) \
  $(call Save_Target_Vars) \
  $(call Save_Global_Obj_Prefixes) \
  $(call Save_Dir_Vars) \
  $(call Save_Depth_Vars) \
  $(call Clean_Dont_Save_Vars)

Do_Subdirs= \
  $(if ${debug_subdirs},$(if $(sav_JMK_SUBDIRS_$(depth)),$(warning \
    JMK: dirs of $(sav_CUR_DIR_$(depth)): [$(sav_JMK_SUBDIRS_$(depth))]))) \
  $(foreach d, $(sav_JMK_SUBDIRS_$(depth)), $(call Do_Subdir,$d))

# $1 - dir to handle
Do_Subdir= \
  $(eval cwd_from_root:=$(call Abs_Path_To_Rel,$(sav_CUR_DIR_$(depth))/$1/)) \
  $(if $(wildcard $(sav_CUR_DIR_$(depth))/$1/Makefile), \
    $(eval sav_SUBDIRS+=$(sav_CUR_DIR_$(depth))/$1) \
    $(if $(filter $(legacy_subdirs_filter),$(cwd_from_root)), \
      $(if ${debug_subdirs},$(warning JMK: legacy dir: [$(cwd_from_root)])), \
      $(if ${debug_subdirs},$(warning JMK: dir: [$(cwd_from_root)])) \
      $(eval CUR_DIR:=$(sav_CUR_DIR_$(depth))/$1) \
      $(eval include $(JMK_ROOT)/jmk/env_subdir.mak) \
      $(eval include $(JMK_ROOT)/$(cwd_from_root)Makefile) \
      $(eval include $(JMK_ROOT)/jmk/rg_subdir.mak) \
      $(call Save_Vars) \
      $(call Do_Subdirs) \
      $(call Cd_Up)), \
    $(if $(CONFIG_RG_GPL), \
      $(if ${debug_subdirs},$(warning JMK: skipping [$(cwd_from_root)])), \
      $(error missing Makefile $(JMK_ROOT)/$(cwd_from_root)Makefile)))

# main() handle subdirs of current directory

$(call Save_Vars)
$(call Do_Subdirs)

# When there are no subdirs for the root subdir we are running make from, we
# never execute Cd_Up() so CUR_DIR is empty. Fix it.
$(if $(CUR_DIR),,$(eval CUR_DIR:=$(sav_CUR_DIR_x)))

# JMKE_PWD_* update on every subdir. now update them back to the flat-make root
include $(JMK_ROOT)/jmk/build_dir_env_subdir.mak
