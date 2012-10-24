# handle user pre-defined BUILD directory: set JMKE_BUILDDIR accordingly
ifdef BUILD
  export BUILD
  JMKE_BUILDDIR:=$(if $(filter /%,$(BUILD)),,$(shell cd $(JMK_ROOT); pwd)/)$(BUILD)
endif

# handle JMKE_BUILDDIR
ifdef JMKE_BUILDDIR
  # we are already in JMKE_BUILDDIR, or the user specified a specific BUILD dir
  JMKE_IS_BUILDDIR:=$(if $(findstring $(JMKE_BUILDDIR),$(CUR_DIR)),y)
  ifndef JMKE_IS_BUILDDIR
    CREATE_BUILDDIR_LINK:=y
  endif
else
  # we are in the initial make source directory, we need to move
  # to JMKE_BUILDDIR and respawn make
  JMKE_IS_BUILDDIR:=$(if $(findstring $(JMK_ROOT)/build,$(CUR_DIR)),y)
  ifndef JMKE_IS_BUILDDIR
    ifdef JMKE_DOING_MAKE_CONFIG
      ifeq ($(CUR_DIR),$(JMK_ROOT))
        CREATE_BUILDDIR_LINK:=y
        JMKE_BUILDDIR:=$(JMK_ROOT)/build.$(DIST)
      endif
    endif
  endif
endif

BUILDDIR_LINK:=$(JMK_ROOT)/build

CREATE_BUILDDIR_LINK_=$(shell mkdir -p $1; ln -sfn $1 $(BUILDDIR_LINK))
ifdef CREATE_BUILDDIR_LINK
    $(call CREATE_BUILDDIR_LINK_,$(JMKE_BUILDDIR))
else
  ifndef JMKE_BUILDDIR
    ifneq ($(wildcard $(BUILDDIR_LINK)),)
      JMKE_BUILDDIR:=$(shell readlink $(BUILDDIR_LINK))
    else
      ifeq ($(DIST),)
        $(error DIST undefined (Cannot find $(BUILDDIR_LINK)). Run make config)
      else
        $(error Cannot find $(BUILDDIR_LINK). Run make config)
      endif
    endif
  endif
endif

JPKG_DIR=$(JMKE_BUILDDIR)/jpkg/rg

export JMK_ROOT:=$(if $(filter /%,$(JMK_ROOT)),$(JMK_ROOT),$(shell cd $(JMK_ROOT) && pwd))
export JMKE_PWD_SRC:=$(if $(JMKE_IS_BUILDDIR),$(CUR_DIR:$(JMKE_BUILDDIR)%=$(JMK_ROOT)%),$(CUR_DIR))
export JMKE_PWD_BUILD:=$(if $(JMKE_IS_BUILDDIR),$(CUR_DIR),$(CUR_DIR:$(JMK_ROOT)%=$(JMKE_BUILDDIR)%))
export PWD_JPKG:=$(CUR_DIR:$(if $(JMKE_IS_BUILDDIR),$(JMKE_BUILDDIR),$(JMK_ROOT))%=$(JPKG_DIR)%)

BUILD2SRC=$(if $(findstring $(JMKE_BUILDDIR),$1), \
  $(subst $(JMKE_BUILDDIR),$(JMK_ROOT),$1), \
  $(if $(filter /%,$1)$(findstring $(JMK_ROOT),$1), \
    $1, \
    $(JMKE_PWD_SRC)/$1))

# begin by searching for JMKE_BUILDDIR, since it usually contains JMK_ROOT as substring
SRC2BUILD_ALLOW_RELATIVE=$(strip \
  $(if $(findstring $(JMKE_BUILDDIR),$1),\
    $1,\
    $(if $(findstring $(JMK_ROOT),$1),\
      $(subst $(JMK_ROOT),$(JMKE_BUILDDIR),$1),\
      $1\
    )\
  )\
)

SRC2BUILD=$(strip \
  $(if $(filter /%,$(call SRC2BUILD_ALLOW_RELATIVE,$1)),\
    $(call SRC2BUILD_ALLOW_RELATIVE,$1),\
    $(JMKE_PWD_BUILD)/$(call SRC2BUILD_ALLOW_RELATIVE,$1)\
  )\
)

export JMKE_BUILDDIR

ifndef JMKE_IS_BUILDDIR
  # we are in the source directory. we need to move to the build directory
  all $(filter-out echovar distclean, $(MAKECMDGOALS)): cd_to_builddir

  # This is the catch-all target for the SRCDIR
  # we need it since the real target can only be built in JMKE_BUILDDIR and we still
  # need to do something in SRCDIR
  %:;

endif

# In the source dir we only want to change to JMKE_BUILDDIR
cd_to_builddir: $(CHECK_CONFIGURE)
ifdef MAKEDEBUG
	@echo "PWD_JPKG: $(PWD_JPKG)"
	@echo "JMKE_PWD_BUILD: $(JMKE_PWD_BUILD)"
	@echo "MAKECMDGOALS: $(MAKECMDGOALS)"
endif
	$(if $(filter /%,$(JMK_ROOT)),,$(error Tree is not configured))
	umask 022 && \
	mkdir -p $(JMKE_PWD_BUILD) $(if $(CONFIG_RG_JPKG),$(PWD_JPKG)) && \
	$(MAKE) -C $(JMKE_PWD_BUILD) -f $(CUR_DIR)/Makefile \
	  $(MAKECMDGOALS) JMK_ROOT=$(JMK_ROOT)

.PHONY: cd_to_builddir
