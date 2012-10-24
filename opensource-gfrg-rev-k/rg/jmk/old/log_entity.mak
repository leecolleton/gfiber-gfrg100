export JMK_DIST_LOG_ENTITY_EXCLUDE
ifeq ($(JMK_DIST_LOG_ENTITY_EXCLUDE),)
  JMK_DIST_LOG_ENTITY_EXCLUDE=n
endif

_GET_OBJ_LOG_ENTITY=$(call DBGFR,$(TARGET_LOG_ENTITY_$(call \
  FIX_OBJ_NAME,$2))$(JMK_LOG_ENTITY_$(call \
  FIX_OBJ_NAME,$2)),_GET_OBJ_LOG_ENTITY,$1,$2,$3)

# Remove "local" prefix from local objects, so they will use the same entity
# ID as the target objects
FIX_OBJ_NAME_FOR_ENTITY_ID=$(if $(call IS_LOCAL_OBJ,$1),$(patsubst \
  local_%,%,$1),$1)
ENTITY_ID=$(if $(call _GET_OBJ_LOG_ENTITY,$1,$(call \
  FIX_OBJ_NAME_FOR_ENTITY_ID, $2),$3),$(call _GET_OBJ_LOG_ENTITY,$1,$(call \
  FIX_OBJ_NAME_FOR_ENTITY_ID, $2),$3),$(JMK_LOG_ENTITY))

# the file will be sorted in the end - see jmk/log_entity_root_end.mak
dist_log_entity:
ifeq ($(JMK_DIST_LOG_ENTITY_EXCLUDE)$(CONFIG_RG_MGT),ny)
    #Every subdirectory adds its entity to the list of entity
	echo '$(JMK_LOG_ENTITY)' >> $(JMKE_BUILDDIR)/pkg/include/dist_log_entity.h
    #Also, every file that compiles add its entity to the list of entities
	$(foreach o,$(ALL_OBJS),$(if $(JMK_LOG_ENTITY_$o),echo '$(JMK_LOG_ENTITY_$o)' >> $(JMKE_BUILDDIR)/pkg/include/dist_log_entity.h;,))
    #Also, add the option to add more entities manually to the list of entities
	echo $(JMK_DIST_LOG_ENTITY) >> $(JMKE_BUILDDIR)/pkg/include/dist_log_entity.h
endif

# Create TARGET_LOG_ENTITY_%
$(foreach t,$(ALL_TARGETS), \
    $(foreach o,$(JMK_O_OBJS_$t) $(JMK_OX_OBJS_$t), \
        $(eval TARGET_LOG_ENTITY_$o:=$(JMK_LOG_ENTITY_$t))))

