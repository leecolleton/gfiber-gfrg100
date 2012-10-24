JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_DYN_LINK
  JMK_SO_TARGET=libfuse-lite.so
  JMK_EXPORT_LIBS+=$(JMK_SO_TARGET)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  JMK_A_TARGET=libfuse-lite.a
  LIB=$(JMK_A_TARGET)
  JMK_EXPORT_LIBS+=$(JMK_A_TARGET)
endif

JMK_O_OBJS=fuse.o fuse_kern_chan.o fuse_lowlevel.o fuse_loop.o fuse_opt.o \
       fuse_session.o fuse_signals.o fusermount.o helper.o mount.o \
       mount_util.o helper.o


JMK_INTERNAL_HEADERS=fuse_i.h fuse_misc.h mount_util.h

JMK_CFLAGS+=-DHAVE_CONFIG_H -I.. -I../include/fuse-lite

include $(JMKE_MK)
