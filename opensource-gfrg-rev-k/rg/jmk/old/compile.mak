# $1 - src
_IS_CPP_FILE=$(call DBGFR,\
  $(filter %.cpp %.cxx %.cc,$1)\
,_IS_CPP_FILE,$1)

# $1 - existing flags
# $2 - obj name
# $3 - src
# - For .c files, the target compiler is always rg_gcc, but for .cpp files we 
#   have to decide ourselves
# - In glibc, both CXX and TARGET_CXX point to the toolchain compiler. If
#   ulibc is also set, then CXX is set to rg_gcc but TARGET_CXX keeps its value
_GET_COMPILER=$(call DBGFR,\
  $(if $(call IS_LOCAL_OBJ,$2),\
    $(CC_FOR_BUILD),\
    $(if $(call GET_MODULE,$2),\
      $(TARGET_CC),\
      $(if $(call _IS_CPP_FILE,$3),\
        $(if $(call _IS_COMPILING_ULIBC,$1),\
          $(CXX),\
  	  $(TARGET_CXX)\
        ),\
        $(CC)\
      )\
    )\
  )\
,_GET_COMPILER,$1,$2,$3)

# Compilation functions for internal use only
# Usage: $(call RG_COMPILE_FUNC,EXTRA_FLAGS,output,src)
_RG_COMPILE_FUNC=$(strip \
  $(JMKE_LN_NOFAIL) $(call BUILD2SRC,$3) $(call SRC2BUILD,$3) && \
  $(call _GET_COMPILER,$1,$2,$3)\
  $(call GET_OBJ_CFLAGS,$1,$2,$3) -c -o $2 \
  $(filter-out $(JMKE_PWD_SRC)/Makefile,\
    $(subst $(JMKE_PWD_BUILD)/,,$(call SRC2BUILD_ALLOW_RELATIVE,$3))) \
  $(if $(STRIP_OBJS),&& $(STRIP) $(STRIP_FLAGS) $2) \
)

# Distributin source files.
# This is the place to do keyword expansion, strip_ifdef, anything else?
RG_COMPILE_FUNC_JPKG_SRC=$(if $(filter $(JMK_DONT_EXPORT),$(notdir $1) $1),\
  echo "Not exporting SRC $1",$(RG_VPATH_CP) $1 \
  $(JPKG_CUR_DIR)/$(1:$(JMKE_PWD_SRC)/%=%))

ifndef CONFIG_RG_JPKG
  RG_COMPILE_FUNC=$(call _RG_COMPILE_FUNC,$1,$2,$3)
endif
ifdef CONFIG_RG_JPKG_SRC
  RG_COMPILE_FUNC=$(if $(JMK_JPKG_TARGET_$2),$(call _RG_COMPILE_FUNC,$1,$2,$3) &&) $(call RG_COMPILE_FUNC_JPKG_SRC,$3)
endif

