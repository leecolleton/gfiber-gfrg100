# Create the JMK_TARGET: JMK_TARGET.elf dependencies
$(foreach t, $(sav_JMK_TARGET), $(eval $t: $t.elf.o))

# Create the JMK_TARGET.elf: JMK_O_OBJS_TARGET dependencies
$(foreach t, $(sav_JMK_TARGET), \
  $(eval $t.elf.o: $(_O_OBJS_$(call Abs_Path_To_Var,$t)) \
    $(_L_OBJS_$(call Abs_Path_To_Var,$t))))

# TODO implement support for zipped files
#LDFLAT_FLAGS+=-z

# Initialize executable stack to default of 4KB
FLAT_STACK_SIZE=4096

FLTFLAGS=$(LDFLAT_FLAGS)

# ld running elf2flt uses FLTFLAGS so they need to be exported
export FLTFLAGS

$(sav_JMK_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(CC) $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(filter-out $(sav_JMK_LDFLAGS_REMOVE_$(call Abs_Path_To_Var,$@)), \
	    $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@))) $(LDFLAGS_ENVIR) \
	  $(call Get_Libs_Flags,$(sav_JMK_LIBS_$(call Abs_Path_To_Var,$@))) \
	  -o $@ && $(FLTHDR) \
	  -s $(or $(sav_JMK_FLAT_STACK_SIZE_$(call Abs_Path_To_Var,$@)), \
	    $(FLAT_STACK_SIZE)) $@, \
	echo "Skipping JMK_TARGET($@:$^) Since JPKG is on")
	@$(JMKE_LN) $@ $(DEBUG_PATH)/$(nodir $@)
