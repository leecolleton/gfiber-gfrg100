_GET_OBJ_SEC_PERM=$(call DBGFR,$(TARGET_SEC_PERM_$(call \
  FIX_OBJ_NAME,$2))$(JMK_SEC_PERM_$(call \
  FIX_OBJ_NAME,$2)),_GET_OBJ_SEC_PERM,$1,$2,$3)

GET_SEC_PERM_ID=$(if $(call _GET_OBJ_SEC_PERM,$1,$(call \
  FIX_OBJ_NAME_FOR_ENTITY_ID, $2),$3),$(call _GET_OBJ_SEC_PERM,$1,$(call \
  FIX_OBJ_NAME_FOR_ENTITY_ID, $2),$3),$(JMK_SEC_PERM))

# Create TARGET_SEC_PERM_%
$(foreach t,$(ALL_TARGETS), \
    $(foreach o,$(JMK_O_OBJS_$t) $(JMK_OX_OBJS_$t), \
        $(eval TARGET_SEC_PERM_$o:=$(JMK_SEC_PERM_$t))))

