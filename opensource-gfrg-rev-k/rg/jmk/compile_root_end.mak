# $1 - src
_IS_CPP_FILE=$(call DBGFR, \
  $(filter %.cpp %.cxx %.cc,$1) \
  ,_IS_CPP_FILE,$1)

# $1 - existing flags
# $2 - obj name
# $3 - src
# - For .c files, the target compiler is always rg_gcc, but for .cpp files we 
#   have to decide ourselves
# - In glibc, both CXX and TARGET_CXX point to the toolchain compiler. If
#   ulibc is also set, then CXX is set to rg_gcc but TARGET_CXX keeps its value
_GET_COMPILER=$(call DBGFR, \
  $(if $(is_local_obj_$(call Abs_Path_To_Var,$2)), \
    $(CC_FOR_BUILD), \
    $(if $(is_mod_obj_$(call Abs_Path_To_Var,$2)), \
      $(TARGET_CC), \
      $(if $(call _IS_CPP_FILE,$3), \
        $(if $(call _IS_COMPILING_ULIBC,$1), \
          $(CXX), \
  	  $(TARGET_CXX) \
        ), \
        $(CC) \
      ) \
    ) \
  ) \
  ,_GET_COMPILER,$1,$2,$3)

# Compilation functions for internal use only
# Usage: $(call RG_COMPILE_FUNC,EXTRA_FLAGS,output,src)
_RG_COMPILE_FUNC=$(strip \
  $(call _GET_COMPILER,$1,$2,$3) \
  $(call GET_OBJ_CFLAGS,$1,$2,$3) -c -o $2 \
  $(filter-out %/Makefile,$3))

# JPKG logic: add dependeny between the source file and its copy in the jpkg
# directory.
# $1 - source file
Add_Export_To_Jpkg_Rule= \
  $(if $(filter $1,$(sav_JMK_DONT_EXPORT)), \
    $(info "Not exporting SRC $1"), \
    $(eval $(1:$(JMK_ROOT)/%=$(JPKG_DIR)/%):$1; \
      $(RG_VPATH_CP) $1 $(1:$(JMK_ROOT)/%=$(JPKG_DIR)/%)) \
    $(eval export_to_jpkg: $(1:$(JMK_ROOT)/%=$(JPKG_DIR)/%)))

ifndef CONFIG_RG_JPKG
  RG_COMPILE_FUNC=$(call _RG_COMPILE_FUNC,$1,$2,$3)
endif
ifdef CONFIG_RG_JPKG_SRC
  RG_COMPILE_FUNC=$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$2)), \
    $(call _RG_COMPILE_FUNC,$1,$2,$3))
endif

