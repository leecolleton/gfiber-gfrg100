# OSAP libs must be linked twice with some work between the links.
# for each libX.so in JMK_OSAP_SO_TARGET run by this order:
# - link libX__lib_osap.so from the objects of libX.so
# - create list (libX__osap_openrg_sym_list.txt) of undefined symbols in
#   libX__lib_osap.so 
# - objcopy these symbols from openrg into libX__osap_openrg_sym
# - link libX.so from its objects again, this time with the extra flag
#   -Wl,--just-symbols=libX__osap_openrg_sym

# $1 - target
# $2 - library
# $3 - symbol file
# $4 - Abs_Path_To_Var(target)
Osap_SO_Target = \
  $(eval sav_JMK_O_OBJS_$(call Abs_Path_To_Var,$2) = $(sav_JMK_O_OBJS_$4)) \
  $(eval sav_JMK_SO_TARGET+=$1 $2) \
  $(eval $3: $2 $(JMKE_BUILDDIR)/pkg/main/openrg) \
  $(eval $1: $3) \
  $(eval sav_JMK_SO_LDFLAGS_$4+=-Wl,--just-symbols=$3) \
  $(eval SO_ARCHIVE_REMOVE_$4+=$3)

$(foreach t,$(sav_JMK_OSAP_SO_TARGET), \
  $(call Osap_SO_Target,$t,$(basename $t)__lib_osap.so,$(basename $t)__osap_openrg_sym,$(call Abs_Path_To_Var,$t)))

$(foreach t,$(sav_JMK_OSAP_SO_TARGET),$(basename $t)__osap_openrg_sym):
	$(NM) -u $< | awk '{print $$2}' > $@_list.txt
	$(OBJCOPY) --strip-all --keep-symbols=$@_list.txt \
	  $(JMKE_BUILDDIR)/pkg/main/openrg $@

# OSAP modules

GEN_OSAP_MANIFEST := $(JMKE_BUILDDIR)/pkg/main/gen_osap_manifest

# $1 - module
# $2 - Abs_Path_To_Var(module)
Osap_Module_Target = \
  $(eval $1: $(sav_JMK_OSAP_MODULE_COMPONENTS_$2) $(GEN_OSAP_MANIFEST) \
  $(sav_JMK_OSAP_MODULE_SCRIPT_$2))

# $1 - module
Osap_Module_Script = $(sav_JMK_OSAP_MODULE_SCRIPT_$(call Abs_Path_To_Var,$1))

$(foreach t,$(sav_JMK_OSAP_MODULE_TARGET), \
  $(call Osap_Module_Target,$t,$(call Abs_Path_To_Var,$t)))

$(sav_JMK_OSAP_MODULE_TARGET):
	$(JMK_ROOT)/jmk/build_osap_module.sh $(call Rel_Path_To_Abs,$@) \
		$(GEN_OSAP_MANIFEST) \
		$(if $(call Osap_Module_Script,$@), \
			-s $(call Osap_Module_Script,$@)) \
		$(filter-out $(GEN_OSAP_MANIFEST) $(RG_LIB)/libc_so.a \
			$(call Osap_Module_Script,$@),$^)
