Conv_Ramdisk_2_DLM = \
  $(if $(JMK_RAMDISK$1_FILES), \
    $(eval JMK_DLM$1_FILES_$(RAMDISK_2_DLM)+=$(JMK_RAMDISK$1_FILES)) \
    $(eval JMK_RAMDISK$1_FILES=))

ifneq ($(RAMDISK_2_DLM),)
  $(call Conv_Ramdisk_2_DLM,)
  $(call Conv_Ramdisk_2_DLM,_BIN)
  $(call Conv_Ramdisk_2_DLM,_ETC)
  $(call Conv_Ramdisk_2_DLM,_LIB)
  $(call Conv_Ramdisk_2_DLM,_VAR)
  $(call Conv_Ramdisk_2_DLM,_MODULES)
  $(call Conv_Ramdisk_2_DLM,_SWF)
  $(call Conv_Ramdisk_2_DLM,_JS)
  $(call Conv_Ramdisk_2_DLM,_CGI)
  $(call Conv_Ramdisk_2_DLM,_IMG)
endif
