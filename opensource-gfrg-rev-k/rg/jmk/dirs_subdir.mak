JMK_SUBDIRS+=$(JMK_SUBDIRS_m)
JMK_SUBDIRS+=$(JMK_SUBDIRS_y)
JMK_SUBDIRS+=$(JMK_LEGACY_SUBDIRS) 

$(call Legacy_Dir_Add,$(addprefix $(PWD_REL)/,$(JMK_LEGACY_SUBDIRS)))

# special subdirs vars handling
ifneq ($(MAKECMDGOALS),all)

  ifeq ($(JMKE_DOING_MAKE_CONFIG),y)
    ifneq ($(JMK_ARCHCONFIG_SUBDIRS),)
      JMK_SUBDIRS:=$(JMK_ARCHCONFIG_SUBDIRS)
    endif
  endif

  ifeq ($(MAKECMDGOALS),clean)
    ifneq ($(JMK_CLEAN_SUBDIRS),)
      JMK_SUBDIRS:=$(JMK_CLEAN_SUBDIRS)
    endif
  endif

  ifneq ($(filter $(MAKECMDGOALS), run_tests run_unittests),)
    ifneq ($(JMK_RUN_UNITTESTS_SUBDIRS),)
      JMK_SUBDIRS:=$(JMK_RUN_UNITTESTS_SUBDIRS)
    endif
  endif

  ifeq ($(MAKECMDGOALS),xmldocs)
    ifneq ($(JMK_DOCS_SUBDIRS),)
      JMK_SUBDIRS:=$(JMK_DOCS_SUBDIRS)
    endif
  endif

endif # not all

ifneq ($(JMKE_DOING_MAKE_CONFIG),y)
  ifdef CONFIG_RG_JPKG
    ifneq ($(JMK_JPKG_EXPORTED_DIR),)
      JMK_SUBDIRS:=$(filter-out $(addsuffix %,$(JMK_JPKG_EXPORTED_DIR)), \
	$(JMK_SUBDIRS))
    endif
  endif
endif