ifdef CONFIG_RG_JPKG_BIN
  # this is the place to strip debug information
  JPKG_SOURCES=$(JMK_EXPORT_AS_SRC) \
    $(filter %.c,$(call GET_FILE_FROM,$(JMK_EXPORT_HEADERS)))

  # $1 - GLIBC or ULIBC
  # $2 - obj name
  # - if (JMK_JPKG_BIN_LIBCS_obj is set), then compile just for the specified
  #   libc.
  # - Otherwise, compile both ulibc and glibc.
  _NEEDS_TO_COMPILE_FOR=$(call DBGFR,\
    $(if $(CONFIG_$1),\
      $(if $(JMK_JPKG_BIN_LIBCS),\
        $(filter $1,$(JMK_JPKG_BIN_LIBCS)),\
        $(if $(JMK_JPKG_BIN_LIBCS_$2),\
          $(filter $1,$(JMK_JPKG_BIN_LIBCS_$2)),\
          yes_please\
        )\
      )\
    )\
  ,_NEEDS_TO_COMPILE_FOR,$1,$2)

  # _RG_COMPILE_AND_EXPORT: compiles and export using the default libc
  #
  # - if JMK_JPKG_TARGET_ is set, then the obj was already compiled in 
  #   RG_COMPILE_FUNC, so no need to compile again
  _RG_COMPILE_AND_EXPORT=$(call DBGFR,\
    $(if $(JMK_JPKG_TARGET_$2),\
      ,\
      $(call _RG_COMPILE_FUNC,$1,$2,$3) && \
    ) \
    $(RG_VPATH_CP) $2 $(JPKG_CUR_DIR)/$2.$(TARGET_MACHINE)\
  ,_RG_COMPILE_AND_EXPORT,$1,$2,$3)

  _RG_COMPILE_AND_EXPORT_BOTH_LIBCS=$(call DBGFR,\
    $(if $(call _NEEDS_TO_COMPILE_FOR,ULIBC,$2),\
      $(call _RG_COMPILE_FUNC,$(_ULIBC_FLAG) $1,$2,$3) && \
      $(JMKE_CP) $2 $2.ulibc && \
      $(RG_VPATH_CP) $2.ulibc $(JPKG_CUR_DIR)/$2.$(TARGET_MACHINE).ulibc &&\
    )\
    $(if $(call _NEEDS_TO_COMPILE_FOR,GLIBC,$2),\
      $(call _RG_COMPILE_FUNC,$(_GLIBC_FLAG) $1,$2,$3) && \
      $(JMKE_CP) $2 $2.glibc && \
      $(RG_VPATH_CP) $2.glibc $(JPKG_CUR_DIR)/$2.$(TARGET_MACHINE).glibc &&\
    )\
    true\
  ,_RG_COMPILE_AND_EXPORT_BOTH_LIBCS,$1,$2,$3)

  RG_COMPILE_FUNC=$(call DBGFR,\
    $(if $(JMK_JPKG_TARGET_$2),\
      $(call _RG_COMPILE_FUNC,$1,$2,$3) &&\
    )\
    $(if $(filter $(JPKG_SOURCES),$(notdir $3)),\
      $(call RG_COMPILE_FUNC_JPKG_SRC,$3),\
      $(if $(filter $(JMK_DONT_EXPORT),$2 $(2:local_%.o=%.o) $(2:%_pic.o=%.o)),\
        echo "Not exporting BIN $2",\
	$(if $(call IS_LOCAL_OBJ,$2),\
	  $(call _RG_COMPILE_AND_EXPORT,$1,$2,$3),\
	  $(if $(call GET_MODULE,$2),\
	    $(call _RG_COMPILE_AND_EXPORT,$1,$2,$3),\
	    $(call _RG_COMPILE_AND_EXPORT_BOTH_LIBCS,$1,$2,$3)\
	  )\
	)\
      )\
    )\
  ,RG_COMPILE_FUNC,$1,$2,$3)
endif

FILTER_SRC=$(filter %.c %.S %.s %.cpp %.cxx,$1)

ifdef JMKE_IS_BUILDDIR

# C source code compilation
%.o: %.c
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_24.o: %.c
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_26.o: %.c
	$(call RG_COMPILE_FUNC,,$@,$<)
	
# C++ source code compilation
%.o: %.cpp
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_24.o: %.cpp
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_26.o: %.cpp
	$(call RG_COMPILE_FUNC,,$@,$<)
%.o: %.cxx
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_24.o: %.cxx
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_26.o: %.cxx
	$(call RG_COMPILE_FUNC,,$@,$<)
%.o: %.cc
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_24.o: %.cc
	$(call RG_COMPILE_FUNC,,$@,$<)
%_mod_26.o: %.cc
	$(call RG_COMPILE_FUNC,,$@,$<)

%_pic.o: %.c
	$(call RG_COMPILE_FUNC,$(FPIC_FLAG),$@,$<)

%_pic.o: %.cpp
	$(call RG_COMPILE_FUNC,$(FPIC_FLAG),$@,$<)

%_pic.o: %.cxx
	$(call RG_COMPILE_FUNC,$(FPIC_FLAG),$@,$<)

# For now assembly code compiles the same as C code
%.o: %.S
	$(call RG_COMPILE_FUNC,-D__ASSEMBLY__,$@,$<)
%_mod_24.o: %.S
	$(call RG_COMPILE_FUNC,-D__ASSEMBLY__,$@,$<)
