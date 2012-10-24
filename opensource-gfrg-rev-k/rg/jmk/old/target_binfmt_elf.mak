# Create the JMK_TARGET: JMK_O_OBJS and JMK_L_OBJS dependencies
$(foreach t,$(JMK_TARGET),$(eval \
    $t: $(_O_OBJS_$t) $(if $(JMK_JPKG_TARGET_$t),$(_L_OBJS_$t))))

LINKAGE_MAP_FLAGS=-Wl,-Map $@.link.map
TRACE_FLAG=-Wl,--trace

$(JMK_TARGET): $(_O_OBJS_$@) $(if $(JMK_JPKG_TARGET_$@),$(_L_OBJS_$@))
	$(if $(JMK_JPKG_TARGET_$@),\
	  touch $@.link.map; \
	  perl $(JMK_ROOT)/pkg/build/app_rm_unneeded_syms.pl $(JMK_ROOT) $@ \
	    $(notdir $@) $(subst /,_,$(PWD_REL)_$@) \
	    $(CC) $^ $(call FIX_VPATH_LDFLAGS,$(filter-out $(JMK_LDFLAGS_REMOVE_$@),\
	    $(JMK_LDFLAGS)) $(JMK_LDFLAGS_$@) $(MAK_LDFLAGS)) $(JMK_LDLIBS) $(JMK_LDLIBS_$@) \
	    $(TRACE_FLAG) $(LINKAGE_MAP_FLAGS) -o $@,\
	  echo "Skipping JMK_TARGET($@:$^) since creating JPKG"\
	 )
	$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@
