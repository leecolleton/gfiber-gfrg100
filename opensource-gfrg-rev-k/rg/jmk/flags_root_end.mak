dep_flags=$(if $(filter %.o,$@),-MMD $(if $(findstring 2.95,$(GCC_VERSION)),,-MP -MF $(@:%.o=%.d) -MT $@))

ifdef CONFIG_RG_OS_LINUX
  MODFLAGS+=-DMODULE
  ifdef CONFIG_RG_OS_LINUX_24
    GET_MOD_24_CFLAGS=-DKBUILD_BASENAME=$(basename $(notdir $1))
  endif
  ifdef CONFIG_RG_OS_LINUX_26
    # The target can be *.o or *_mod_26.o.
    GET_MOD_26_CFLAGS= -D"KBUILD_STR(s)=\#s" \
     -D"KBUILD_BASENAME=KBUILD_STR($(basename $(notdir $1)))" \
     -D"KBUILD_MODNAME=KBUILD_STR($(patsubst %_mod_26,%,$(basename $(notdir $1))))"
  endif
endif

ifdef CONFIG_RG_OS_LINUX
  GET_MODFLAGS=$(call DBGFR, \
    $(sav_JMK_RG_LINUX_CFLAGS_$(call Abs_Path_To_Dir_Var,$1)) \
    $(sav_JMK_MOD_CFLAGS_$(call Abs_Path_To_Dir_Var,$1)) \
    -I$(JMKE_BUILDDIR)/pkg/include $(dep_flags) \
    $(if $(is_mod2stat_obj_$(call Abs_Path_To_Var,$1)),,$(MODFLAGS)) \
    $(if $(filter %_mod_24.o,$1),$(call GET_MOD_24_CFLAGS,$1)) \
    $(if $(filter %_mod_26.o,$1),$(call GET_MOD_26_CFLAGS,$1)) \
    $(if $(is_ox_obj_$(call Abs_Path_To_Var,$1)),-DEXPORT_SYMTAB) \
  ,GET_MODFLAGS,$1)
else
  GET_MODFLAGS=-D__KERNEL__ $(sav_JMK_CFLAGS_$(call Abs_Path_To_Dir_Var,$1)) \
    $(sav_JMK_MOD_CFLAGS_$(call Abs_Path_To_Dir_Var,$1) $(dep_flags))
endif

_ULIBC_FLAG=--rg-use-ulibc
_GLIBC_FLAG=--rg-use-glibc

# $1 - existing flags
ifdef CONFIG_RG_JPKG_BIN
  _IS_COMPILING_ULIBC=$(filter $(_ULIBC_FLAG),$1)
else
  _IS_COMPILING_ULIBC=$(CONFIG_ULIBC)
endif

# $1 - existing flags
# $2 - obj name
# $3 - src
# - add the libc flag only if needed
# - JMK_RG_ULIBC_CFLAGS & JMK_RG_GLIBC_CFLAGS are already added in rg_gcc for c files, 
#   therefore they are added here only for cpp files.
_GET_CFLAGS_ULIBC=$(call DBGFR, \
  $(if $(call _IS_CPP_FILE,$3), \
    $(sav_JMK_RG_ULIBC_CFLAGS_$(call Abs_Path_To_Dir_Var,$2)), \
    $(if $(filter $(_ULIBC_FLAG),$1),,$(_ULIBC_FLAG)) \
  ) \
  $(sav_JMK_CFLAGS_$(call Abs_Path_To_Dir_Var,$2)) $(dep_flags) \
,_GET_CFLAGS_ULIBC,$1,$2,$3)

_GET_CFLAGS_GLIBC=$(call DBGFR, \
  $(if $(call _IS_CPP_FILE,$3), \
    $(sav_JMK_RG_GLIBC_CFLAGS_$(call Abs_Path_To_Dir_Var,$2)), \
    $(if $(filter $(_GLIBC_FLAG),$1),,$(_GLIBC_FLAG)) \
  ) \
  $(sav_JMK_CFLAGS_$(call Abs_Path_To_Dir_Var,$2)) $(dep_flags) \
,_GET_CFLAGS_GLIBC,$1,$2,$3)

# Create the TARGET_CFLAGS_% cflags. This variable is the JMK_CFLAGS_target of
# the target that %.o is connected to.
# Note that if the same object is used for more than one target the last 
# target JMK_CFLAGS will overrun the previes cflags.
$(foreach t,$(all_targets_var), \
  $(foreach o,$(target_objs_var_$t), \
    $(eval TARGET_CFLAGS_$o:=$(sav_JMK_CFLAGS_$t))))

# $1 - object name as fixed-name var-format
__Obj_CFlags = $(TARGET_CFLAGS_$1) $(sav_JMK_CFLAGS_$1)

# $1 - existing flags
# $2 - obj name
# $3 - src
# - locals don't pass through rg_gcc, so we fix the include path here.
_GET_OBJ_CFLAGS=$(call DBGFR, \
  $(if $(call _IS_CPP_FILE,$3), \
    $(sav_JMK_CXXFLAGS_$(call Abs_Path_To_Dir_Var,$3)) \
    $(filter-out $(_ULIBC_FLAG) $(_GLIBC_FLAG),$1), \
    $1) \
  $(if $(is_local_obj_$(call Abs_Path_To_Var,$2)), \
    $(sav_JMK_LOCAL_CFLAGS_$(call Abs_Path_To_Dir_Var,$2)) $(dep_flags) \
    -I$(RG_BUILD)/local/include, \
    $(if $(is_mod_obj_$(call Abs_Path_To_Var,$2)), \
      $(call GET_MODFLAGS,$2), \
      $(if $(call _IS_COMPILING_ULIBC,$1), \
        $(call _GET_CFLAGS_ULIBC,$1,$2,$3), \
        $(call _GET_CFLAGS_GLIBC,$1,$2,$3) \
      ) \
    ) \
  ) \
  -DENTITY_ID=$(call GET_ENTITY_ID,$2) \
  $(addprefix -DSEC_PERM_ID=,$(call GET_SEC_PERM_ID,$2)) \
  $(call __Obj_CFlags,$(fix_obj_name_$(call Abs_Path_To_Var,$2))) \
  ,_GET_OBJ_CFLAGS,$1,$2,$3)

# $1 - existing flags
# $2 - obj name
# $3 - src
# linux 2.6 includes are already fixed in config_host.c, don't fix them again
GET_OBJ_CFLAGS=$(call DBGFR, \
  $(filter-out $(sav_JMK_CFLAGS_REMOVE_$(call Abs_Path_To_Var,$2)), \
    $(if $(filter %_mod_24.o,$2), \
      $(filter-out $(RG_LINUX_26_CFLAGS),$(call _GET_OBJ_CFLAGS,$1,$2,$3)), \
      $(if $(filter %_mod_26.o,$2), \
	$(filter-out $(RG_LINUX_24_CFLAGS),$(call _GET_OBJ_CFLAGS,$1,$2,$3)), \
	$(if $(filter %.cpp,$3), \
	  $(filter-out -Wstrict-prototypes,$(call _GET_OBJ_CFLAGS,$1,$2,$3)), \
	  $(call _GET_OBJ_CFLAGS,$1,$2,$3))))), \
  ,GET_OBJ_CFLAGS,$1,$2,$3)

# PIC files
FPIC_FLAG=-fpic

