JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak
include $(NET_TOOLS_MAKE_FILE)

HWOBJS = hw.o loopback.o slip.o ether.o ax25.o ppp.o arcnet.o tr.o tunnel.o \
    frame.o sit.o rose.o ash.o fddi.o hippi.o hdlclapb.o irda.o ec_hw.o

AFOBJS = unix.o inet.o inet6.o ax25.o ipx.o ddp.o ipx.o netrom.o af.o rose.o \
    econet.o

AFGROBJS = inet_gr.o inet6_gr.o ipx_gr.o ddp_gr.o netrom_gr.o ax25_gr.o \
    rose_gr.o getroute.o

AFSROBJS = inet_sr.o inet6_sr.o netrom_sr.o ipx_sr.o setroute.o

ACTOBJS  = slip_ac.o ppp_ac.o activate.o

VARIA = getargs.o masq_info.o proc.o util.o nstrcmp.o interface.o sockets.o

ifeq ($(HAVE_IP_TOOLS),1)
VARIA += util-ank.o
endif

JMK_O_OBJS = $(VARIA) $(AFOBJS) $(HWOBJS) $(AFGROBJS) $(AFSROBJS) $(ACTOBJS)
JMK_CLEAN += $(JMK_L_OBJS)
JMK_A_TARGET=libnet-tools.a

JMK_CFLAGS += -I$(JMK_ROOT)/pkg/net-tools -idirafter $(JMK_ROOT)/pkg/net-tools/include \
  -idirafter ../include -I$(JMK_ROOT)/pkg/include

include $(JMKE_MK)

