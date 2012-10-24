JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

JMK_CFLAGS+=-DIN_BUSYBOX

JMK_O_OBJS_udhcp.a+=$(if $(CONFIG_UDHCPC)$(CONFIG_UDHCPD),\
  common.o options.o packet.o pidfile.o signalpipe.o socket.o)

JMK_O_OBJS_udhcp.a+=$(if $(CONFIG_UDHCPC),\
  dhcpc.o clientpacket.o clientsocket.o script.o)

JMK_O_OBJS_udhcp.a+=$(if $(CONFIG_UDHCPD),\
  dhcpd.o arpping.o files.o leases.o serverpacket.o static_leases.o)

JMK_O_OBJS_udhcp.a+=$(if $(CONFIG_DUMPLEASES),dumpleases.o)

ifneq ($(strip $(JMK_O_OBJS_udhcp.a)),)
  JMK_A_TARGET+=udhcp.a
endif

include $(JMKE_MK)
