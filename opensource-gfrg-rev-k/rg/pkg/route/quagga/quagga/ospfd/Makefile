ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif
include $(JMK_ROOT)/pkg/route/quagga/quagga/common.mak

JMK_TARGET=ospfd
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_INTERNAL_HEADERS= ospf_interface.h ospf_apiserver.h ospf_packet.h ospf_asbr.h \
  ospf_dump.h ospf_lsdb.h ospf_snmp.h ospf_route.h ospf_zebra.h \
  ospf_neighbor.h ospf_flood.h ospf_opaque.h ospf_ia.h ospf_te.h ospfd.h \
  ospf_network.h ospf_abr.h ospf_api.h ospf_ase.h ospf_ism.h ospf_lsa.h \
  ospf_nsm.h ospf_spf.h ospf_vty.h

JMK_O_OBJS = ospfd.o ospf_zebra.o ospf_interface.o ospf_ism.o ospf_neighbor.o \
  ospf_nsm.o ospf_dump.o ospf_network.o ospf_packet.o ospf_lsa.o ospf_spf.o \
  ospf_route.o ospf_ase.o ospf_abr.o ospf_ia.o ospf_flood.o ospf_lsdb.o \
  ospf_asbr.o ospf_routemap.o ospf_snmp.o ospf_opaque.o ospf_te.o ospf_vty.o \
  ospf_api.o ospf_apiserver.o ospf_main.o

JMK_LIBS+=$(JMKE_BUILDDIR)/pkg/route/quagga/quagga/lib/libquagga.a \
  __local_crypt $(OPENRG_GPL_LIBS)

$(call JMKE_INCLUDE_RULES)
