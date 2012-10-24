JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_TARGET=ntfs-3g
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_O_OBJS=ntfs-3g.o

JMK_CFLAGS+=-DHAVE_CONFIG_H -DFUSE_USE_VERSION=26 -I.. -I../include/ntfs-3g \
	-I../include/fuse-lite

JMK_LDFLAGS+=-lrt -lntfs-3g -lfuse-lite -lpthread $(OPENRG_GPL_LDLIBS)

include $(JMKE_MK)
