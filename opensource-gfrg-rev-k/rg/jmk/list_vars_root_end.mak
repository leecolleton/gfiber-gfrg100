## pre-process

sav_JMK_MOD_TARGET += $(sav_JMK_MOD_O_TARGET)

# $1 - prefix to the name of the created var (sav_JMK_O_OBJS / sav_JMK_L_OBJS)
Define_Create_Local_Var = \
  $(foreach t,$(sav_JMK_CREATE_LOCAL), \
    $(eval $1_$(call Abs_Path_To_Var,$(call ADD_LOCAL_PREFIX,$t)) += $(strip \
      $($1_$(call Abs_Path_To_Var,$t)))))

$(call Define_Create_Local_Var,sav_JMK_O_OBJS)
$(call Define_Create_Local_Var,sav_JMK_L_OBJS)

## initialize immediate accumulating variables
# Note: It is NOT redundant. This cause that any later += to act like :=
pic_objs :=
ox_objs :=
mod_objs :=

## targets

NORMAL_TARGETS := \
  $(sort $(sav_JMK_TARGET) $(sav_JMK_O_TARGET) $(sav_JMK_SO_TARGET) \
    $(sav_JMK_A_TARGET) $(sav_JMK_MOD_TARGET))
LOCAL_TARGETS := \
  $(sort $(sav_JMK_LOCAL_TARGET) $(sav_JMK_LOCAL_CXX_TARGET) \
    $(sav_JMK_LOCAL_A_TARGET) $(sav_JMK_LOCAL_O_TARGET))
ALL_TARGETS := \
  $(sort $(LOCAL_TARGETS) $(NORMAL_TARGETS) $(sav_JMK_OTHER_TARGETS) \
    $(sav_JMK_OSAP_MODULE_TARGET))

A_TARGETS := $(sort $(sav_JMK_A_TARGET) $(sav_JMK_LOCAL_A_TARGET))

## lists vars

# define _O_OBJS_% _L_OBJS_%
# Using _[OL]_OBJS_% as the internal variable of all objs for each target.
# This variable contain the real object names for compilation including local_
# prefix for local targets.
# User shouldn't use _O_OBJS or _L_OBJS directly!

$(foreach t, \
  $(call Abs_Path_To_Var, $(NORMAL_TARGETS) $(sav_JMK_OTHER_TARGETS)), \
  $(eval _O_OBJS_$t := $(sav_JMK_O_OBJS_$t)) \
  $(eval _L_OBJS_$t := $(sav_JMK_L_OBJS_$t)))

$(foreach t, $(call Abs_Path_To_Var,$(LOCAL_TARGETS)), \
  $(eval _O_OBJS_$t := $(call ADD_LOCAL_PREFIX,$(sav_JMK_O_OBJS_$t))) \
  $(eval _L_OBJS_$t := $(call ADD_LOCAL_PREFIX,$(sav_JMK_L_OBJS_$t))) \
  $(eval ALL_LOCAL_OBJS += $(_O_OBJS_$t)))

ALL_LOCAL_OBJS := $(sort $(ALL_LOCAL_OBJS))

# Create _O_OBJS_%_pic

# handle o-target under so_target/a_target_pic (also recursively)
# $1 - list of o-targets
Pic_O_Target = \
  $(foreach o, $1, \
    $(eval new_o_targets+=$(o:%.o=%_pic.o)) \
    $(call Pic_O_Objs,$(call Abs_Path_To_Var,$o),$(call Abs_Path_To_Var,$(o:%.o=%_pic.o))))

# $1 - object in var format
# $2 - pic object in var format
Pic_O_Objs = \
  $(eval _O_OBJS_$2 := $(sav_JMK_O_OBJS_$1:%.o=%_pic.o)) \
  $(eval pic_objs += $(_O_OBJS_$2)) \
  $(call Pic_O_Target, $(filter $(sav_JMK_O_TARGET),$(sav_JMK_O_OBJS_$1)))

$(foreach t, \
  $(call Abs_Path_To_Var,$(sav_JMK_SO_TARGET) $(sav_JMK_A_TARGET_PIC)), \
  $(call Pic_O_Objs,$t,$t))

sav_JMK_O_TARGET+=$(new_o_targets)
NORMAL_TARGETS+=$(new_o_targets)
ALL_TARGETS+=$(new_o_targets)

# Change module objects from x.o to x_mod_24.o and/or x_mod_26.o
# $1 - target in var format
# $2 - linux version: 24/26
# $3 - empty for O_OBJS, "X" for OX_OBJS
Mod_O_Objs = \
  $(if $(CONFIG_RG_OS_LINUX_$2), \
    $(patsubst %.o,%_mod_$2.o, \
      $(sav_JMK_O$3_OBJS_$1) $(if $3,$(sav_JMK_MOD_$2_O_OBJS_$1))))

