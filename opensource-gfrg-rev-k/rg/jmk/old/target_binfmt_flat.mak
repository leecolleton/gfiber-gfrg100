ifndef CONFIG_BINFMT_FLAT
  $(error CONFIG_BINFMT_FLAT is not defined, but using its logic!)
endif

# Create the JMK_TARGET: JMK_TARGET.elf dependencies
$(foreach t, $(JMK_TARGET), $(eval $t: $t.elf.o))


# Create the JMK_TARGET.elf: JMK_O_OBJS_TARGET dependencies
$(foreach t, $(JMK_TARGET:%=%.elf.o), \
    $(eval $t: $(_O_OBJS_$(t:%.elf.o=%)) $(_L_OBJS_$(t:%.elf.o=%))))

# TODO implement support for zipped files
#LDFLAT_FLAGS+=-z

# Initialize executable stack to default of 4KB
JMK_FLAT_STACK_SIZE=4096

FLTFLAGS=$(LDFLAT_FLAGS)

# ld running elf2flt uses FLTFLAGS so they need to be exported
export FLTFLAGS

$(JMK_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),\
	$(CC) $(_O_OBJS_$@) $(_L_OBJS_$@) $(filter-out $(JMK_LDFLAGS_REMOVE_$@),\
	$(JMK_LDFLAGS)) $(JMK_LDFLAGS_$@) $(MAK_LDFLAGS) $(JMK_LDLIBS) $(JMK_LDLIBS_$@) -o \
	$@ && $(FLTHDR) -s \
	$(if $(JMK_FLAT_STACK_SIZE_$@),$(JMK_FLAT_STACK_SIZE_$@),$(JMK_FLAT_STACK_SIZE))\
	$@,\
	echo "Skipping JMK_TARGET($@:$^) Since JPKG is on")
	@$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@
