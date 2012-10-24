# We can do either 'make archconfig' or 'make config'
JMKE_DOING_MAKE_CONFIG:=$(JMKE_DOING_MAKE_CONFIG)
ifeq ($(MAKECMDGOALS),archconfig)
  JMKE_DOING_MAKE_CONFIG:=y
endif
ifeq ($(MAKECMDGOALS),config)
  JMKE_DOING_MAKE_CONFIG:=y
endif
ifeq ($(MAKECMDGOALS),rg_config)
  JMKE_DOING_MAKE_CONFIG:=y
endif

export JMKE_DOING_MAKE_CONFIG

# check whether makeflags for config were defined
ifeq ($(JMKE_DOING_MAKE_CONFIG)-$(IN_RGSRC_ROOT),y-y)
  ifeq ($(DIST),)
    # fetch makeflags from license file, if possible
    ifneq ($(LIC),)
      $(shell rm -f /tmp/rg_config.mak)
      $(shell $(JMK_ROOT)/pkg/license/lic_rg makeflags-lines "$(LIC)" \
        2>/dev/null 1>/tmp/rg_config.mak)
      include /tmp/rg_config.mak
      $(shell rm -f /tmp/rg_config.mak)
      EXTRA_MAKEFLAGS:=$(shell $(JMK_ROOT)/pkg/license/lic_rg makeflags "$(LIC)" \
      2>/dev/null)
    endif
  endif
  ifeq ($(DIST),)
    $(error DIST not defined)
  endif
endif

CHECK_CONFIGURE:=$(if $(JMKE_DOING_MAKE_CONFIG),,check_configure)
ifeq ($(MAKECMDGOALS),oldconfig)
  # Do not check that the tree is configured
  CHECK_CONFIGURE:=
endif

check_configure:
	$(if $(wildcard $(CONFIGURATION_FILE)),,$(error Tree is not configured. Run make config))
	$(if $(wildcard $(CONFIG_STARTED_FILE)),$(error Tree did not complete configuration successfully. Run make config),)

# allow archconfig targets to compile before headers are exported using
# the export_headers target (the alternative is to divide archconfig into 
# two passes over the whole tree; first for export_headers and second for
# archconfig targets)
ifdef JMKE_DOING_MAKE_CONFIG
  JMK_LOCAL_CFLAGS+=-I$(JMK_ROOT)/pkg/include
endif

