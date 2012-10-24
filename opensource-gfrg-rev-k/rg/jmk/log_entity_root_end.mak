# Create TARGET_LOG_ENTITY_%
$(foreach t,$(all_targets_var), \
  $(foreach o,$(target_objs_var_$t), \
    $(eval TARGET_LOG_ENTITY_$o:=$(sav_JMK_LOG_ENTITY_$t))))

_GET_ENTITY_ID=$(or $(sav_JMK_LOG_ENTITY_$1),$(TARGET_LOG_ENTITY_$1))

GET_ENTITY_ID = $(strip \
  $(call _GET_ENTITY_ID,$(fix_obj_name_nolocal_$(call Abs_Path_To_Var,$1))))

dist_log_h_file=$(JMKE_BUILDDIR)/pkg/include/dist_log_entity.h

# XXX takes long time - try to improve
dist_log_excludes:=$(addsuffix /,$(sav_JMK_DIST_LOG_ENTITY_EXCLUDE))
dist_log_h_new = \
  $(sav_JMK_DIST_LOG_ENTITY) \
  $(foreach o,$(ALL_OBJS), \
    $(if $(filter $(dir $o),$(dist_log_excludes)),, \
      $(call GET_ENTITY_ID,$o)))
dist_log_h_prev = $(shell uniq $(dist_log_h_file))
dist_log_entities = $(sort $(dist_log_h_prev) $(dist_log_h_new))

ifeq ($(JMKE_DOING_MAKE_CONFIG),y)
  # Create pkg/include/dist_log_entity.h
  # \054 is for comma
  # Every subdirectory adds its entity to the list of entity
  # Also, every file that compiles adds its entity to the list of entities
  $(dist_log_h_file): $(addsuffix /Makefile,$(call BUILD2SRC,$(sav_SUBDIRS))) \
    legacy_archconfig
	echo -e $(addsuffix '\054\n',$(dist_log_entities)) > $@
endif
