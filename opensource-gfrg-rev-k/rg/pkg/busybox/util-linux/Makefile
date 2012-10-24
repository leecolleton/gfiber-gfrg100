JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_DMESG),dmesg.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FBSET),fbset.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FDFLUSH),fdflush.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FDFORMAT),fdformat.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FDISK),fdisk.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FREERAMDISK),freeramdisk.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_FSCK_MINIX),fsck_minix.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_GETOPT),getopt.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_HEXDUMP),hexdump.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_HWCLOCK),hwclock.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_LOSETUP),losetup.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_MKFS_MINIX),mkfs_minix.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_MKSWAP),mkswap.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_MORE),more.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_MOUNT),mount.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_NFSMOUNT),nfsmount.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_PIVOT_ROOT),pivot_root.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_RDATE),rdate.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_SWAPONOFF),swaponoff.o)
JMK_O_OBJS_util-linux.a+=$(if $(CONFIG_UMOUNT),umount.o)

ifneq ($(strip $(JMK_O_OBJS_util-linux.a)),)
  JMK_A_TARGET+=util-linux.a
endif

ifneq ($(strip $(CONFIG_LFS)),y)
ifeq ($(strip $(FDISK_SUPPORT_LARGE_DISKS)),y)
  JMK_CFLAGS_fdisk.o+=-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE \
    -D_FILE_OFFSET_BITS=64
endif
endif

include $(JMKE_MK)
