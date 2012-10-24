JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_DYN_LINK
  JMK_SO_TARGET=libntfs-3g.so
  JMK_EXPORT_LIBS+=$(JMK_SO_TARGET)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  JMK_A_TARGET=libntfs-3g.a
  LIB=$(JMK_A_TARGET)
  JMK_EXPORT_LIBS+=$(JMK_A_TARGET)
endif

JMK_O_OBJS=attrib.o compat.o device_io.o logfile.o mft.o unistr.o attrlist.o \
  compress.o dir.o logging.o bitmap.o index.o mst.o bootsect.o debug.o inode.o \
  runlist.o volume.o collate.o device.o lcnalloc.o security.o \
  $(if $(CONFIG_RG_DBG_ULIBC_MALLOC),,misc.o)

JMK_CFLAGS+=-DHAVE_CONFIG_H -I.. -I../include/ntfs-3g

include $(JMKE_MK)