# Create _O_OBJS_% _OX_OBJS_% for JMK_MOD_TARGET
$(foreach t,$(call Abs_Path_To_Var,$(sav_JMK_MOD_TARGET)), \
  $(eval _O_OBJS_$t := $(call Mod_O_Objs,$t,24,) $(call Mod_O_Objs,$t,26,)) \
  $(eval _OX_OBJS_$t := $(call Mod_O_Objs,$t,24,X) $(call Mod_O_Objs,$t,26,X)) \
  $(eval ox_objs += $(_OX_OBJS_$t)) \
  $(eval mod_objs += $(_O_OBJS_$t) $(_OX_OBJS_$t)))

mod2stat_objs := \
  $(foreach t,$(call Abs_Path_To_Var,$(sav_JMK_MOD_2_STAT)), \
    $(_O_OBJS_$t) $(_OX_OBJS_$t))

ALL_TARGET_OBJS := $(sort \
  $(foreach t,$(call Abs_Path_To_Var,$(NORMAL_TARGETS)), \
    $(_O_OBJS_$t) $(_OX_OBJS_$t)) \
  $(sav_JMK_OTHER_OBJS))

ALL_OBJS := $(sort \
  $(ALL_LOCAL_OBJS) $(ALL_TARGET_OBJS) $(filter %.o,$(sav_JMK_OTHER_TARGETS)))
ALL_PRODS := $(ALL_OBJS) $(ALL_TARGETS)

ifdef CONFIG_BINFMT_FLAT
  ALL_PRODS += $(foreach t,$(sav_JMK_TARGET),$t.elf.o $t.gdb.o)
endif

export_headers_srcs := \
  $(foreach f,$(sav_cp_JMK_EXPORT_HEADERS),$(call GET_FILE_FROM_TO,$f,1))

## Abs_Path_To_Var

all_targets_var := $(call Abs_Path_To_Var,$(ALL_TARGETS))

$(foreach t,$(all_targets_var), \
  $(eval target_objs_var_$t := \
    $(call Abs_Path_To_Var,$(sav_JMK_O_OBJS_$t) $(sav_JMK_OX_OBJS_$t))))

## direct access vars

$(foreach o, $(call Abs_Path_To_Var,$(ALL_LOCAL_OBJS)), \
  $(eval is_local_obj_$o := y))

$(foreach o, $(call Abs_Path_To_Var,$(pic_objs)), \
  $(eval is_pic_obj_$o := y))

$(foreach o, $(call Abs_Path_To_Var,$(ox_objs)), \
  $(eval is_ox_obj_$o := y))

$(foreach o, $(call Abs_Path_To_Var,$(mod_objs)), \
  $(eval is_mod_obj_$o := y))

$(foreach o, $(call Abs_Path_To_Var,$(mod2stat_objs)), \
  $(eval is_mod2stat_obj_$o := y))

$(foreach t,$(sav_JMK_SO_TARGET), \
  $(eval so_target_$(call Abs_Path_To_Var,$(t:%.so=%.a)) := $t))

# $1 - starting object name in var format (including all _pic _mod_24 etc)
# $2 - fixed name in var format
# $3 - fixed-no-local name in var format
Define_Fix_Name= \
  $(eval fix_obj_name_$1 := $2) \
  $(eval fix_obj_name_nolocal_$1 := $3)

# $1 - starting object name
# $2 - fixed name
_Define_Fix_Name_Target= \
  $(call Define_Fix_Name,$1,$2,$2)
Define_Fix_Name_Target= \
  $(call _Define_Fix_Name_Target,$(call Abs_Path_To_Var,$1),$(call Abs_Path_To_Var,$2))

# $1 - starting object name
# $2 - fixed name
_Define_Fix_Name_Local= \
  $(call Define_Fix_Name,$1,$1,$2)
Define_Fix_Name_Local= \
  $(call _Define_Fix_Name_Local,$(call Abs_Path_To_Var,$1),$(call Abs_Path_To_Var,$2))

$(foreach o,$(ALL_LOCAL_OBJS), \
  $(call Define_Fix_Name_Local,$o,$(call DEL_LOCAL_PREFIX,$o)))

$(foreach o,$(ALL_TARGET_OBJS) $(filter %.o,$(sav_JMK_OTHER_TARGETS)), \
  $(call Define_Fix_Name_Target,$o,$o))

$(foreach o,$(pic_objs), \
  $(call Define_Fix_Name_Target,$o,$(o:%_pic.o=%.o)))

$(foreach o,$(mod_objs), \
  $(call Define_Fix_Name_Target,$o,$(patsubst %_mod_24.o,%.o,$(patsubst %_mod_26.o,%.o,$o))))