ifdef CONFIG_RG_JPKG_BIN
  JPKG_SOURCES:=$(sav_JMK_EXPORT_AS_SRC) $(filter %.c,$(export_headers_srcs))

  # $1 - GLIBC or ULIBC
  # $2 - obj name
  # - if (JMK_JPKG_BIN_LIBCS_obj is set), then compile just for the specified
  #   libc.
  # - Otherwise, compile both ulibc and glibc.
  _NEEDS_TO_COMPILE_FOR=$(call DBGFR, \
    $(if $(CONFIG_$1), \
      $(if $(sav_JMK_JPKG_BIN_LIBCS_$(call Abs_Path_To_Var,$2)), \
	$(filter $1,$(sav_JMK_JPKG_BIN_LIBCS_$(call Abs_Path_To_Var,$2))), \
	yes_please)) \
  ,_NEEDS_TO_COMPILE_FOR,$1,$2)

  # _RG_COMPILE_AND_EXPORT: compiles and export using the default libc
  #
  # - if JMK_JPKG_TARGET_ is set, then the obj was already compiled in 
  #   RG_COMPILE_FUNC, so no need to compile again
  _RG_COMPILE_AND_EXPORT=$(call DBGFR, \
    $(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$2)), \
      , \
      $(call _RG_COMPILE_FUNC,$1,$2,$3) && \
    ) \
    $(RG_VPATH_CP) $2 $(2:$(JMKE_BUILDDIR)/%=$(JPKG_DIR)/%).$(TARGET_MACHINE) \
  ,_RG_COMPILE_AND_EXPORT,$1,$2,$3)

  _RG_COMPILE_AND_EXPORT_BOTH_LIBCS=$(call DBGFR, \
    $(if $(call _NEEDS_TO_COMPILE_FOR,ULIBC,$2), \
      $(call _RG_COMPILE_FUNC,$(_ULIBC_FLAG) $1,$2,$3) && \
      $(JMKE_CP) $2 $2.ulibc && \
      $(RG_VPATH_CP) $2.ulibc \
        $(2:$(JMKE_BUILDDIR)/%=$(JPKG_DIR)/%).$(TARGET_MACHINE).ulibc && \
    ) \
    $(if $(call _NEEDS_TO_COMPILE_FOR,GLIBC,$2), \
      $(call _RG_COMPILE_FUNC,$(_GLIBC_FLAG) $1,$2,$3) && \
      $(JMKE_CP) $2 $2.glibc && \
      $(RG_VPATH_CP) $2.glibc \
        $(2:$(JMKE_BUILDDIR)/%=$(JPKG_DIR)/%).$(TARGET_MACHINE).glibc && \
    ) \
    true \
  ,_RG_COMPILE_AND_EXPORT_BOTH_LIBCS,$1,$2,$3)

  RG_COMPILE_FUNC=$(call DBGFR, \
    $(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$2)), \
      $(call _RG_COMPILE_FUNC,$1,$2,$3) && \
    ) \
    $(if $(filter $(JPKG_SOURCES),$3),true, \
      $(if $(filter $(sav_JMK_DONT_EXPORT),$2 \
	$(dir $2)/$(patsubst local_%.o,%.o,$(notdir $2)) $(2:%_pic.o=%.o)), \
        echo "Not exporting BIN $2", \
	$(if $(is_local_obj_$(call Abs_Path_To_Var,$2)), \
	  $(call _RG_COMPILE_AND_EXPORT,$1,$2,$3), \
	  $(if $(is_mod_obj_$(call Abs_Path_To_Var,$2)), \
	    $(call _RG_COMPILE_AND_EXPORT,$1,$2,$3), \
	    $(call _RG_COMPILE_AND_EXPORT_BOTH_LIBCS,$1,$2,$3) \
	  ) \
	) \
      ) \
    ) \
  ,RG_COMPILE_FUNC,$1,$2,$3)
endif

FILTER_SRC=$(filter %.c %.S %.s %.cpp %.cxx,$1)

_Get_Src = \
  $(or $(filter $1,$(sav_JMK_AUTOGEN_SRC)), \
    $(wildcard $(1:$(JMKE_BUILDDIR)/%=$(JMK_ROOT)/%)))

# search for source file in build-dir or source-dir
# $1 - the object name without any prefixes or suffixes
Get_Src = \
  $(foreach s,c cpp, \
    $(call _Get_Src,$(1:%.o=%.$s)))

# add compilation rule
# $1 - target
# $2 - source
# $3 - extra flags
# NOTE: We might want to think about performance here...
Add_Compile_Rule=$(eval $1: $2 ; $$(call RG_COMPILE_FUNC,$3,$1,$2)) \
  $(if $(filter-out $(jpkg_exported_rest) $(jpkg_exported),$2), \
    $(eval jpkg_exported_rest+=$2) \
    $(if $(CONFIG_RG_JPKG_SRC),$(call Add_Export_To_Jpkg_Rule,$2)) \
    $(if $(CONFIG_RG_JPKG_BIN),$(if $(filter $(JPKG_SOURCES),$2), \
      $(call Add_Export_To_Jpkg_Rule,$2))))

# replace the suffix of the file and return true if the file exists.
# $1 - file
# $2 - a string in the form "suffix=replacement", for example, .o=.c
Wildcard_Src=$(and $(wildcard $(1:$2)),$(filter-out $1, $(1:$2)))

