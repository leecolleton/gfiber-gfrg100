JMK_ROOT:=$(realpath $(JMK_ROOT))

# handle user pre-defined BUILD directory: set JMKE_BUILDDIR accordingly
ifdef BUILD
  JMKE_BUILDDIR:=$(if $(filter /%,$(BUILD)),,$(JMK_ROOT)/)$(BUILD)
else
  JMKE_BUILDDIR:=
endif

# handle JMKE_BUILDDIR
ifdef JMKE_BUILDDIR
  CREATE_BUILDDIR_LINK=y
else
  ifdef JMKE_DOING_MAKE_CONFIG
    ifeq ($(CUR_DIR),$(JMK_ROOT))
      CREATE_BUILDDIR_LINK=y
      JMKE_BUILDDIR:=$(JMK_ROOT)/build.$(DIST)
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

export JMKE_BUILDDIR
JPKG_DIR=$(JMKE_BUILDDIR)/jpkg/rg

BUILD2SRC=$(1:$(JMKE_BUILDDIR)%=$(JMK_ROOT)%)

# begin by searching for JMKE_BUILDDIR, since it usually contains JMK_ROOT as
# substring
SRC2BUILD=$(strip \
  $(if $(findstring $(JMKE_BUILDDIR),$1), \
    $1, \
    $(1:$(JMK_ROOT)%=$(JMKE_BUILDDIR)%)))

Abs_Path_To_Jpkg=$(addprefix $(JPKG_DIR)/,$(call Abs_Path_To_Rel,$1))

# TODO FIXME - temporary. until makefile conversion is done, we may be called
# from source dir or from build dir.
JMKE_IS_BUILDDIR:=$(if $(findstring $(JMKE_BUILDDIR),$(CUR_DIR)),y)
