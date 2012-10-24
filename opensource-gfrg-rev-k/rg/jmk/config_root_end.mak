# maximal dependencies tree:

archconfig:: cfg_mkdirs cfg_first_tasks $(export_tasks) cfg_last_tasks

cfg_last_tasks: $(sav_JMK_ARCHCONFIG_JPKG_LAST_TASKS) \
  $(if $(CONFIG_RG_JPKG_SRC),,$(sav_JMK_ARCHCONFIG_LAST_TASKS))

ifndef CONFIG_RG_JPKG_SRC
  cfg_last_tasks $(sav_JMK_ARCHCONFIG_LAST_TASKS) \
    $(sav_JMK_ARCHCONFIG_JPKG_LAST_TASKS): legacy_archconfig

  legacy_archconfig: cfg_mkdirs cfg_first_tasks $(export_tasks)
else
  # Don't run "make config" for legacy subdirs
  cfg_last_tasks $(sav_JMK_ARCHCONFIG_JPKG_LAST_TASKS): \
    cfg_mkdirs cfg_first_tasks $(export_tasks)
endif

$(export_tasks):: cfg_mkdirs cfg_first_tasks
cfg_first_tasks: $(sav_JMK_ARCHCONFIG_JPKG_FIRST_TASKS) \
  $(if $(CONFIG_RG_JPKG_SRC),,$(sav_JMK_ARCHCONFIG_FIRST_TASKS))

cfg_first_tasks $(sav_JMK_ARCHCONFIG_FIRST_TASKS) \
  $(sav_JMK_ARCHCONFIG_JPKG_FIRST_TASKS): cfg_first_tasks_start

cfg_first_tasks_start: cfg_mkdirs

# minimal dependencies tree:
# contents are same as above. this is just a more readable version imo.
#
# archconfig: cfg_last_tasks
# cfg_last_tasks: $(sav_JMK_ARCHCONFIG_[JPKG_]LAST_TASKS)
# cfg_last_tasks $(sav_JMK_ARCHCONFIG_[JPKG_]LAST_TASKS): \
#   legacy_archconfig 
# legacy_archconfig: $(export_tasks)
# $(export_tasks):: cfg_first_tasks
# cfg_first_tasks: $(sav_JMK_ARCHCONFIG_[JPKG_]FIRST_TASKS)
# cfg_first_tasks $(sav_JMK_ARCHCONFIG_[JPKG_]FIRST_TASKS): \
#   cfg_first_tasks_start
# cfg_first_tasks_start: cfg_mkdirs

cfg_mkdirs:
	$(if $(sav_SUBDIRS),$(JMKE_MKDIR) $(call SRC2BUILD,$(sav_SUBDIRS)))
ifdef CONFIG_RG_JPKG
	$(if $(sav_SUBDIRS), \
	  $(JMKE_MKDIR) $(call Abs_Path_To_Jpkg,$(sav_SUBDIRS)))
endif

internal_phony_targets+=cfg_mkdirs cfg_first_tasks_start cfg_first_tasks
internal_phony_targets+=$(export_tasks) cfg_last_tasks_start cfg_last_tasks
internal_phony_targets+=archconfig

# One day we'll remove archconfig, until then...
config: archconfig
