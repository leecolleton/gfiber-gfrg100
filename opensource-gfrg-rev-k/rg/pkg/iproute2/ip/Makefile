IPROUTE2SRC=..
include $(IPROUTE2SRC)/envir.mak

JMK_LDFLAGS+=$(LIBNETLINK) -lm

JMK_TARGET=ip rtmon
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_INTERNAL_HEADERS+= ./xfrm.h ./ip_common.h

JMK_O_OBJS_ip=ip.o ipaddress.o iproute.o iprule.o rtm_map.o iptunnel.o \
  ipneigh.o iplink.o ipmaddr.o ipmonitor.o ipmroute.o ipxfrm.o \
  xfrm_state.o xfrm_policy.o

JMK_O_OBJS_rtmon=rtmon.o

include $(JMKE_MK)

