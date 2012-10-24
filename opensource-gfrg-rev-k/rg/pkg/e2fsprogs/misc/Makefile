E2FSSRC=..
include $(E2FSSRC)/envir.mak

JMK_TARGET=mke2fs badblocks
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)
#JMK_TARGET+=badblocks tune2fs dumpe2fs e2image fsck mklpf chattr lsattr uuidgen

JMK_O_OBJS_mke2fs=mke2fs.o util.o get_device_by_label.o
JMK_O_OBJS_badblocks=badblocks.o
JMK_O_OBJS_tune2fs=tune2fs.o util.o get_device_by_label.o
JMK_O_OBJS_dumpe2fs=dumpe2fs.o
JMK_O_OBJS_e2image=e2image.o
JMK_O_OBJS_fsck=fsck.o get_device_by_label.o base_device.o fstype.o
JMK_O_OBJS_mklpf=mklost+found.o
JMK_O_OBJS_chattr=chattr.o
JMK_O_OBJS_lsattr=lsattr.o
JMK_O_OBJS_uuidgen=uuidgen.o

include $(E2FSMK)
