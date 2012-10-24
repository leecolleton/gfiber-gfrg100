CREATE_CONFIG=$(JMKE_BUILDDIR)/pkg/build/create_config

# TODO:
# in 'make config <config flags...>' we actually run at the end
# 'create_config <config flags...>', with some modifications:
# JMK_ROOT=xxx VPATH=xxx: not passed
# DIST=xxx: passed as -d xxx
# OS=xxx: passed as -o xxx
# HW=xxx: passed as -h xxx
# XXX=yyy: passwd as -f XXX=yyy
# -d -s and all other flags without '=': not passed
#
# We need to change 'create_config' to accept the flags in the same format
# as 'make config', thus - simplifing the Makefile (below), and handling
# or ignoring the arguments in the C code of create_config.
#
# Remove MAKEFLAGS that are not of the type CONFIG_SOMETHING=value
CONFIG_FLAGS_FIX=$(foreach o,$(filter-out debug_% JMK_ROOT=$(JMK_ROOT) VPATH=$(VPATH),$1),$(if $(findstring =,$(o)),$(o),))
MAKEFLAGS_CYGWIN_FIX=$(filter-out " --unix -- ", $(MAKEFLAGS) $(EXTRA_MAKEFLAGS))
UNSPACED_MAKEFLAGS=$(subst \ ,^^^,$(MAKEFLAGS_CYGWIN_FIX))
UNSPACED_MAKEFLAGS_FIX=$(call CONFIG_FLAGS_FIX,$(UNSPACED_MAKEFLAGS))
MAKEFLAGS_FIX=$(subst ^^^,\ ,$(UNSPACED_MAKEFLAGS_FIX))

ifneq ($(CONFIG_RG_GPL),y)
 ifeq ($(shell ls pkg/license/lic_rg.c 2> /dev/null),)
   LIC_RG_APP=$(JMK_ROOT)/pkg/license/lic_rg
   ifeq ($(shell ls pkg/license/lic_rg 2> /dev/null),)
     $(LIC_RG_APP):
	 @echo $(LIC_RG_APP) " is missing. compilation can't continue."
	 @false
   endif
 else
   LIC_RG_APP=$(JMKE_BUILDDIR)/pkg/license/lic_rg
   include $(JMK_ROOT)/pkg/license/Makefile.lic_rg
 endif
else
  LIC_RG_APP=
endif

# create_config uses these environment variables
export DEV_IF_CONF_FILE=$(JMKE_BUILDDIR)/pkg/main/device_conf.set
export MAC_TYPE_DEVS_FILE=$(JMKE_BUILDDIR)/pkg/main/dev_if_mac_types.c
export MAJOR_FEATURES_FILE=$(JMKE_BUILDDIR)/openrg_features.txt
export MAJOR_FEATURES_CFILE=$(JMKE_BUILDDIR)/pkg/build/features.c
export OSTYPE:=$(shell echo $$OSTYPE)
export CONFIG_LOG=$(JMKE_BUILDDIR)/config.log

ifdef DIST
  CREATE_CONFIG_FLAGS=-d "$(DIST)"
endif

ifdef OS
  CREATE_CONFIG_FLAGS+=-o "$(OS)"
endif

ifdef HW
  CREATE_CONFIG_FLAGS+=-h "$(HW)"
endif

CONFIG_OPTIONS=$(filter-out BUILD=$(BUILD) DIST=$(DIST) OS=$(OS) HW=$(HW) --jobserver-fds%,$(UNSPACED_MAKEFLAGS_FIX))
UNSPACED_FLAGS=$(foreach o,$(CONFIG_OPTIONS),-f $(o))
CREATE_CONFIG_FLAGS+=$(subst ^^^,\ ,$(UNSPACED_FLAGS))
