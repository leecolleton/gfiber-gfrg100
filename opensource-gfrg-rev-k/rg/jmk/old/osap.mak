ifdef JMKE_IS_BUILDDIR
$(foreach t,$(JMK_OSAP_SO_TARGET), \
  $(eval JMK_O_OBJS_lib__osap_$t=$(JMK_O_OBJS_$t)) \
  $(eval JMK_SO_TARGET+=lib__osap_$t) \
  $(eval __osap_openrg_sym_$(basename $t): lib__osap_$t \
    $(JMKE_BUILDDIR)/pkg/main/openrg) \
  $(eval $t: __osap_openrg_sym_$(basename $t)) \
  $(eval \
    JMK_SO_LDFLAGS_$t+=-Wl,--just-symbols=__osap_openrg_sym_$(basename $t)) \
  $(eval SO_ARCHIVE_REMOVE_$t+=__osap_openrg_sym_$(basename $t)) \
  $(eval JMK_SO_TARGET+=$t))

$(foreach t,$(JMK_OSAP_SO_TARGET),__osap_openrg_sym_$(basename $t)):
	$(NM) -u $< | awk '{print $$2}' > \
	  $(@:__osap_openrg_sym%=__osap_sym_list%).txt
	$(OBJCOPY) --strip-all \
	  --keep-symbols=$(@:__osap_openrg_sym%=__osap_sym_list%).txt \
	  $(JMKE_BUILDDIR)/pkg/main/openrg $@
endif
