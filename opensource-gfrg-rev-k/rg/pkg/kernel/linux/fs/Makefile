JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

JMK_SUBDIRS+=$(if $(CONFIG_YAFFS_FS),yaffs2)
JMK_JPKG_EXPORTED_DIR_SRC+=$(if $(CONFIG_YAFFS_FS),yaffs2)
JMK_LINK_DIRS+=$(if $(CONFIG_YAFFS_FS),yaffs2)

include $(JMKE_MK)
