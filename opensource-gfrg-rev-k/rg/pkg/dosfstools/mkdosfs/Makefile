JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_TARGET=mkdosfs
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)
JMK_CFLAGS+=-D_FILE_OFFSET_BITS=64

JMK_O_OBJS=mkdosfs.o

include $(JMKE_MK)
