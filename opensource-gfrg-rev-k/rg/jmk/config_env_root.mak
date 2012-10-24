# We can do either 'make archconfig' or 'make config'
JMKE_DOING_MAKE_CONFIG:=$(JMKE_DOING_MAKE_CONFIG)
ifeq ($(MAKECMDGOALS),archconfig)
  JMKE_DOING_MAKE_CONFIG=y
endif
ifeq ($(MAKECMDGOALS),config)
  JMKE_DOING_MAKE_CONFIG=y
endif
ifeq ($(MAKECMDGOALS),rg_config)
  JMKE_DOING_MAKE_CONFIG=y
endif

export JMKE_DOING_MAKE_CONFIG

# check whether makeflags for config were defined
ifeq ($(JMKE_DOING_MAKE_CONFIG)-$(IN_RGSRC_ROOT),y-y)
  ifeq ($(DIST),)
    # fetch makeflags from license file, if possible
    ifneq ($(LIC),)
      # Ugly. Please fix me
      $(shell rm -f /tmp/rg_config.mak)
      $(shell $(JMK_ROOT)/pkg/license/lic_rg makeflags-lines "$(LIC)" \
        2>/dev/null 1>/tmp/rg_config.mak)
      include /tmp/rg_config.mak
      EXTRA_MAKEFLAGS:=$(shell $(JMK_ROOT)/pkg/license/lic_rg makeflags "$(LIC)" \
      2>/dev/null)
    endif
  endif
  ifeq ($(DIST),)
    $(error DIST not defined)
  endif
endif
