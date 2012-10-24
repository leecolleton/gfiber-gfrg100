JMK_ROOT=../../
include $(JMK_ROOT)/envir.mak

termcapfile=/etc/termcap
JMK_A_TARGET=libtermcap.a
JMK_O_OBJS=termcap.o tparam.o version.o

JMK_CFLAGS+=-DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DSTDC_HEADERS=1 \
	-DTERMCAP_FILE=\"$(termcapfile)\"

JMK_EXPORT_LIBS+=$(JMK_A_TARGET)
JMK_EXPORT_HEADERS+=termcap.h

include $(JMKE_MK)
