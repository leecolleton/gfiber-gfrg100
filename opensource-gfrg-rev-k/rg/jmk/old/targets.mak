NORMAL_TARGETS=$(strip $(sort $(JMK_TARGET) $(JMK_O_TARGET) $(JMK_SO_TARGET) $(JMK_A_TARGET) \
  $(JMK_MOD_TARGET)))
LOCAL_TARGETS=$(strip $(sort $(JMK_LOCAL_TARGET) $(JMK_LOCAL_CXX_TARGET) \
  $(JMK_LOCAL_A_TARGET))) $(JMK_LOCAL_O_TARGET)
ALL_TARGETS=$(LOCAL_TARGETS) $(NORMAL_TARGETS) $(JMK_OTHER_TARGETS)
# Note: I'm using _O_OBJS_% variables that are only calculated in rg.mak. 
# These calculated variables can be used only in and after including rg.mak
ALL_LOCAL_OBJS=$(sort $(foreach o,$(LOCAL_TARGETS),$(_O_OBJS_$o)))

ALL_TARGET_OBJS=$(sort $(foreach o,$(NORMAL_TARGETS),$(_O_OBJS_$o) \
  $(_OX_OBJS_$o)) $(JMK_O_OBJS) $(_OTHER_OBJS) $(JMK_sO_OBJS))

ALL_OBJS=$(ALL_LOCAL_OBJS) $(ALL_TARGET_OBJS) $(filter %.o,$(JMK_OTHER_TARGETS))
ALL_PRODS=$(ALL_OBJS) $(ALL_TARGETS)

ifdef CONFIG_BINFMT_FLAT
  ALL_PRODS+=$(foreach t,$(JMK_TARGET),$t.elf.o)
  ALL_PRODS+=$(foreach t,$(JMK_TARGET),$t.gdb.o)
endif

A_TARGETS=$(JMK_A_TARGET) $(JMK_LOCAL_A_TARGET)

JMK_MOD_TARGET+=$(JMK_MOD_O_TARGET)

ifdef JMK_CREATE_LOCAL
  JMK_LOCAL_TARGET+=$(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_TARGET)), \
    $(filter-out ./,$(dir $t))local_$(notdir $t))
  JMK_LOCAL_A_TARGET+=$(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_A_TARGET)), \
    $(filter-out ./,$(dir $t))$(patsubst lib%.a,liblocal_%.a,$(notdir $t)))
  JMK_LOCAL_O_TARGET+=$(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_O_TARGET)), \
    $(filter-out ./,$(dir $t))$(patsubst %.o,local_%.o,$(notdir $t)))
endif
