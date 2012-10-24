ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_CLEAN=rootfs.img efss_data.o $(COMPRESSED_DISK) $(MODFS_DISK) \
  $(COMPRESSED_INIT_DISK)

ifdef CONFIG_RG_UML
  JMK_CLEAN+=$(JMK_ROOT)/os/linux/fs.img
endif

export REMOVE_IFDEFS=y

# JMK_OTHER_TASKS and not JMK_OTHER_TARGETS because we don't want it to
# be deleted in every clean
ifndef CONFIG_WINDOWS_HOST
  JMK_OTHER_TASKS=exec_as_root

  JMK_LOCAL_TARGET=checksum
  JMK_O_OBJS_checksum=checksum.o
endif

JMK_CD_EXPORTED_FILES+=exec_as_root.c ./make_ramdisk.sh export_src.c \
  make_packages.sh jslint.conf
JMK_INTERNAL_HEADERS=license_text.h

JMK_EXPORT_HEADERS_DIR=build
JMK_EXPORT_HEADERS+=$(JMKE_PWD_BUILD)/features.c__c/features.c
ifdef CONFIG_RG_CONFIG_STRINGS
  JMK_EXPORT_HEADERS+=config_strings.h
endif
JMK_AUTOGEN_SRC+=features.c
JMK_CD_EXPORTED_FILES+=Makefile.ramdisk create_config.c \
  create_config.h device_config.c dist_config.c feature_config.c \
  hw_config.c config_opt.c major_features.c config_opt.h \
  Makefile.create_config config_host.c config_target_os.c detect_host.sh \
  mklibs rg_utils.pm gc.pl app_rm_unneeded_syms.pl
ifeq ($(CONFIG_RG_EFSS),y)
  JMK_LOCAL_TARGET+=mkefs
  JMK_LIBS_mkefs=$(JMKE_BUILDDIR)/pkg/zlib/liblocal_z.a
  JMK_O_OBJS_mkefs=mkefs.o
  JMK_O_TARGET+=efss_main.o
  JMK_O_OBJS_efss_main.o=efss.o
  JMK_CLEAN+=image.h info.h libramdisk.a
  JMK_CD_EXPORTED_FILES+=efss_data.c efss_data.h efss.h
endif

JMK_LOCAL_TARGET+=write_int
JMK_O_OBJS_write_int=write_int.o

ifeq ($(CONFIG_OPENRG)-$(CONFIG_RG_GPL),y-)
  JMK_LOCAL_TARGET+=rg_version_info
  JMK_LIBS_rg_version_info=$(JMKE_BUILDDIR)/pkg/util/liblocal_openrg.a \
    $(JMKE_BUILDDIR)/pkg/util/liblocal_jutil.a
  JMK_O_OBJS_rg_version_info=rg_version_info.o
endif

JMK_FIRST_CLEAN=disk_image_clean

ifdef CONFIG_RG_JPKG
  JMK_SUBDIRS=conf_files
endif
ifeq ($(CONFIG_RG_JNET_SERVER)$(CONFIG_RG_VAS_PORTAL)$(CONFIG_RG_RGLOADER),)
  JMK_SUBDIRS=conf_files
endif
# JMK_CLEAN_SUBDIRS=$(JMK_SUBDIRS) $(if $(JMKE_IS_DISTRIBUTION),,make_cd)

include $(JMKE_PWD_SRC)/Makefile.create_config

disk_image_clean: $(_EXEC_AS_ROOT)
	$(_EXEC_AS_ROOT_COMMAND) rm -fr disk_image
	$(_EXEC_AS_ROOT_COMMAND) rm -fr bootldr_disk_image

.PHONY: do_footprint
JMK_CLEAN+=footprint_report.txt pkg_sum_file.txt  
ifdef CONFIG_RG_LZMA_COMPRESSED_KERNEL
  ARCHIVER:=--archiver "$(JMKE_BUILDDIR)/pkg/lzma/SRC/lzma -so e"
endif

ifndef CONFIG_RG_GPL
  JMK_LOCAL_TARGET+=footprint_report
endif
JMK_LIBS_footprint_report+=$(JMKE_BUILDDIR)/pkg/util/liblocal_jutil.a
JMK_CFLAGS_footprint_report+=-g
JMK_O_OBJS_footprint_report+=footprint_report.o

do_footprint: $(JMKE_PWD_BUILD)/footprint_report
	$(JMKE_BUILDDIR)/pkg/build/footprint_report \
	  --output $(JMKE_BUILDDIR)/pkg/build/footprint_report.txt \
	  --output-list $(JMKE_BUILDDIR)/pkg/build/footprint_list_file.txt \
	  --strip $(STRIP) --objcopy $(OBJCOPY) \
	  --linux_img $(PROD_KERNEL_IMAGE) --builddir $(JMKE_BUILDDIR) \
	  $(ARCHIVER)

JMK_CD_EXPORTED_FILES+=rg_gcc.c
$(JMKE_BUILDDIR)/pkg/build/rg_gcc: $(JMK_ROOT)/pkg/build//rg_gcc.c \
  $(JMKE_BUILDDIR)/rg_config.mk
	. $(JMKE_BUILDDIR)/host_config.sh ; $$CC \
	  -I$(JMKE_BUILDDIR)/pkg/include -O0 -Wall -Wstrict-prototypes -g \
	  -fno-strict-aliasing \
	  -DRG_GCC="\"$(REAL_GCC)\"" \
	  -DGCC_PREFIX="\"$(LIB_GCC_DIR_PREFIX)\"" \
	  -DJMK_ROOT="\"$(JMK_ROOT)\"" \
	  -DJMKE_BUILDDIR="\"$(JMKE_BUILDDIR)\"" \
	  -o $@ $<

$(JMKE_BUILDDIR)/pkg/build/gcc: $(JMKE_BUILDDIR)/pkg/build/rg_gcc
	$(JMKE_LN) $(JMKE_BUILDDIR)/pkg/build/rg_gcc $@

JMK_RAMDISK_DIRS=$(RAMDISK_MOUNT_POINT)

$(call JMKE_INCLUDE_RULES)
