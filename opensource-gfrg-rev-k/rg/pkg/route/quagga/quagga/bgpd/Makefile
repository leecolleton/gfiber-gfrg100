ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif
include $(JMK_ROOT)/pkg/route/quagga/quagga/common.mak

JMK_TARGET=bgpd
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_INTERNAL_HEADERS= bgpd.h bgp_table.h bgp_aspath.h bgp_route.h bgp_dump.h \
  bgp_debug.h bgp_community.h bgp_attr.h bgp_regex.h bgp_clist.h bgp_fsm.h \
  bgp_packet.h bgp_zebra.h bgp_open.h bgp_filter.h bgp_nexthop.h bgp_damp.h \
  bgp_mplsvpn.h bgp_advertise.h bgp_network.h bgp_vty.h bgp_ecommunity.h

JMK_O_OBJS = bgpd.o bgp_fsm.o bgp_aspath.o bgp_community.o bgp_attr.o bgp_debug.o \
  bgp_route.o bgp_zebra.o bgp_open.o bgp_routemap.o bgp_packet.o bgp_network.o \
  bgp_filter.o bgp_regex.o bgp_clist.o bgp_dump.o bgp_snmp.o bgp_ecommunity.o \
  bgp_mplsvpn.o bgp_nexthop.o bgp_damp.o bgp_table.o bgp_advertise.o bgp_vty.o \
  bgp_main.o

JMK_LIBS+=$(JMKE_BUILDDIR)/pkg/route/quagga/quagga/lib/libquagga.a \
  __local_crypt $(OPENRG_GPL_LIBS) __local_m

$(call JMKE_INCLUDE_RULES)
