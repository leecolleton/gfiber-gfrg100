# Create TARGET_SEC_PERM_%
$(foreach t,$(all_targets_var), \
  $(foreach o,$(target_objs_var_$t), \
    $(eval TARGET_SEC_PERM_$o:=$(sav_JMK_SEC_PERM_$t))))

_GET_SEC_PERM_ID=$(or $(sav_JMK_SEC_PERM_$1),$(TARGET_SEC_PERM_$1))
GET_SEC_PERM_ID=$(strip \
  $(call _GET_SEC_PERM_ID,$(fix_obj_name_nolocal_$(call Abs_Path_To_Var,$1))))
