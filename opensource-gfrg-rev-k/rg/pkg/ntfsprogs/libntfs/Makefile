JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_A_TARGET=libntfs.a
JMK_O_OBJS=attrib.o attrlist.o bitmap.o bootsect.o collate.o compat.o compress.o \
  debug.o device.o device_io.o dir.o index.o inode.o lcnalloc.o logfile.o \
  logging.o mft.o mst.o runlist.o security.o unistr.o version.o volume.o

JMK_CFLAGS+=-DHAVE_CONFIG_H -D_FILE_OFFSET_BITS=64 -I.. -I../include/ntfs

include $(JMKE_MK)
