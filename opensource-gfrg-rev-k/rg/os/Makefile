JMK_ROOT= ../
include $(JMK_ROOT)/envir.mak

ifndef CONFIG_RG_OS
ifdef JMKE_IS_BUILDDIR
  $(error CONFIG_RG_OS not defined!)
endif
endif

ifdef CONFIG_RG_OS_LINUX
  JMK_ARCHCONFIG_FIRST_TASKS+=cp_kernel
endif

ifdef CONFIG_RG_OS_LINUX_24
  LINUX_AUTOCONF_H=$(JMKE_BUILDDIR)/os/linux-2.4/$(AUTOCONF_H)
endif
ifdef CONFIG_RG_OS_LINUX_26
  LINUX_AUTOCONF_H+=$(JMKE_BUILDDIR)/os/linux-2.6/$(AUTOCONF_H)
endif

JMK_ARCHCONFIG_FIRST_TASKS+=os_create_links
JMK_ARCHCONFIG_FIRST_TASKS+=$(if $(CONFIG_RG_OS_LINUX), prepare_linux)

KERNEL_DIR=$(CURDIR)/kernel

ifdef CONFIG_RG_OS_LINUX_24
  ifdef CONFIG_RG_JPKG
    JMK_ARCHCONFIG_SUBDIRS+=linux-2.4
  else
    JMK_SUBDIRS+=linux-2.4
  endif
endif

ifdef CONFIG_RG_OS_LINUX_26
  # kernel 2.6 does not have an archconfig target
  JMK_RAMDISK_SUBDIRS+=linux-2.6
endif

