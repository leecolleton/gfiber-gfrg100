JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_TARGET=dosfsck
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)
JMK_CFLAGS+=-D_FILE_OFFSET_BITS=64

JMK_O_OBJS=boot.o check.o common.o dosfsck.o fat.o file.o io.o lfn.o

include $(JMKE_MK)
