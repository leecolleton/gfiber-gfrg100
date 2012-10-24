IPROUTE2SRC=..
include $(IPROUTE2SRC)/envir.mak

JMK_LDFLAGS+=$(LIBNETLINK) -lm

JMK_TARGET=ss nstat ifstat rtacct lnstat
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_INTERNAL_HEADERS+= ./ssfilter.h ./lnstat.h  

JMK_O_OBJS_ss=ss.o ssfilter.o
JMK_O_OBJS_lnstat=lnstat.o lnstat_util.o
JMK_O_OBJS_nstat=nstat.o
JMK_O_OBJS_ifstat=ifstat.o
JMK_O_OBJS_rtacct=rtacct.o
# Currently not supported since requires Berkeley DB with 1.85 compatibility
#JMK_O_OBJS_arpd=arpd.o
#JMK_TARGET+=arpd

DISTCLEAN+=ssfilter.c

include $(JMKE_MK)

ifndef JMKE_IS_DISTRIBUTION
ssfilter.c: ssfilter.y
	$(YACC) ssfilter.y -o ssfilter.c
endif

