JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_TARGET=sfdisk 
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)
JMK_O_OBJS_sfdisk=sfdisk.o i386_sys_types.o partname.o

# add JMK_TARGET+=fdisk if you want to compile fdisk aswell
JMK_O_OBJS_fdisk=fdisk.o llseek.o fdiskbsdlabel.o fdisksgilabel.o fdisksunlabel.o \
	fdiskaixlabel.o i386_sys_types.o partname.o

JMK_CFLAGS+=-I. -I../lib -O2 -D_FILE_OFFSET_BITS=64

include $(JMKE_MK)