# compute the source of the object.
# We look for the source file in source tree and in the buildder.
# We also look for the source file in the list of auto-generted files.
# $1 - target
Get_Obj_Source=\
  $(eval O_ROOT_PATH:=$(call BUILD2SRC,$1)) \
  $(if $(filter $(ALL_LOCAL_OBJS),$1), \
    $(or \
      $(wildcard $(O_ROOT_PATH).$(TARGET_MACHINE)*), \
      $(call Get_Src,$(dir $1)$(patsubst local_%,%,$(notdir $1)))), \
    $(or \
      $(call Wildcard_Src,$(O_ROOT_PATH),.o=.c), \
      $(call Wildcard_Src,$1,.o=.c), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_24.o=.c), \
      $(call Wildcard_Src,$1,_mod_24.o=.c), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_26.o=.c), \
      $(call Wildcard_Src,$1,_mod_26.o=.c), \
      $(call Wildcard_Src,$(O_ROOT_PATH),.o=.cpp), \
      $(call Wildcard_Src,$1:.o=.cpp), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_24.o=.cpp), \
      $(call Wildcard_Src,$1,_mod_24.o=.cpp), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_26.o=.cpp), \
      $(call Wildcard_Src,$1,_mod_26.o=.cpp), \
      $(call Wildcard_Src,$(O_ROOT_PATH),.o=.cxx), \
      $(call Wildcard_Src,$1,.o=.cxx), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_24.o=.cxx), \
      $(call Wildcard_Src,$1,_mod_24.o=.cxx), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_26.o=.cxx), \
      $(call Wildcard_Src,$o,_mod_26.o=.cxx), \
      $(call Wildcard_Src,$(O_ROOT_PATH),.o=.cc), \
      $(call Wildcard_Src,$1,.o=.cc), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_24.o=.cc), \
      $(call Wildcard_Src,$1,_mod_24.o=.cc), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_26.o=.cc), \
      $(call Wildcard_Src,$1,_mod_26.o=.cc), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_pic.o=.c), \
      $(call Wildcard_Src,$1,_pic.o=.c), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_pic.o=.cpp), \
      $(call Wildcard_Src,$1,_pic.o=.cpp), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_pic.o=.cxx), \
      $(call Wildcard_Src,$1,_pic.o=.cxx), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_pic.o=.cc), \
      $(call Wildcard_Src,$1,_pic.o=.cc), \
      $(call Wildcard_Src,$(O_ROOT_PATH),.o=.S), \
      $(call Wildcard_Src,$1,.o=.S), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_24.o=.S), \
      $(call Wildcard_Src,$1,_mod_24.o=.S), \
      $(call Wildcard_Src,$(O_ROOT_PATH),_mod_26.o=.S), \
      $(call Wildcard_Src,$1,_mod_26.o=.S), \
      $(wildcard $(O_ROOT_PATH).$(TARGET_MACHINE)*), \
      $(filter $(1:.o=.c) $(1:_mod_24.o=.c) $(1:_mod_26.o=.c) $(1:.o=.cpp) \
        $(1:_mod_24.o=.cpp) $(1:_mod_26.o=.cpp) $(1:.o=.cxx) \
	$(1:_mod_24.o=.cxx) $(1:_mod_26.o=.cxx) $(1:.o=.cc) $(1:_mod_24.o=.cc) \
	$(1:_mod_26.o=.cc) $(o:_pic.o=.c) $(1:_pic.o=.cpp) $(1:_pic.o=.cxx) \
	$(1:_pic.o=.cc) $(1:.o=.S) $(1:_mod_24.o=.S) $(1:_mod_26.o=.S), \
	$(sav_JMK_AUTOGEN_SRC))))

# for all the ojects, add a rule in the form:
#  <target_file> : <source_file>
#   	$(call RG_COMPILE_FUNC,<extra_flags>,<target_file>,<source_file>)
# Note: In the distribution tree, we might have objects in the form
# *.o.i386-jungo-linux-gnu[.glibc|.ulibc|]. In that case we don't want to
# compile the object. Instead, we will use the rule:
# $(JMKE_BUILDDIR)/%.o: $(JMK_ROOT)/%.o.$(TARGET_MACHINE)
# or similar
$(foreach o, \
  $(filter-out $(sav_JMK_LOCAL_O_TARGET) $(sav_JMK_O_TARGET),$(ALL_OBJS)), \
    $(eval SRC_OBJ:=$(call Get_Obj_Source, $o)) \
    $(if $(filter $(call BUILD2SRC,$o).$(TARGET_MACHINE)%,$(SRC_OBJ)),, \
      $(eval EXTRA_FLAGS=) \
      $(if $(is_pic_obj_$(call Abs_Path_To_Var,$o)), \
        $(eval EXTRA_FLAGS:=$(FPIC_FLAG))) \
      $(if $(filter %.S,$(SRC_OBJ)), \
        $(eval EXTRA_FLAGS:=-D__ASSEMBLY__)) \
      $(call Add_Compile_Rule,$o,$(SRC_OBJ),$(EXTRA_FLAGS))))

# dependency is defined in general_dep.mak
$(sav_JMK_sO_OBJS):
	$(call RG_COMPILE_FUNC, \
	  $(sav_JMK_sOFLAGS_$(call Abs_Path_To_Dir_Var,$@)) \
	  -x assembler-with-cpp,$@,$<)

# dependencies are defined by the Makefile
$(sav_JMK_OTHER_OBJS):
	$(call RG_COMPILE_FUNC,,$@,$(call FILTER_SRC,$(JMKE_DEPS)))

