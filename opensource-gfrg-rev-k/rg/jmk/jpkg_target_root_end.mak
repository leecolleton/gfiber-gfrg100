jpkg_targets_all=$(if $(CONFIG_RG_JPKG),$(JMKE_DOING_MAKE_CONFIG),y)

# $1 - target in var format
# $2 - target's dir in var format
_Is_Jpkg_Target= $(strip \
  $(sav_JMK_JPKG_TARGET_$1)$(sav_JMK_JPKG_TARGETS_ALL_$2) \
  $(if $(CONFIG_RG_JPKG_BIN), \
    $(sav_JMK_JPKG_TARGET_BIN_$1)$(sav_JMK_JPKG_BIN_TARGETS_ALL_$2)))

# $1 - relative path of target/object
Is_Jpkg_Target = $(or $(jpkg_targets_all), \
  $(call _Is_Jpkg_Target,$(call Abs_Path_To_Var,$1),$(call Abs_Path_To_Dir_Var,$1)))

jpkg_targets= \
  $(foreach t,$(ALL_TARGETS), \
    $(if $(call Is_Jpkg_Target,$t),$t)) \

$(foreach t, $(call Abs_Path_To_Var,$(jpkg_targets)), \
  $(eval sav_JMK_JPKG_TARGET_$t=y) \
  $(foreach o, $(_O_OBJS_$t) $(_OX_OBJS_$t), \
    $(eval sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$o)=y)))
