# This is a container to all the special make command goals that does not
# require jmk logic. The real work is done in Makefile.main.

IN_RGSRC_ROOT=y

ifndef JMKE_ENV_INCLUDED
  JMK_ROOT:=$(PWD)

  ifeq ($(filter test_jpkg,$(MAKECMDGOALS)),)
    include $(JMK_ROOT)/jmk/env_root.mak
  endif

endif

SPECIAL_MAKECMDGOALS=echovar distclean test_jpkg

echovar:
	@echo $($(ECHOVAR))

distclean:
	rm -rf $(JMKE_BUILDDIR) $(BUILDDIR_LINK)

test_jpkg: $(JMK_ROOT)/pkg/tools/compile_jpkg.sh
	@MAKEFLAGS="$(filter-out LIC=% JPKG_DIST=% CONTINUE=% DATE=%,$(MAKEFLAGS))" $<
	@echo
	@echo JPKGs compiled succesfully !
	@echo


ifeq ($(filter $(SPECIAL_MAKECMDGOALS), $(MAKECMDGOALS)),)
  include $(JMK_ROOT)/Makefile.main
endif

.PHONY: $(SPECIAL_MAKECMDGOALS)
