include $(JMK_ROOT)/config_root_first_phase_env.mak

$(CONFIG_STARTED_FILE):
	touch $@

$(JMKE_BUILDDIR)/.rgsrc: $(CONFIG_STARTED_FILE)
	echo $(JMK_ROOT) > $@

$(CONFIGURATION_FILE): $(JMKE_BUILDDIR)/.rgsrc
	echo $(MAKEFLAGS_FIX) > $(CONFIGURATION_FILE)

$(JMKE_BUILDDIR)/.configure: $(CONFIGURATION_FILE)
	$(JMKE_LN) $< $@

# compile create config prereqs
$(JMKE_BUILDDIR)/host_config.mk: $(JMKE_BUILDDIR)/.configure
	$(JMK_ROOT)/pkg/build/detect_host.sh \
	  $(JMKE_BUILDDIR)/host_config.h \
	  $(JMKE_BUILDDIR)/host_config.mk \
	  $(JMKE_BUILDDIR)/host_config.sh
	mkdir -p $(JMKE_BUILDDIR)/pkg/build/

MKDIR_SUBDIRS+=$(addprefix $(JMKE_BUILDDIR)/, \
  pkg/lib pkg/lib/rg pkg/license pkg/jpkg pkg/build pkg/main pkg/util \
  pkg/include)

lic_rg_prereqs: $(JMKE_BUILDDIR)/host_config.mk $(MKDIR_SUBDIRS)

$(LIC_RG_APP): lic_rg_prereqs

create_cfg_prereqs: $(LIC_RG_APP) $(JMKE_BUILDDIR)/host_config.mk \
  $(MKDIR_SUBDIRS)

# compile create config
$(CREATE_CONFIG): create_cfg_prereqs

include $(JMK_ROOT)/pkg/build/Makefile.create_config

# run create config
$(CONFIG_FILE): $(CREATE_CONFIG)
	JMKE_BUILDDIR=$(JMKE_BUILDDIR) JMK_ROOT=$(JMK_ROOT) CONFIG_LOG=$(CONFIG_LOG) \
	  $(CREATE_CONFIG) \
	    $(CREATE_CONFIG_FLAGS) -m $(CONFIG_FILE) \
	    -e $(DEV_IF_CONF_FILE) \
	    -M $(MAJOR_FEATURES_FILE) \
	    -F $(MAJOR_FEATURES_CFILE) \
	    -i $(RG_CONFIG_H) -c $(RG_CONFIG_C) \
	    >$(CONFIG_LOG) \
	    2>&1 ; RET=$$? ; cat $(CONFIG_LOG) ; exit $$RET
	$(JMKE_LN) $(CONFIG_FILE) $(JMKE_BUILDDIR)/rg_configure
	$(JMKE_LN) $(RG_CONFIG_H) $(JMKE_BUILDDIR)/pkg/include

do_first_phase: $(CONFIG_FILE)
	
# Finished first pass. Moving to the second, this time with configs.
do_second_phase: do_first_phase
	$(MAKE) ROOT_CONFIG_SECOND_PHASE=y config

config_ended_successfully: do_second_phase

config: config_ended_successfully
rg_config: do_first_phase
	rm -f $(CONFIG_STARTED_FILE)

config_ended_successfully: archconfig
	rm -f $(CONFIG_STARTED_FILE)
	cat $(CONFIG_LOG)
	@echo Configuration ended successfully. You can now run make.

.PHONY: do_first_phase do_second_phase
