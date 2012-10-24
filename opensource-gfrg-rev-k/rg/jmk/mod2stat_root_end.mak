# $1 - object to link to
Static_Link_Name = \
  $(STATIC_MOD_DIR)$(sav_JMK_MOD_2_STAT_PREFIX_$(call Abs_Path_To_Var,$1))$(notdir $1)

# $1 - object to link to
# $2 - static link name
Static_Link = \
  $(eval $2: $1 ; \
    $(JMKE_LN) $1 $2) \
  $(eval MOD_2_STAT_LINKS+=$2)

$(foreach o, $(filter-out $(sav_JMK_MOD_O_TARGET),$(sav_JMK_MOD_2_STAT)), \
  $(call Static_Link,$o,$(call Static_Link_Name,$o)))