%_mod_26.o: %.S
	$(call RG_COMPILE_FUNC,-D__ASSEMBLY__,$@,$<)

local_%.o : %.c
	$(call RG_COMPILE_FUNC,,$@,$<)

local_%.o : %.cpp
	$(call RG_COMPILE_FUNC,,$@,$<)

$(JMK_sO_OBJS):
	$(call RG_COMPILE_FUNC,$(JMK_sOFLAGS) -x assembler-with-cpp,$@,$<)

# This is a default rule to make an object out of its dependencies.
$(_OTHER_OBJS):
	$(call RG_COMPILE_FUNC,,$@,$(call FILTER_SRC,$^))

%.c: %.y
	$(YACC) $(JMK_YFLAGS) $< -o $@

$(JMK_O_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),$(LD) $(ENDIAN_LDFLAGS) -r -o $@ $(foreach o,$(filter-out $(JMKE_PWD_SRC)/Makefile,$^), $(call OS_PATH,$(o))) $(JMK_LDFLAGS_$@))
	@$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@

$(JMK_LOCAL_O_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),$(HOST_LD) -r -o $@ $(foreach o,$(filter-out $(JMKE_PWD_SRC)/Makefile,$^), $(call OS_PATH,$(o))) $(JMK_LDFLAGS_$@))

ifdef CONFIG_RG_OS_LINUX

$(JMK_MOD_TARGET):
ifdef CONFIG_RG_OS_LINUX_26
	@# compile kos_26_mod.o for all modules (override RG_COMPILE_FUNC with exact cflags)
	$(if $(JMK_JPKG_TARGET_$@),\
	$(if $(filter $@,$(JMK_MOD_2_STAT) $(JMK_MOD_O_TARGET)),,\
	$(JMKE_LN) $(JMK_ROOT)/pkg/util/kos_26_mod.c $(*F).mod.c && \
	$(TARGET_CC) $(JMK_RG_LINUX_CFLAGS) $(JMK_MOD_CFLAGS) $(MOD_26_CFLAGS) \
	  -DMODULE -c -o $(*F).mod.o $(*F).mod.c))
endif
	$(if $(JMK_JPKG_TARGET_$@), \
	  $(LD) $(ENDIAN_LDFLAGS) -r -o $@ \
	    -Map $@.link.map \
	    $(filter-out $(*F).mod.c,\
	      $(filter-out %.h,\
	        $(filter-out $(JMKE_PWD_SRC)/Makefile,$^)\
	       )\
	     )\
	    $(if $(CONFIG_RG_OS_LINUX_26),\
	      $(if $(filter $@,$(JMK_MOD_2_STAT) $(JMK_MOD_O_TARGET)),,$(*F).mod.o)\
	     ) \
	    $(JMK_LDFLAGS_$@)\
	 )
	@$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@

endif # CONFIG_RG_OS_LINUX

# TODO: Remove the __create_lib_ logic and replace it with a touch on the 
# changed object (make sure it is newer than the archive).
GET_AR=$(if $(filter $1,$(JMK_A_TARGET)),$(AR),$(if $(filter $1,$(JMK_LOCAL_A_TARGET)),$(HOST_AR),$(error RG_MAKEFILE internal error $1)))

$(foreach t,$(A_TARGETS),__create_lib_$t):
	$(if $(JMK_JPKG_TARGET_$@),\
	  $(if $?,$(call GET_AR,$(@:__create_lib_%=%)) crv \
	  $(@:__create_lib_%=%) $?))

$(JMK_A_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),$(RANLIB) $@)
	@$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$(@F)

$(JMK_LOCAL_A_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),$(HOST_RANLIB) $@)

# Note: the -Wl,--no-whole-archive must be the last argument in order to make
# sure that when a so_target add -Wl,--whole-archive, it will not add all libgcc
# and libc into the sheared library

SO_LAST_FLAGS=-Wl,--no-whole-archive -Wl,-Map $@.link.map
SO_NAME=-Wl,-soname=$@
ALL_ARCHIVE=-Wl,--whole-archive
NO_ALL_ARCHIVE=-Wl,--no-whole-archive
#	    $(JMK_SO_CFLAGS_$@) $(if $(findstring libcrypto,$@),$^, $(ALL_ARCHIVE) $(DEBUG_PATH)/$(@F:.so=_pic.a)  $(NO_ALL_ARCHIVE))\