JMK_CLEAN+=$(wildcard $(STATIC_MOD_DIR)/*) $(JMKE_BUILDDIR)/$(notdir $(OPENRG_IMG)) $(JMKE_BUILDDIR)/$(notdir $(OPENRG_PROD)) layout.bin layout.sec

ifndef CONFIG_RG_JPKG
  JMK_OTHER_TASKS+=$(OPENRG_IMG) $(JMKE_BUILDDIR)/$(notdir $(OPENRG_IMG)) make_kernel
endif

ifndef CONFIG_RG_GPL
  ifndef CONFIG_RG_JPKG
    ifeq ($(CONFIG_RG_PROD_IMG),y)
      JMK_OTHER_TASKS+=$(OPENRG_PROD) $(JMKE_BUILDDIR)/$(notdir $(OPENRG_PROD))
    else  
      ifneq ($(CONFIG_RG_NETWORK_BOOT_IMAGE),y)
        JMK_OTHER_TASKS+=$(if $(CONFIG_OPENRG)$(CONFIG_RG_RGLOADER),image_size_verify)
      endif
    endif
    ifeq ($(CONFIG_RG_NETWORK_BOOT_IMAGE),y)
network_boot_message: $(JMK_OTHER_TASKS)

      JMK_OTHER_TASKS+=network_boot_message
    else
end_message: $(JMK_OTHER_TASKS)

      JMK_OTHER_TASKS+=end_message
    endif
  endif
else
  $(shell touch $(JMKE_BUILDDIR)/os/linux/arch/$(ARCH)/ramdisk/mod.img)
  $(shell touch $(JMKE_BUILDDIR)/os/linux/arch/$(ARCH)/ramdisk/ramdisk.gz)
  $(shell touch $(JMKE_BUILDDIR)/os/linux/arch/$(ARCH)/cramdisk/cramfs.img)
  $(shell touch $(JMKE_BUILDDIR)/os/linux/arch/$(ARCH)/cramdisk/cramfs_init.img)
  $(shell touch $(JMKE_BUILDDIR)/os/linux/needed_symbols.lst)
endif
  
ifdef CONFIG_RG_OS_LINUX
  JMK_CD_EXPORTED_FILES+=Rules_common.make Makefile.vmlinuz
endif
ifdef CONFIG_RG_OS_LINUX_24
  JMK_JPKG_EXPORTED_DIR_SRC+=linux-2.4
endif
ifdef CONFIG_RG_OS_LINUX_26
  JMK_JPKG_EXPORTED_DIR_SRC+=linux-2.6
endif

ifdef CONFIG_RG_FACTORY_SETTINGS
  FACTORY_SETTINGS=$(JMKE_BUILDDIR)/pkg/main/rg_factory
endif

ifndef CONFIG_RG_JPKG
  ifneq ($(MAKECMDGOALS),archconfig)
# Adding explicit all: I have to include the boot Makefile before rg.mak for
# distribution purposes but I want the all: the be the first rule. (Ugly? yes,
# working? don't touch)
all:

    include Makefile.vmlinuz
  endif
endif

ifeq ($(CONFIG_RG_RGLOADER),y)
    SECTION_TYPE="\<BOOT\>"
else
    SECTION_TYPE="\<IMAGE\>"
endif

include $(JMKE_MK)
include $(JMK_ROOT)/create_includes.mak

$(OPENRG_IMG): dummy

end_message: dummy
	@echo
	@echo "OpenRG rg_factory is ready at $(FACTORY_SETTINGS)"
	@echo "OpenRG image is ready at $(OPENRG_IMG)"
ifneq ($(CONFIG_RG_UML),)
	@echo "To run this image: "
	@echo "$$ jrguml burn    # 'burn' (load) the new image"
	@echo "$$ jnetwork start # connect uml0 (WAN) and uml1 (LAN) interfaces to UML"
	@echo "$$ jrguml start --std # start running OpenRG/UML"
	@echo "To later stop OpenRG/UML, run 'jrguml stop' from a DIFFERENT terminal"
	@echo "For further assistance see pkg/tools/README"
endif

network_boot_message: dummy
ifeq ($(findstring $(DIST), BCM96358), $(DIST))
	@echo
	@echo "Copy the following file to /tftpboot in order to allow network-booting:"
	@echo $$ cp $(JMKE_BUILDDIR)/vendor/broadcom/bcm963xx/linux-2.6/boot/bootimg.elf /tftpboot
	@echo
endif

$(JMKE_BUILDDIR)/$(notdir $(OPENRG_IMG)): $(notdir $(OPENRG_IMG))
	@$(JMKE_LN) $(OPENRG_IMG) $@

ifeq ($(CONFIG_RG_PROD_IMG),y)
  ifeq ($(findstring $(DIST), RGLOADER_RTA770W RGLOADER_WADB100G), $(DIST))
    # Create RGLoader image: 'jump boot' and 'boot' sections only
    BOOTSTRAP_CMD_PARAM=-s '0 ../vendor/broadcom/bcm963xx/boot/jmp_boot.img'
    BOOTSTRAP=$(OPENRG_IMG)
    # No factory settings or OpenRG image when creating RGLoader
    FACTORY_SETTINGS=
    PROD_SKIP_IMAGE=y
  endif
  ifdef BOOTSTRAP
    BOOTSTRAP_CMD_PARAM+=-b $(BOOTSTRAP)
  endif
  ifdef FACTORY_SETTINGS
    FACTORY_SETTINGS_CMD_PARAM=-f $(FACTORY_SETTINGS)
  endif
  ifneq ($(PROD_SKIP_IMAGE),y)
    IMAGE_PARAM=-i $(OPENRG_IMG)
  endif
$(OPENRG_PROD): $(OPENRG_IMG) $(BOOTSTRAP) $(FACTORY_SETTINGS)
	$(JMKE_BUILDDIR)/pkg/perm_storage/build_flash -v $(BOOTSTRAP_CMD_PARAM) \
	  $(FACTORY_SETTINGS_CMD_PARAM) $(FLASH_LAYOUT_CMD_PARAM) \
	  $(IMAGE_PARAM) -p $(FLASH_IMG_OFFSET)
	cp flash.img $@
ifdef CONFIG_RG_PERM_STORAGE_UBI
	$(JMK_ROOT)/pkg/perm_storage/build_ubi_image.sh $(JMKE_BUILDDIR)/pkg/mtd-utils/ubi-utils/src/ubinize flash.img ubi.img
endif

$(JMKE_BUILDDIR)/$(notdir $(OPENRG_PROD)): $(notdir $(OPENRG_PROD))
	@$(JMKE_LN) $(OPENRG_PROD) $@
else	
image_size_verify: $(OPENRG_IMG)
ifdef JMKE_IS_BUILDDIR
	$(JMKE_BUILDDIR)/pkg/perm_storage/build_flash
ifndef CONFIG_RG_IGNORE_IMAGE_SECTION_SIZE
	@section_size=`$(JMKE_BUILDDIR)/pkg/perm_storage/build_flash  | grep $(SECTION_TYPE) | head -n 1 | awk 'BEGIN { RS="\r\n"} { print $$8}'`; \
	image_size=`du -b -D $(OPENRG_IMG) | awk '{print $$1}'`; \
	free_space=$$((section_size - image_size)); \
	echo "section size = $$((section_size)) , image size = $$image_size" \
	    ", free space = $$free_space"; \
	if (((section_size>0)) && ((image_size>section_size))); then \
	    echo "Image too big (exceeds section size by "\
	        $$((image_size - section_size)) " bytes)."; \
	    exit 1; \
	fi;
endif
endif
endif

modules:
ifdef JMKE_IS_BUILDDIR
	$(MAKE) -C $(KERNEL_DIR) $@
endif

cp_kernel:
	$(MKDIR) $(JMKE_BUILDDIR)/os/
ifdef CONFIG_RG_OS_LINUX_24
	$(JMKE_CP_LN) $(JMK_ROOT)/os/linux-2.4 $(JMKE_BUILDDIR)/os/
endif
ifdef CONFIG_RG_OS_LINUX_26
	$(MKDIR) $(JMKE_BUILDDIR)/os/linux-2.6
endif
	ln -sfn $(JMK_ROOT)/os/Makefile.vmlinuz $(JMKE_BUILDDIR)/os
	ln -sfn $(JMK_ROOT)/os/Makefile $(JMKE_BUILDDIR)/os
	ln -sfn $(JMK_ROOT)/os/Rules_common.make $(JMKE_BUILDDIR)/os

os_create_links:
ifdef CONFIG_RG_OS_LINUX
  ifeq ($(CONFIG_RG_OS),LINUX_22)
	$(JMKE_LN) $(JMK_ROOT)/linux $(CURDIR)/linux-2.2
	$(JMKE_LN) $(CURDIR)/linux-2.2 $(CURDIR)/linux
  endif
  ifeq ($(CONFIG_RG_OS),LINUX_24)
	$(JMKE_LN) $(CURDIR)/linux-2.4 $(CURDIR)/linux
  endif
  ifeq ($(CONFIG_RG_OS),LINUX_26)
	$(JMKE_LN) $(CURDIR)/linux-2.6 $(CURDIR)/linux
  endif
	@$(JMKE_LN) $(CURDIR)/linux $(KERNEL_DIR)
	@$(MKDIR) $(STATIC_MOD_DIR)
else
endif
LINUX_DOT_CONFIG_FILE=$(JMKE_BUILDDIR)/os/linux-2.6/.config
$(LINUX_AUTOCONF_H):
	$(MKDIR) $(@D)
	@cat $(RG_CONFIG_H) | grep -v "extern " | grep -vw _RG_CONFIG_H_ | \
	  grep -v "^#endif" > $@
#        Replace the CC compiler from rg_gcc (as in rg_config.mk) with the 
#        correct one, since only CC is used by the kernel.
ifdef CONFIG_RG_OS_LINUX_26
	@echo "# Jungo auto-generated .config file" > $(LINUX_DOT_CONFIG_FILE)
	@echo "include $(CONFIG_FILE)" >> $(LINUX_DOT_CONFIG_FILE) 
	@echo "CC=$(if $(CONFIG_RG_UML),$(HOSTCC),$(TARGET_CC))" \
	  >> $(LINUX_DOT_CONFIG_FILE)     
	@echo "CPP=$(if $(CONFIG_RG_UML),$(HOSTCPP),$(TARGET_CPP))" \
	  >> $(LINUX_DOT_CONFIG_FILE)     
endif

prepare_linux: $(LINUX_AUTOCONF_H)
ifdef CONFIG_RG_OS_LINUX_26
	$(MAKE) -C $(JMK_ROOT)/os/linux-2.6/ O=$(JMKE_BUILDDIR)/os/linux-2.6/ scripts
	$(MAKE) -C $(JMK_ROOT)/os/linux-2.6/ O=$(JMKE_BUILDDIR)/os/linux-2.6/ prepare
endif

ifdef CONFIG_RG_OS_LINUX
del_subdirs:
	rm -rf $(STATIC_MOD_DIR)
endif

.PHONY: cp_kernel create_links modules del_subdirs prepare_linux
