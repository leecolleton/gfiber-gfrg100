JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

UBI_CFLAGS += -I../include -I. -I../../include -D_FILE_OFFSET_BITS=64
JMK_CFLAGS += $(UBI_CFLAGS)
JMK_LOCAL_CFLAGS += $(UBI_CFLAGS) -D__be16=__u16 -D__be32=__u32 -D__be64=__u64

UBI_LIBS=libubi.a libmtd.a libubigen.a libscan.a
_LOCAL_UBI_LIBS_BASE=iniparser.a ubigen.a

UBI_UTILS=ubiupdatevol ubimkvol ubirmvol ubicrc32 ubinfo ubiattach \
  ubidetach ubiformat ubirename mtdinfo ubirsvol

LOCAL_UBI_UTILS=ubinize

$(foreach l,$(UBI_LIBS), \
  $(eval JMK_O_OBJS_$l+=$(l:%.a=%.o)))

$(foreach l,$(_LOCAL_UBI_LIBS_BASE), \
  $(eval JMK_O_OBJS_liblocal_$l+=$(l:%.a=lib%.o)))

JMK_O_OBJS_libmtd.a+=libmtd_legacy.o
JMK_O_OBJS_liblocal_iniparser.a+=dictionary.o
JMK_O_OBJS_libscan.a+=crc32.o

$(foreach t,$(UBI_UTILS) $(LOCAL_UBI_UTILS), \
  $(eval JMK_O_OBJS_$t+=$t.o common.o))

JMK_O_OBJS_ubicrc32+=crc32.o
JMK_O_OBJS_ubinize+=crc32.o
JMK_O_OBJS_ubiformat+=crc32.o
JMK_O_OBJS_mtdinfo+=crc32.o

JMK_LDFLAGS+=-L.
JMK_LOCAL_LDFLAGS+=-L.
JMK_LDLIBS+=-lubi
JMK_LDLIBS_ubinize+=-llocal_iniparser -llocal_ubigen
JMK_LDLIBS_mtdinfo+=-lmtd -lubigen
JMK_LDLIBS_ubiformat+=-lmtd -lscan -lubigen

JMK_A_TARGET+=$(UBI_LIBS)
JMK_TARGET+=$(UBI_UTILS)
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_LOCAL_A_TARGET+=$(addprefix liblocal_,$(_LOCAL_UBI_LIBS_BASE))
JMK_LOCAL_TARGET+=$(LOCAL_UBI_UTILS)

include $(JMKE_MK)
