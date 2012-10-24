JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

GMPSRC=$(JMK_ROOT)/pkg/gmp

JMK_TARGET=ranbits rsasigkey
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_CFLAGS+=-I../lib
JMK_LDFLAGS+=-L../lib
JMK_LDLIBS:=-lfreeswan $(JMK_LDLIBS)

JMK_O_OBJS_ranbits=ranbits.o
JMK_O_OBJS_rsasigkey=rsasigkey.o
JMK_CFLAGS+=-I$(GMPSRC)
JMK_LDFLAGS_rsasigkey=$(JMK_LDFLAGS) -L$(GMPSRC)
JMK_LDLIBS_rsasigkey:=-lgmp $(OPENRG_GPL_LDLIBS) $(JMK_LDLIBS_rsasigkey)

include $(JMKE_MK)
