
ifdef JMKE_IS_BUILDDIR

  ALL_CLEAN_FILES=$(JMK_CLEAN) $(ALL_OBJS) $(ALL_PRODS) *.d
  ALL_CLEAN_FILES+=$(foreach t,$(JMK_TARGET) $(JMK_SO_TARGET) $(PIC_A_TARGET) $(JMK_MOD_TARGET),$(DEBUG_PATH)/$t $t.link.map)

  ifeq ($(CONFIG_BINFMT_FLAT),y)
    ALL_CLEAN_FILES+=$(foreach t,$(JMK_TARGET),$t.elf.o) $(foreach t,$(JMK_TARGET),$t.gdb.o) $(JMK_FAST_RAM_OBJS) $(JMK_FAST_RAM_OBJS:%.fr=%.o)
  endif

  ALL_CLEAN_FILES:=$(filter-out $(NOT_FOR_CLEAN),$(ALL_CLEAN_FILES))


  clean:: $(JMK_FIRST_CLEAN) $(CLEAN_SUBDIRS_) do_clean

  do_clean:
	rm -rf $(ALL_CLEAN_FILES)

  .PHONY: do_clean clean

endif

distclean:
	rm -rf $(JMKE_BUILDDIR) $(BUILDDIR_LINK)