PIC_A_TARGET=$(JMK_SO_TARGET:.so=_pic.a)

$(PIC_A_TARGET):
	$(if $(JMK_JPKG_TARGET_$(@:_pic.a=.so)),\
	      $(if $(filter $(@:_pic.a=.a),$^), \
	          $(JMKE_LN) $(@:_pic.a=.a) $@  && , \
	          $(AR) -rs $@ $^ && \
	          $(RANLIB)  $@  &&\
	       ) \
	       $(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@ , \
	  @echo "PIC_A_TARGET: Not building ($@:$)"\
	) 

$(JMK_SO_TARGET) : lib%.so : lib%_pic.a
	$(if $(JMK_JPKG_TARGET_$@),\
	  touch $@.link.map; \
	    $(CC) -shared -o $@ $(SO_NAME) $(FPIC_FLAG) $(JMK_SO_CFLAGS) \
	    $(JMK_SO_CFLAGS_$@) $(ALL_ARCHIVE) \
	    $(filter-out $(SO_ARCHIVE_REMOVE_$@), $^) $(NO_ALL_ARCHIVE) \
	    $(call FIX_VPATH_LDFLAGS,$(JMK_SO_LDFLAGS) $(JMK_SO_LDFLAGS_$@)) \
	    $(SO_LAST_FLAGS), \
	  @echo "JMK_SO_TARGET: Not building ($@:$)"\
	) 
	@$(JMKE_LN) $(JMKE_PWD_BUILD)/$@ $(DEBUG_PATH)/$@ 

# We have to have static on a local target so that the target will work in a 
# distribution tree on a different computer with different libc version.
$(JMK_LOCAL_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),\
	$(CC_FOR_BUILD) -o $@ $^ $(call FIX_VPATH_LDFLAGS,\
	$(filter-out $(JMK_LDFLAGS_REMOVE_$@),\
	$(JMK_LOCAL_LDFLAGS)) $(JMK_LDFLAGS_$@)) $(filter-out $(JMK_LDLIBS_REMOVE_$@),\
	$(JMK_LOCAL_LDLIBS)) $(JMK_LDLIBS_$@),\
	@echo "JMK_LOCAL_TARGET: Not building ($@:$^)")

$(JMK_LOCAL_CXX_TARGET):
	$(if $(JMK_JPKG_TARGET_$@),\
	$(CXX_FOR_BUILD) -o $@ $^ $(call FIX_VPATH_LDFLAGS,\
	$(filter-out $(JMK_LDFLAGS_REMOVE_$@),$(JMK_LOCAL_LDFLAGS)) $(JMK_LDFLAGS_$@))\
	$(filter-out $(JMK_LDLIBS_REMOVE_$@),$(JMK_LOCAL_LDLIBS)) $(JMK_LDLIBS_$@),\
	@echo "JMK_LOCAL_CXX_TARGET: Not building ($@:$^)")

# This is the default binary distribution rule.
ifdef CONFIG_ULIBC
%.o: %.o.$(TARGET_MACHINE).ulibc
	$(MKDIR) $(dir $@)
	$(JMKE_LN) $(if $(filter /%,$<),,$(CUR_DIR)/)$< $@
endif

ifdef CONFIG_GLIBC
%.o: %.o.$(TARGET_MACHINE).glibc
	$(MKDIR) $(dir $@)
	$(JMKE_LN) $(if $(filter /%,$<),,$(CUR_DIR)/)$< $@
endif

# for locals and modules (no libc distinction)
%.o: %.o.$(TARGET_MACHINE)
	$(MKDIR) $(dir $@)
	$(JMKE_LN) $(if $(filter /%,$<),,$(CUR_DIR)/)$< $@

JSLINT=-process $(JMKE_PWD_BUILD)/$1
jslint:
	$(JSLINT_CMD) -conf $(JMK_ROOT)/pkg/build/jslint.conf $(foreach o,$(JMK_JS_FILES),$(call JSLINT,$o))
endif
