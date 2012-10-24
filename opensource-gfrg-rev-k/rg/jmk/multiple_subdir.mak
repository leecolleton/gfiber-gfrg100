ifdef JMK_COMPILE_MULTIPLE_CONFIGS

IS_ANY_MULTIPLE_CONFIG_SET=$(strip \
  $(foreach conf, \
    $(filter-out $(JMK_COMPILE_MULTIPLE_CONFIGS_SKIP), \
      $(JMK_COMPILE_MULTIPLE_CONFIGS)), \
    $($(conf))))

normal_targets=$(sort $(JMK_TARGET) $(JMK_O_TARGET) $(JMK_SO_TARGET) \
  $(JMK_A_TARGET) $(JMK_MOD_TARGET) $(JMK_MOD_O_TARGET))
local_targets=$(sort $(JMK_LOCAL_TARGET) $(JMK_LOCAL_CXX_TARGET) \
  $(JMK_LOCAL_A_TARGET) $(JMK_LOCAL_O_TARGET))
all_targets=$(local_targets) $(normal_targets) $(JMK_OTHER_TARGETS)

ifeq ($(IS_ANY_MULTIPLE_CONFIG_SET),)

  # None of the configs in JMK_COMPILE_MULTIPLE_CONFIGS is set - add to
  # JMK_O[X]_OBJS the multiple objs as they are

  # add to JMK_O_OBJS_/JMK_OX_OBJS_ the multiple objs
  $(foreach t,$(all_targets), \
    $(eval JMK_O_OBJS_$t+=$(JMK_O_OBJS_MULTIPLE_$t) $(JMK_O_OBJS_MULTIPLE)) \
    $(eval JMK_OX_OBJS_$t+=$(JMK_OX_OBJS_MULTIPLE_$t) $(JMK_OX_OBJS_MULTIPLE)))

else # IS_ANY_MULTIPLE_CONFIG_SET

# helper function for loops
# $1 - empty for JMK_O_OBJS , or
#      X     for JMK_OX_OBJS
# $2 - target name
_O_OR_OX_MULTIPLE= \
  $(if $(JMK_O$1_OBJS_MULTIPLE_$2), \
    $(foreach conf, \
      $(filter-out $(JMK_COMPILE_MULTIPLE_CONFIGS_SKIP), \
	$(JMK_COMPILE_MULTIPLE_CONFIGS)), \
      $(if $($(conf)), $(call Rel_Path_To_Abs, \
	$(patsubst %.o,%.$(conf:CONFIG_%=%).o, \
	  $(JMK_O$1_OBJS_MULTIPLE_$2) $(JMK_O$1_OBJS_MULTIPLE)), \
	$(JMKE_PWD_BUILD)/))))

# - create renamed objs/X_objs for all configs
#   e.g. for CONFIG_ABC - 
#   a.o => a.ABC.o
# - add to JMK_O[X]_OBJS_ the renamed objs
# - save all in ALL_MULTIPLE_OBJS
$(foreach t,$(all_targets), \
  $(eval _O_OBJS_MULTIPLE_RENAMED:=$(call _O_OR_OX_MULTIPLE,,$t)) \
  $(eval _OX_OBJS_MULTIPLE_RENAMED:=$(call _O_OR_OX_MULTIPLE,X,$t)) \
  $(eval JMK_O_OBJS_$t:=$(JMK_O_OBJS_$t) $(_O_OBJS_MULTIPLE_RENAMED)) \
  $(eval JMK_OX_OBJS_$t:=$(JMK_OX_OBJS_$t) $(_OX_OBJS_MULTIPLE_RENAMED)) \
  $(eval ALL_MULTIPLE_OBJS:=$(ALL_MULTIPLE_OBJS) \
    $(_O_OBJS_MULTIPLE_RENAMED) $(_OX_OBJS_MULTIPLE_RENAMED)))


endif # IS_ANY_MULTIPLE_CONFIG_SET
endif # JMK_COMPILE_MULTIPLE_CONFIGS
