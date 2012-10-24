# TODO clean non-ramdisk related stuff from this file
ifeq ($(DEBUG_FLAG),1)
  DEBUG_MAKE=debug
endif

# return from object list, the objects that should be put on the ramdisk
RD_Mod_Filter = \
  $(filter-out $(sav_JMK_MOD_2_STAT) $(sav_JMK_MOD_O_TARGET),$1)

# reformat file to src__dst
# $1 - source paths (dir+file)
# $2 - destination dir
RD_Export_Fmt = $(foreach f,$1,$f__$2/$(notdir $f))

ifdef CONFIG_RG_JNET_SERVER
  ramdisk_images_path=$(JRMS_UI_IMG_DST) \
    $(if $(CONFIG_RG_JRMS_REDUCE_SUPPORT),$(JRMS_SCR_IMG_DST))
endif
ifdef CONFIG_RG_VAS_PORTAL
  ramdisk_images_path=/vas/images
endif
ifdef CONFIG_OPENRG
  ramdisk_images_path=/home/httpd/html/images
endif

RAMDISK_FILES_ = \
  $(sav_JMK_RAMDISK_FILES) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_SWF_FILES),/home/httpd/html/swf) \
  $(foreach f, $(ramdisk_images_path), \
    $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_IMG_FILES),$f)) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_JS_FILES),/home/httpd/html) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_CGI_FILES),/home/httpd/html) \
  $(call RD_Export_Fmt,$(call RD_Mod_Filter,$(sav_JMK_RAMDISK_MODULES_PERMANENT_FILES)),/lib/modules) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_PLUGINS_FILES),/plugins) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_LIB_FILES),/lib) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_ETC_FILES),/etc) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_BIN_FILES),/bin) \
  $(call RD_Export_Fmt,$(sav_JMK_RAMDISK_VAR_FILES),/var)

# for kernel modules, change the module filename from .ko to .o in the ramdisk
RAMDISK_MODULES_FILES_ = \
  $(call RD_Export_Fmt,$(call RD_Mod_Filter,$(sav_JMK_RAMDISK_MODULES_FILES)),/lib/modules) \
  $(foreach f,$(call RD_Mod_Filter,$(sav_JMK_RAMDISK_KERNEL_MODULES)), \
    $f__/lib/modules/$(basename $(notdir $f)).o)

# $1 filename with full path in the ramdisk. E.g.: /bin/simple_init.sh
Need_Strip=$(filter-out $(sav_JMK_RAMDISK_NO_STRIP),$(filter /bin/%,$1))

# $1 - source
# $2 - destination in ramdisk
RD_RO_Strip_Func = $(strip \
  $(if $(call Need_Strip,$2), \
    $(if $(CONFIG_BINFMT_FLAT),RAMDISK_NO_STRIP,RAMDISK_STRIP), \
    $(if $(filter $1,$(sav_JMK_JS_FILES)), RAMDISK_STRIP_JS, \
	$(if $(filter /lib/%,$2), \
	  $(if $(filter /lib/modules/%,$2), \
	    $(if $(filter $1,$(sav_JMK_MODULES_DONT_STRIP)), \
	      RAMDISK_NO_STRIP, RAMDISK_STRIP_DEBUG_SYM), \
	    RAMDISK_STRIP), \
	  $(if $(filter /etc/%.so,$2),RAMDISK_STRIP,RAMDISK_NO_STRIP)))))

# $1 - source
# $2 - destination in ramdisk
RD_RO_File = \
  ${if ${debug_ramdisk},${warning JMK: ramdisk RO_File: \
    $(JMKE_RAMDISK_RO_DIR)/$2: $1}} \
  $(eval $(JMKE_RAMDISK_RO_DIR)/$2: $1 ; \
    $(call RAMDISK_CP_RO_FUNC,$1,$2,$(call RD_RO_Strip_Func,$1,$2))) \
  $(eval ramdisk_files+=$(JMKE_RAMDISK_RO_DIR)/$2)

$(foreach f,$(RAMDISK_FILES_), \
  $(call RD_RO_File,$(call _GET_FILE_FROM_TO,$f,1),$(call _GET_FILE_FROM_TO,$f,2)))

# $1 - source path (in build dir)
RD_Mod_Strip_Func = $(strip \
  $(if $(filter $1,$(sav_JMK_MODULES_DONT_STRIP)), \
    RAMDISK_NO_STRIP, \
    RAMDISK_STRIP_DEBUG_SYM))

RD_Mod_Cp_Func = $(if $(CONFIG_RG_MODFS),RAMDISK_CP_MOD_FUNC,RAMDISK_CP_RO_FUNC)

ramdisk_mod_dir := $(if $(CONFIG_RG_MODFS),$(MODFS_DIR),$(JMKE_RAMDISK_RO_DIR))

# $1 - source
# $2 - destination in ramdisk
RD_Mod_File = \
  ${if ${debug_ramdisk},${warning JMK: ramdisk Mod_File: \
    ${ramdisk_mod_dir}/$2: $1}} \
  $(eval $(ramdisk_mod_dir)/$2: $1 ; \
    $(call $(RD_Mod_Cp_Func),$1,$2,$(call RD_Mod_Strip_Func,$1))) \
  $(eval ramdisk_mod_files+=$(ramdisk_mod_dir)/$2)

$(foreach f,$(RAMDISK_MODULES_FILES_), \
  $(call RD_Mod_File,$(call _GET_FILE_FROM_TO,$f,1),$(call _GET_FILE_FROM_TO,$f,2)))

# $1 - source
# $2 - destination in ramdisk
RD_RW_File = \
  ${if ${debug_ramdisk},${warning JMK: ramdisk RW_File: \
    ${JMKE_RAMDISK_RW_DIR}/$2: $1}} \
  $(eval $(JMKE_RAMDISK_RW_DIR)/$2: $1 ; $(call RAMDISK_CP_RW_FUNC,$1,$2)) \
  $(eval ramdisk_rw_files+=$(JMKE_RAMDISK_RW_DIR)/$2)

$(foreach f,$(sav_JMK_RAMDISK_RW_FILES), \
  $(call RD_RW_File,$(call _GET_FILE_FROM_TO,$f,1),$(call _GET_FILE_FROM_TO,$f,2)))

$(sav_JMK_RAMDISK_DIRS):
	$(JMKE_MKDIR) $@

ALL_RAMDISK_FILES:=$(ramdisk_files) $(ramdisk_mod_files) $(ramdisk_rw_files)
ALL_RAMDISK_TASKS+=$(sav_JMK_RAMDISK_TASKS)
ALL_RAMDISK_DIRS+=$(sav_JMK_RAMDISK_DIRS)

include $(JMK_ROOT)/pkg/build/Makefile.ramdisk
