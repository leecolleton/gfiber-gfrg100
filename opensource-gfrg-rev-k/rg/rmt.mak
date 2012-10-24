UPD_UTIL=$(JMKE_BUILDDIR)/pkg/rmt-update/upd_file_util

_EXT_VER=$(shell $(JMKE_BUILDDIR)/pkg/build/rg_version_info -e)
EXT_VER=$(if $(_EXT_VER),-e $(_EXT_VER),)

# To create the image, after we did all subdirs, we need to create modules, 
# make ramdisk and create the os.
$(OPENRG_RMT_UPDATE_IMG): $(OPENRG_IMG) $(if $(CONFIG_RG_JPKG),,$(UPD_UTIL))
ifndef CONFIG_RG_JPKG
ifneq ($(CONFIG_RG_NETWORK_BOOT_IMAGE),y)
	@$(UPD_UTIL) -o -C -f $@ -i $(OPENRG_IMG)
	@$(UPD_UTIL) -o -A -f $@ -h $(CONFIG_RG_HW) -d $(CONFIG_RG_DIST) \
	    -v `$(JMKE_BUILDDIR)/pkg/build/rg_version_info -c` $(EXT_VER) -t firmware
	@ln -fs $(OPENRG_RMT_UPDATE_IMG) \
	  $(JMKE_BUILDDIR)/openrg-`$(JMKE_BUILDDIR)/pkg/build/rg_version_info \
	    -f`-$(CONFIG_RG_DIST).rmt
	@echo Remote-update file: $@
endif
endif

ifdef CONFIG_RG_CERT
PRIV_KEY_FILE=$(JMKE_BUILDDIR)/pkg/main/cert/jungo_ca.key
$(OPENRG_RMT_UPDATE_RSA_IMG): $(OPENRG_IMG) $(if $(CONFIG_RG_JPKG),,$(UPD_UTIL))
ifndef CONFIG_RG_JPKG
ifneq ($(CONFIG_RG_NETWORK_BOOT_IMAGE),y)
	@$(UPD_UTIL) -C -f $@ -i $(OPENRG_IMG) -k $(PRIV_KEY_FILE)
	@$(UPD_UTIL) -A -f $@ -h $(CONFIG_RG_HW) -d $(CONFIG_RG_DIST) \
	     -v `$(JMKE_BUILDDIR)/pkg/build/rg_version_info -c` $(EXT_VER) -k $(PRIV_KEY_FILE) -t firmware
	@ln -fs $(OPENRG_RMT_UPDATE_RSA_IMG) \
	  $(JMKE_BUILDDIR)/openrg-`$(JMKE_BUILDDIR)/pkg/build/rg_version_info \
	    -f`-$(CONFIG_RG_DIST).rms
	@echo Secure remote-update file: $@
endif
endif
endif


ifdef CONFIG_RG_DLM
ifndef CONFIG_RG_JPKG	
OPENRG_DLM=$(DISK_IMAGE_DIR)/dlm.img
ifneq ($(CONFIG_RG_NETWORK_BOOT_IMAGE),y)
$(OPENRG_RMT_UPDATE_DLM): $(OPENRG_DLM) $(if $(CONFIG_RG_JPKG),,$(UPD_UTIL))
	@$(UPD_UTIL) -o -C -f $@ -i $(OPENRG_DLM)
	@$(UPD_UTIL) -o -A -f $@ -h $(CONFIG_RG_HW) -d $(CONFIG_RG_DIST) \
	    -v `$(JMKE_BUILDDIR)/pkg/build/rg_version_info -c` $(EXT_VER) -t dlm
	@ln -fs $(OPENRG_RMT_UPDATE_DLM) \
	  $(JMKE_BUILDDIR)/dlm-`$(JMKE_BUILDDIR)/pkg/build/rg_version_info \
	    -f`-$(CONFIG_RG_DIST).rmt
	@echo DLM remote-update file: $@

ifdef CONFIG_RG_CERT
$(OPENRG_RMT_UPDATE_RSA_DLM): $(OPENRG_DLM) $(if $(CONFIG_RG_JPKG),,$(UPD_UTIL))
	@$(UPD_UTIL) -C -f $@ -i $(OPENRG_DLM) -k $(PRIV_KEY_FILE)
	@$(UPD_UTIL) -A -f $@ -h $(CONFIG_RG_HW) -d $(CONFIG_RG_DIST) \
	     -v `$(JMKE_BUILDDIR)/pkg/build/rg_version_info -c` $(EXT_VER) -k $(PRIV_KEY_FILE) -t dlm
	@ln -fs $(OPENRG_RMT_UPDATE_RSA_DLM) \
	  $(JMKE_BUILDDIR)/dlm-`$(JMKE_BUILDDIR)/pkg/build/rg_version_info \
	    -f`-$(CONFIG_RG_DIST).rms
	@echo Secure DLM remote-update file: $@
endif
endif
endif
endif
