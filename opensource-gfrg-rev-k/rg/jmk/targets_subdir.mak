ifdef JMK_CREATE_LOCAL
  JMK_LOCAL_TARGET:=$(JMK_LOCAL_TARGET) \
    $(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_TARGET)), \
      $(filter-out ./,$(dir $t))local_$(notdir $t))
  JMK_LOCAL_A_TARGET:=$(JMK_LOCAL_A_TARGET) \
    $(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_A_TARGET)), \
      $(filter-out ./,$(dir $t))$(patsubst lib%.a,liblocal_%.a,$(notdir $t)))
  JMK_LOCAL_O_TARGET:=$(JMK_LOCAL_O_TARGET) \
    $(foreach t,$(filter $(JMK_CREATE_LOCAL),$(JMK_O_TARGET)), \
      $(filter-out ./,$(dir $t))$(patsubst %.o,local_%.o,$(notdir $t)))
endif