$(JMKE_BUILDDIR)/%.c: $(JMK_ROOT)/%.y
	$(YACC) $(sav_JMK_YFLAGS_$(call Abs_Path_To_Dir_Var,$@)) $< -o $@
$(JMKE_BUILDDIR)/%.c: $(JMKE_BUILDDIR)/%.y
	$(YACC) $(sav_JMK_YFLAGS_$(call Abs_Path_To_Dir_Var,$@)) $< -o $@

$(JMKE_BUILDDIR)/%.c: $(JMK_ROOT)/%.l
	$(LEX) $(sav_JMK_LEXFLAGS_$(call Abs_Path_To_Dir_Var,$@)) -t $< > $@
$(JMKE_BUILDDIR)/%.c: $(JMKE_BUILDDIR)/%.l
	$(LEX) $(sav_JMK_LEXFLAGS_$(call Abs_Path_To_Dir_Var,$@)) -t $< > $@

$(sav_JMK_O_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(LD) $(ENDIAN_LDFLAGS) -r -o $@ \
	    $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@)))

$(sav_JMK_LOCAL_O_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(HOST_LD) -r -o $@ \
	    $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@)))

ifdef CONFIG_RG_OS_LINUX

$(sav_JMK_MOD_TARGET):
ifdef CONFIG_RG_OS_LINUX_26
	@# compile kos_26_mod.o for all modules (override RG_COMPILE_FUNC with exact cflags)
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(if $(filter $@,$(sav_JMK_MOD_2_STAT) $(sav_JMK_MOD_O_TARGET)),, \
	    $(JMKE_LN) $(JMK_ROOT)/pkg/util/kos_26_mod.c $(basename $@).mod.c && \
	    $(TARGET_CC) \
	      $(sav_JMK_RG_LINUX_CFLAGS_$(call Abs_Path_To_Dir_Var,$@)) \
	      $(sav_JMK_MOD_CFLAGS_$(call Abs_Path_To_Dir_Var,$@)) \
	      $(call GET_MOD_26_CFLAGS, $@) -DMODULE -c \
	      -o $(basename $@).mod.o $(basename $@).mod.c))
endif
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(LD) $(ENDIAN_LDFLAGS) -r -o $@ -Map $@.link.map \
	    $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(_OX_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	    $(if $(CONFIG_RG_OS_LINUX_26), \
	      $(if $(filter $@,$(sav_JMK_MOD_2_STAT) $(sav_JMK_MOD_O_TARGET)),,\
		$(basename $@).mod.o) \
	     ) \
	    $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@)) \
	 )
	@$(JMKE_LN) $@ $(DEBUG_PATH)/$(@F)

endif # CONFIG_RG_OS_LINUX

GET_AR=$(if $(filter $1,$(sav_JMK_A_TARGET)),$(AR), \
  $(if $(filter $1,$(sav_JMK_LOCAL_A_TARGET)),$(HOST_AR), \
    $(error RG_MAKEFILE internal error $1)))

$(A_TARGETS):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(if $(JMKE_NEW_DEPS_WITH_LIBS), \
	    $(call GET_AR,$@) crv $@ $(JMKE_NEW_DEPS_WITH_LIBS);) \
	  $(if $(filter $@,$(sav_JMK_A_TARGET)), \
	    $(RANLIB) $@, \
	    $(HOST_RANLIB) $@) \
	)
	$(if $(filter $@,$(sav_JMK_A_TARGET)), \
	  $(JMKE_LN) $@ $(DEBUG_PATH)/$(@F))

# Note: the -Wl,--no-whole-archive must be the last argument in order to make
# sure that when a so_target add -Wl,--whole-archive, it will not add all libgcc
# and libc into the sheared library

SO_LAST_FLAGS=-Wl,--no-whole-archive -Wl,-Map $@.link.map
SO_NAME=-Wl,-soname=$@
ALL_ARCHIVE=-Wl,--whole-archive
NO_ALL_ARCHIVE=-Wl,--no-whole-archive

PIC_A_TARGET:=$(sav_JMK_SO_TARGET:.so=_pic.a)

$(PIC_A_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$(@:_pic.a=.so))), \
	    $(if $(filter $(@:_pic.a=.a),$(JMKE_DEPS_WITH_LIBS)), \
	          $(JMKE_LN) $(@:_pic.a=.a) $@ && , \
	          $(AR) -rs $@ $(JMKE_DEPS_WITH_LIBS) && \
	          $(RANLIB) $@ && \
	       ) \
	       $(JMKE_LN) $@ $(DEBUG_PATH)/$(@F) , \
	  @echo "PIC_A_TARGET: Not building ($@:$)" \
	) 

