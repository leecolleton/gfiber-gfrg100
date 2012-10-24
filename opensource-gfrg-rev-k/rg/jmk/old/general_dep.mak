# This file creates all the dependencies between variable contents and should
# only be included from rg.mak.

# Create the $(JMK_TARGET) logic
ifdef BINFRMT_FILE
  include $(JMK_ROOT)/jmk/old/$(BINFRMT_FILE)
endif

DEP_OBJS=$(_O_OBJS_$1) $(_OX_OBJS_$1) $(if $(JMK_JPKG_TARGET_$1),$(_L_OBJS_$1))

ifdef VPATH
  GET_SRC=$(firstword $(wildcard $1.S) $(wildcard $(VPATH)/$1.S) \
    $(wildcard $1.cpp) $(wildcard $(VPATH)/$1.cpp) $(wildcard $1.cc) \
    $(wildcard $(VPATH)/$1.cc) $(wildcard $(VPATH)/$1.c) $(wildcard $1.c))
else
  GET_SRC=$(firstword $(wildcard $1.S) $(wildcard $1.cpp) $(wildcard $1.cc) \
    $(wildcard $1.c))
endif

ifdef NORMAL_TARGETS
  $(foreach t,$(filter-out $(JMK_A_TARGET),$(NORMAL_TARGETS)),\
      $(eval $(t:.so=_pic.a): $(call DEP_OBJS,$t)))
endif

ifdef JMK_MOD_TARGET
  ifdef CONFIG_RG_OS_LINUX_24
    $(foreach t,\
	$(filter %_mod_24.o,\
	    $(foreach m,$(JMK_MOD_TARGET),$(_O_OBJS_$m) $(_OX_OBJS_$m))),\
	$(eval $t: $(call GET_SRC,$(t:%_mod_24.o=%))))
  endif
  
  ifdef CONFIG_RG_OS_LINUX_26
    $(foreach t,\
	$(filter %_mod_26.o,\
	    $(foreach m,$(JMK_MOD_TARGET),$(_O_OBJS_$m) $(_OX_OBJS_$m))),\
	$(eval $t: $(call GET_SRC,$(t:%_mod_26.o=%))))
  endif
endif

ifdef A_TARGETS
  $(foreach t,$(A_TARGETS),$(eval \
      __create_lib_$t: $(_O_OBJS_$t) $(if $(JMK_JPKG_TARGET_$t),$(_L_OBJS_$t))) \
      $(eval $t: __create_lib_$t))
endif

ifneq ($(JMK_LOCAL_TARGET)$(JMK_LOCAL_O_TARGET)$(JMK_LOCAL_CXX_TARGET),)
  $(foreach t,$(JMK_LOCAL_TARGET) $(JMK_LOCAL_O_TARGET) $(JMK_LOCAL_CXX_TARGET),\
      $(eval $t: $(call DEP_OBJS,$t)))
endif

# Adding the special objects for JMK_O_TARGET.
ifdef JMK_O_TARGET
  # Warning: JMK_sO_OBJS and JMK_FAST_RAM_OBJS aren't for multiple targets!
  $(foreach t,$(JMK_O_TARGET),\
      $(eval $t: $(JMK_sO_OBJS) $(JMK_FAST_RAM_OBJS)))
endif

ifdef JMK_sO_OBJS
  # It seems like this could be done by '$(JMK_sO_OBJS): %.o : %.s' BUT:
  # (1) We will have a problem when running in a binary distribution.
  # (2) We can't just use '%.o : %.s' because usually %.s files stand
  # for assembly files that should NOT go through precompilation and we 
  # want to precompile only if the %.s file is in the JMK_sO_OBJS variable. 
  # So, the solution is to use a recursive dependency and '$(JMK_sO_OBJS) : '
  # instead (see rg.mak)
  $(foreach o,$(JMK_sO_OBJS),\
      $(eval $o: $(o:%.o=%.s)))
endif

# All compiled objs depend on the makefile that creates them
ifdef ALL_OBJS
  $(foreach o,$(ALL_OBJS),\
      $(eval $o: $(wildcard $(JMKE_PWD_SRC)/Makefile)))
endif

# DUMMY_VAR is used when we want to force make to calculate a $(),
# usually in case we want to $(error ....).
ALL_DEPS+=$(DUMMY_VAR) export_headers ln_internal_headers $(JMK_FIRST_TASKS) $(JMK_OTHER_DEPS) $(SUBDIRS_)

ifdef CONFIG_RG_JPKG_SRC
  ALL_DEPS+=$(ALL_OBJS) $(JMK_LOCAL_A_TARGET) $(JMK_LOCAL_O_TARGET) $(JMK_LOCAL_TARGET)
else
  ifdef CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY
    ALL_DEPS+=$(ALL_LOCAL_OBJS) $(JMK_LOCAL_A_TARGET) $(JMK_LOCAL_O_TARGET) $(JMK_LOCAL_TARGET) $(JMK_LOCAL_ONLY_LAST_TASKS)
  else
    ALL_DEPS+= $(JMK_OTHER_TARGETS) $(JMK_LOCAL_A_TARGET) $(JMK_LOCAL_O_TARGET) \
      $(JMK_LOCAL_TARGET) $(JMK_LOCAL_CXX_TARGET) $(JMK_OTHER_TASKS) $(JMK_O_TARGET) \
      $(JMK_MOD_TARGET) $(MOD_2_STAT_LINKS) $(JMK_A_TARGET) $(JMK_SO_TARGET) $(JMK_TARGET) \
      $(JMK_LAST_TASKS)
  endif
endif

ifdef CONFIG_RG_JPKG
  ALL_DEPS+=$(JMK_JPKG_FIRST_TASKS) export_to_jpkg $(JMK_JPKG_LAST_TASKS)
endif

ifdef CONFIG_RG_JSLINT
  ifneq ($(JMK_JS_FILES),)
    ifndef CONFIG_RG_JPKG_SRC
      ALL_DEPS+=jslint
    endif
  endif
endif
    
# One all rule to rule them all: Should be only in this file!
# BAD:
#     all: my_rule
# GOOD:
#     $(JMK_OTHER_TASKS)+=my_rule
ifdef JMKE_IS_BUILDDIR
$(JMK_OTHER_TASKS): $(SUBDIRS_)

all: $(ALL_DEPS)
endif
