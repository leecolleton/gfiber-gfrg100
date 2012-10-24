JMK_JPKG_TARGETS_ALL:=$(strip $(JMKE_DOING_MAKE_CONFIG) $(JMK_JPKG_TARGETS_ALL) \
  $(if $(CONFIG_RG_JPKG_BIN),$(JMK_JPKG_BIN_TARGETS_ALL)))
ifndef CONFIG_RG_JPKG
  JMK_JPKG_TARGETS_ALL:=y
endif

ifdef JMK_JPKG_TARGETS_ALL
  JPKG_TARGETS:=$(ALL_TARGETS) $(foreach t,$(A_TARGETS),__create_lib_$t)
else
  JPKG_TARGETS:=$(foreach t,$(ALL_TARGETS),$(if $(JMK_JPKG_TARGET_$t),$t)) \
    $(foreach t,$(A_TARGETS),$(if $(JMK_JPKG_TARGET_$t),__create_lib_$t))
  ifdef CONFIG_RG_JPKG_BIN
    JPKG_TARGETS:=$(foreach t,$(ALL_TARGETS),$(if $(JMK_JPKG_TARGET_BIN_$t),$t)) \
      $(foreach t,$(A_TARGETS),$(if $(JMK_JPKG_TARGET_BIN_$t),__create_lib_$t)) \
      $(JPKG_TARGETS)
  endif
endif

ifdef JPKG_TARGETS
  $(foreach t,$(JPKG_TARGETS), $(eval JMK_JPKG_TARGET_$t=y))
  $(foreach t,$(JPKG_TARGETS),\
    $(foreach o,$(_O_OBJS_$t) $(_OX_OBJS_$t),\
        $(eval JMK_JPKG_TARGET_$o=y)))
endif

