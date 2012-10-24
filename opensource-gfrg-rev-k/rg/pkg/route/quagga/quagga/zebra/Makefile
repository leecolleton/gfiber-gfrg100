ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif
include $(JMK_ROOT)/pkg/route/quagga/quagga/common.mak

JMK_TARGET=zebra
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_INTERNAL_HEADERS= rib.h zserv.h router-id.h redistribute.h debug.h ipforward.h \
  irdp.h rtadv.h interface.h ioctl.h connected.h rt.h

JMK_O_OBJS = zserv.o main.o interface.o connected.o zebra_rib.o redistribute.o \
  debug.o rtadv.o zebra_snmp.o zebra_vty.o irdp_main.o irdp_interface.o \
  irdp_packet.o router-id.o ipforward_proc.o rt_ioctl.o if_proc.o \
  rtread_proc.o if_ioctl.o ioctl.o

JMK_LIBS+=$(JMKE_BUILDDIR)/pkg/route/quagga/quagga/lib/libquagga.a \
  $(JMKE_BUILDDIR)/pkg/build/lib/libcrypt.a $(OPENRG_GPL_LIBS)

$(call JMKE_INCLUDE_RULES)
