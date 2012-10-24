ifdef JMKE_IS_BUILDDIR
  ifneq ($(MAKECMDGOALS),rg_config)
    archconfig:: $(JMK_ARCHCONFIG_FIRST_TASKS) export_headers \
      export_headers_flat $(if $(CONFIG_RG_JPKG_SRC),,export_libs) \
      ln_internal_headers $(ARCHCONFIG_SUBDIRS_) $(JMK_ARCHCONFIG_LAST_TASKS) \
      dist_log_entity
  else
    archconfig::
  endif

  # One day we'll remove archconfig, until then...
  config: archconfig
  rg_config: config
endif
