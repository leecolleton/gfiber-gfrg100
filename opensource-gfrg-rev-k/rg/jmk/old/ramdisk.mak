ifeq ($(DEBUG_FLAG),1)
  DEBUG_MAKE=debug
endif

# Automatic dependency.
JMK_CFLAGS+=$(if $(filter %.o,$@),-MMD $(if $(findstring 2.95,$(GCC_VERSION)),,-MP -MF $(@:%.o=%.d) -MT $@))
JMK_MOD_CFLAGS+=$(if $(filter %.o,$@),-MMD $(if $(findstring 2.95,$(GCC_VERSION)),,-MP -MF $(@:%.o=%.d) -MT $@))
JMK_LOCAL_CFLAGS+=$(if $(filter %.o,$@),-MMD -MP -MF $(@:%.o=%.d) -MT $@)
-include *.d

# Generic rule for recursive subdirs:
# (The code is here and not in rg.mak because it is used by linux kernel in
# ramdisk and CD creation)
# Create a variable that contains all subdirs you want using this pattern:
# DIR/ACTION.subdir__ where DIR is the dir name, action is the action name 
# (like clean) and .subdir__ is a fix sufix. Add a depandancy to it.
# This will make -C DIR ACTION.
BUILD_SUB_DIR=$(if $(filter /%,$(dir $@)),,$(CUR_DIR)/)$(dir $@)
MKDIR_SUBDIR=$(MKDIR) $(BUILD_SUB_DIR)
ifdef CONFIG_RG_JPKG
  MKDIR_SUBDIR+= $(BUILD_SUB_DIR:$(JMKE_PWD_BUILD)%=$(PWD_JPKG)%)
endif

BUILD_SUB_DIR_REL=$(subst $(if $(findstring $(JMKE_BUILDDIR),$(BUILD_SUB_DIR)),$(JMKE_BUILDDIR)/,$(JMK_ROOT)/),,$(BUILD_SUB_DIR))

THE_MAKEFILE=$(if $(wildcard $(BUILD_SUB_DIR)/Makefile),$(BUILD_SUB_DIR)/Makefile,$(JMKE_PWD_SRC)/$(dir $@)/Makefile)

IS_JPKG_SUBDIR=$(if $(JMKE_DOING_MAKE_CONFIG),,$(if $(CONFIG_RG_JPKG),$(if $(filter $(JMK_JPKG_EXPORTED_DIR),$(@D)),y,$(if $(CONFIG_RG_JPKG_SRC),$(if $(filter $(JMK_JPKG_EXPORTED_DIR_SRC),$(@D)),y)))))
# if "make config" and the subdir is in the list of JMK_LINK_DIRS then
# create the links
LINK_THE_DIR=$(if $(JMKE_DOING_MAKE_CONFIG),\
  $(if $(filter $(@D),$(JMK_LINK_DIRS)), $(MKDIR) \
  $(dir $(BUILD_SUB_DIR)) && $(JMKE_CP_LN) \
  $(BUILD_SUB_DIR:$(JMKE_BUILDDIR)%=$(JMK_ROOT)%) `dirname $(BUILD_SUB_DIR)` && )) \

%.subdir__:
ifdef JMKE_IS_BUILDDIR
ifdef MAKEFILE_DEBUG
	@echo "PWD_JPKG:$(PWD_JPKG)"
	@echo "CURDIR:$(CUR_DIR)"
	@echo "BUILD_SUB_DIR:$(BUILD_SUB_DIR)"
	@echo "IS_JPKG_SUBDIR:$(if $(IS_JPKG_SUBDIR),YES,NO)"
endif
	$(if $(filter _DUMMY_SUBDIR_%,$@), \
	  , \
	    $(if $(IS_JPKG_SUBDIR), \
	      , \
	        $(if $(wildcard $(THE_MAKEFILE)), \
	         $(LINK_THE_DIR) $(MKDIR_SUBDIR) && $(MAKE) -f $(THE_MAKEFILE) \
	         $(DEBUG_MAKE) $(JMK_RGTV_MAKEFLAGS) -C $(BUILD_SUB_DIR) $(patsubst %.subdir__,%,$(notdir $@)), \
  	         $(if $(CONFIG_RG_GPL), \
	            , \
	            $(error missing Makefile $(THE_MAKEFILE)) \
	         ) \
	       ) \
	    ) \
	 )
else
	@echo "subdir: $(dir $@)"
endif

_DUMMY_SUBDIR_:

# RAMDISK (ramdisk and cramfs) logic
ifndef JMK_RAMDISK_SUBDIRS
  JMK_RAMDISK_SUBDIRS = $(JMK_SUBDIRS)
endif
RAMDISK_SUBDIRS_ = $(JMK_RAMDISK_SUBDIRS:%=%/ramdisk.subdir__)

# Add WBM image files with their full directory name
JMK_RAMDISK_FILES+=$(JMK_RAMDISK_SWF_FILES:%=/home/httpd/html/swf/%)
JMK_RAMDISK_FILES+=$(JMK_RAMDISK_IMG_FILES:%=/home/httpd/html/images/%)
JMK_RAMDISK_FILES+=$(JMK_RAMDISK_JS_FILES:%=/home/httpd/html/%)

