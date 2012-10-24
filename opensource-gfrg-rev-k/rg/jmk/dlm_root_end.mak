
# clean the dlm_staging dir
clean_dlm_dir:
	rm -rf $(DLM_STAGING_DIR)
	$(JMKE_MKDIR) $(DLMFS_DIR)

# Combine file list in a per-target DLM_FILES_x
$(foreach t, $(sav_JMK_DLM_TARGET), \
  $(eval DLM_FILES_$t = \
    $(sav_JMK_DLM_FILES_$t) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_SWF_FILES_$t),/home/httpd/html/swf) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_IMG_FILES_$t),/home/httpd/html/images) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_JS_FILES_$t),/home/httpd/html) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_CGI_FILES_$t),/home/httpd/html) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_LIB_FILES_$t),/lib) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_ETC_FILES_$t),/etc) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_BIN_FILES_$t),/bin) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_VAR_FILES_$t),/var) \
    $(call RD_Export_Fmt,$(sav_JMK_DLM_MODULES_FILES_$t),/lib/modules)))

# $1 - source
# $2 - destination in package
# $3 - package name
DLM_CP_File = \
  $(eval $(DLM_STAGING_DIR)/$3/$2: $1 ; \
    $(call DLM_CP_FUNC,$1,$(DLM_STAGING_DIR)/$3/$2,$(call RD_RO_Strip_Func,$1,$2))) \
  $(eval dlm_files_$3+=$(DLM_STAGING_DIR)/$3/$2)

# $1 - dlm name
# $2 - deb target
DLM_Build_Deb = \
  $(JMKE_MKDIR) $(DLM_STAGING_DIR)/$1/DEBIAN && \
  echo Package: $1 > $(DLM_STAGING_DIR)/$1/DEBIAN/control && \
  echo Version: $(JMK_DLM_VERSION_$1) >> $(DLM_STAGING_DIR)/$1/DEBIAN/control && \
  echo Maintainer: 'Jungo LTD. <rg_support@jungo.com>' >> \
	  $(DLM_STAGING_DIR)/$1/DEBIAN/control && \
  echo Description: $(JMK_DLM_DESC_$1) >> $(DLM_STAGING_DIR)/$1/DEBIAN/control && \
  echo Architecture: $(ARCH) >> $(DLM_STAGING_DIR)/$1/DEBIAN/control && \
  dpkg-deb --build $(DLM_STAGING_DIR)/$1 $2

# For each target, copy files in DLM_FILES_x to the dlm staging area,
# and add a rule for creating a .deb in the dlmfs directory.
$(foreach t,$(sav_JMK_DLM_TARGET), \
  $(foreach f,$(DLM_FILES_$t), \
    $(call DLM_CP_File,$(call _GET_FILE_FROM_TO,$f,1),$(call _GET_FILE_FROM_TO,$f,2),$t)) \
  $(eval $(DLMFS_DIR)/$t.deb: $(dlm_files_$t) ; \
    $(call DLM_Build_Deb,$t,$$@)))

ramdisk_dlm_files=$(foreach t,$(sav_JMK_DLM_TARGET),$(DLMFS_DIR)/$t.deb)

# Append ramdisk_dlm_files to ALL_RAMDISK_FILES
ALL_RAMDISK_FILES+=$(ramdisk_dlm_files)