$(sav_JMK_SO_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  touch $@.link.map; \
	  $(CC) -shared -o $@ $(SO_NAME) $(FPIC_FLAG) \
	    $(sav_JMK_SO_CFLAGS_$(call Abs_Path_To_Var,$@)) \
	    $(ALL_ARCHIVE) \
	    $(filter-out $(SO_ARCHIVE_REMOVE_$(call Abs_Path_To_Var,$@)), \
	      $(JMKE_DEPS_WITH_LIBS)) \
	    $(NO_ALL_ARCHIVE) \
	    $(sav_JMK_SO_LDFLAGS_$(call Abs_Path_To_Var,$@)) $(SO_LAST_FLAGS), \
	  @echo "JMK_SO_TARGET: Not building ($@:$)" \
	) 
	@$(JMKE_LN) $@ $(DEBUG_PATH)/$(@F) 

# $1 - dir
# $2 - file or __local_<name>
Get_Lib_Flags = \
  $(if $(filter __local_%,$2), \
    -l$(2:__local_%=%), \
    -l$(basename $(2:lib%=%)) -L$1)

# Get libraries and according -L flags
# $1 - list of libraries
Get_Libs_Flags = \
  $(foreach l,$1,$(call Get_Lib_Flags,$(dir $l),$(notdir $l)))

# We have to have static on a local target so that the target will work in a 
# distribution tree on a different computer with different libc version.
$(sav_JMK_LOCAL_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(CC_FOR_BUILD) -o $@ $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(filter-out $(sav_JMK_LDFLAGS_REMOVE_$(call Abs_Path_To_Var,$@)), \
	    $(sav_JMK_LOCAL_LDFLAGS_$(call Abs_Path_To_Dir_Var,$@))) \
	  $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@)) \
	  $(call Get_Libs_Flags,$(sav_JMK_LOCAL_LIBS_$(call Abs_Path_To_Var,$@))) \
	  $(call Get_Libs_Flags,$(sav_JMK_LIBS_$(call Abs_Path_To_Var,$@))), \
	  @echo "JMK_LOCAL_TARGET: Not building ($@:$(JMKE_DEPS))")

$(sav_JMK_LOCAL_CXX_TARGET):
	$(if $(sav_JMK_JPKG_TARGET_$(call Abs_Path_To_Var,$@)), \
	  $(CXX_FOR_BUILD) -o $@ $(_O_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(_L_OBJS_$(call Abs_Path_To_Var,$@)) \
	  $(filter-out $(sav_JMK_LDFLAGS_REMOVE_$(call Abs_Path_To_Var,$@)),\
	    $(sav_JMK_LOCAL_LDFLAGS_$(call Abs_Path_To_Dir_Var,$@))) \
	  $(sav_JMK_LDFLAGS_$(call Abs_Path_To_Var,$@)) \
	  $(call Get_Libs_Flags,$(sav_JMK_LOCAL_LIBS_$(call Abs_Path_To_Var,$@))) \
	  $(call Get_Libs_Flags,$(sav_JMK_LIBS_$(call Abs_Path_To_Var,$@))), \
	  @echo "JMK_LOCAL_CXX_TARGET: Not building ($@:$(JMKE_DEPS))")

# This is the default binary distribution rule.
ifdef CONFIG_ULIBC
$(JMKE_BUILDDIR)/%.o: $(JMK_ROOT)/%.o.$(TARGET_MACHINE).ulibc
	$(JMKE_MKDIR) $(dir $@)
	$(JMKE_LN) $< $@
endif

ifdef CONFIG_GLIBC
$(JMKE_BUILDDIR)/%.o: $(JMK_ROOT)/%.o.$(TARGET_MACHINE).glibc
	$(JMKE_MKDIR) $(dir $@)
	$(JMKE_LN) $< $@
endif

# for locals and modules (no libc distinction)
$(JMKE_BUILDDIR)/%.o: $(JMK_ROOT)/%.o.$(TARGET_MACHINE)
	$(JMKE_MKDIR) $(dir $@)
	$(JMKE_LN) $< $@

$(JMKE_PWD_BUILD)/.jslint:
	$(JSLINT_CMD) -conf $(JMK_ROOT)/pkg/build/jslint.conf \
	  $(foreach o,$(sav_JMK_JS_FILES),-process $(call SRC2BUILD,$o))
	@touch $@

ALL_CLEAN_FILES+=$(JMKE_PWD_BUILD)/.jslint
