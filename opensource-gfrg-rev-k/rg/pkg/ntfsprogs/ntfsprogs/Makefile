JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_TARGET=mkntfs ntfsfix
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_O_OBJS_mkntfs=mkntfs.o utils.o sd.o boot.o upcase.o attrdef.o
JMK_O_OBJS_ntfsfix=ntfsfix.o utils.o

JMK_CFLAGS+=-DHAVE_CONFIG_H -D_FILE_OFFSET_BITS=64 -I..

JMK_LDFLAGS+=-lntfs-3g  $(OPENRG_GPL_LDLIBS)

include $(JMKE_MK)