# TODO: I add a suffix/prefix to JMK_RAMDISK_FILES so I can make a rule from
# it without clashing with the filename. It can (should?) be changed to
# be target_file: src_file
JMK_RAMDISK_LIB_FILES+=$(addprefix modules/,$(filter-out $(JMK_MOD_2_STAT) $(JMK_MOD_O_TARGET),$(JMK_RAMDISK_MODULES_PERMANENT_FILES)))
RAMDISK_MODULES_FILES_FILTERED=$(filter-out $(JMK_MOD_2_STAT) $(JMK_MOD_O_TARGET),$(JMK_RAMDISK_MODULES_FILES))
RAMDISK_KERNEL_MODULES_FILTERED=$(filter-out $(JMK_MOD_2_STAT) $(JMK_MOD_O_TARGET),$(JMK_RAMDISK_KERNEL_MODULES))

ifdef BOOTLDR_RAMDISK
  JMK_RAMDISK_FILES=
  JMK_RAMDISK_BIN_FILES=$(BOOTLDR_BIN_FILES)
  RAMDISK_MODULES_FILES_FILTERED=$(BOOTLDR_MODULES)
endif

RAMDISK_FILES_=$(sort $(JMK_RAMDISK_FILES:%=__%_rd) \
	       $(JMK_RAMDISK_PLUGINS_FILES:%=__/plugins/%_rd) \
	       $(JMK_RAMDISK_LIB_FILES:%=__/lib/%_rd) \
	       $(JMK_RAMDISK_ETC_FILES:%=__/etc/%_rd) \
	       $(JMK_RAMDISK_BIN_FILES:%=__/bin/%_rd) \
	       $(JMK_RAMDISK_VAR_FILES:%=__/var/%_rd))

RAMDISK_MODULES_FILES_=$(RAMDISK_MODULES_FILES_FILTERED:%=__/lib/modules/%_rd)
RAMDISK_KERNEL_MODULES_=$(RAMDISK_KERNEL_MODULES_FILTERED:%=__kernmod__%_rd)

$(RAMDISK_FILES_):
	@$(strip $(call RAMDISK_CP_RO_FUNC,$(notdir $(@:__%_rd=%)),$(@:__%_rd=%),\
	  $(if $(filter /bin/%,$(@:__%_rd=%)),\
	    $(if $(CONFIG_BINFMT_FLAT),\
	      DONT_STRIP,\
	      STRIP\
	    ),\
	    $(if $(filter /lib/%,$(@:__%_rd=%)),\
	      $(if $(filter /lib/modules/%,$(@:__%_rd=%)),\
	        STRIP_DEBUG_SYM,\
		STRIP\
	      ),\
	      $(if $(filter /etc/%.so,$(@:__%_rd=%)),\
	        STRIP,\
	        DONT_STRIP\
	      )\
	    )\
	  )\
	))

$(RAMDISK_MODULES_FILES_):
	@$(call $(if $(CONFIG_RG_MODFS),RAMDISK_CP_MOD_FUNC,RAMDISK_CP_RO_FUNC),$(notdir $(@:__%_rd=%)),$(@:__%_rd=%),$(if $(filter $(notdir $(@:__%_rd=%)),$(JMK_MODULES_DONT_STRIP)),DONT_STRIP,STRIP_DEBUG_SYM))

#change the module filename from .ko to .o in the ramdisk
# the module file path is relative to the source dir
$(RAMDISK_KERNEL_MODULES_):
	@$(call $(if $(CONFIG_RG_MODFS),RAMDISK_CP_MOD_FUNC,RAMDISK_CP_RO_FUNC),$(@:__kernmod__%_rd=%),/lib/modules/$(basename $(notdir $(@:__kernmod__%_rd=%))).o,STRIP_DEBUG_SYM)

# Note: Kernel is compiling with -j. Can not relay on elemnts order for \
# dependency

JMK_RAMDISK_NORMAL_TASKS := $(RAMDISK_FILES_) $(RAMDISK_MODULES_FILES_) \
    $(RAMDISK_KERNEL_MODULES_) $(RAMDISK_SUBDIRS_)
RAMDISK_ALL_TASKS := $(JMK_RAMDISK_NORMAL_TASKS) $(JMK_RAMDISK_FIRST_TASKS) \
    $(JMK_RAMDISK_LAST_TASKS)

$(JMK_RAMDISK_NORMAL_TASKS): $(JMK_RAMDISK_FIRST_TASKS)
$(JMK_RAMDISK_LAST_TASKS): $(JMK_RAMDISK_NORMAL_TASKS)

ramdisk:: $(RAMDISK_ALL_TASKS)

# If the executable is not already stripped, strip it
ifndef CONFIG_BINFMT_FLAT
  ramdisk:: $(RAMDISK_STRIPPED_FILES_)
endif

dummy:

.PHONY: dummy ramdisk
