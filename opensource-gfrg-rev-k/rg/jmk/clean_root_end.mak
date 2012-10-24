ALL_CLEAN_FILES+=$(sav_JMK_CLEAN) $(ALL_OBJS:%.o=%.d) \
  $(ALL_PRODS) $(PIC_A_TARGET) \
  $(foreach t, $(NORMAL_TARGETS) $(PIC_A_TARGET), \
    $(DEBUG_PATH)/$(notdir $t) $t.link.map)

clean: $(sav_JMK_FIRST_CLEAN) do_clean

do_clean: $(sav_JMK_FIRST_CLEAN) 
	rm -rf $(ALL_CLEAN_FILES)

.PHONY: do_clean clean
