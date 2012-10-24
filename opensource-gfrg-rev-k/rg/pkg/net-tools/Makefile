JMK_ROOT := ../..
include $(JMK_ROOT)/envir.mak
include $(NET_TOOLS_MAKE_FILE)

JMK_INTERNAL_HEADERS+= \
    ./lib/net-features.h \
    ./lib/net-support.h \
    ./lib/pathnames.h \
    ./lib/proc.h \
    ./lib/util.h \
    ./intl.h \
    ./include/interface.h \
    ./include/ipx.h \
    ./include/sockets.h \
    ./include/util-ank.h

RELEASE	= 1.54
H_FILES = config.h version.h
CONFIG_FILE = $(JMKE_BUILDDIR)/pkg/include/net-tools-config.h
MKCONFIG = $(JMKE_BUILDDIR)/pkg/util/mkconfig

JMK_CFLAGS	+= -D_GNU_SOURCE -I. -idirafter include -Ilib
JMK_LDFLAGS	+= -Llib
JMK_LDLIBS:=-lnet-tools $(JMK_LDLIBS)

JMK_SUBDIRS	= lib

ifdef CONFIG_RG_NETTOOLS_ARP
JMK_TARGET+=arp
JMK_RAMDISK_BIN_FILES+=arp
endif

ifdef NOT_DEFINED
JMK_TARGET+=netstat ifconfig hostname route rarp
endif

ifeq ($(HAVE_IP_TOOLS),1)
JMK_TARGET+=iptunnel ipmaddr
endif

#create for each target in JMK_TARGET a JMK_O_OBJS_target=target.o
$(foreach t, $(JMK_TARGET), $(eval JMK_O_OBJS_$t:=$(t:%=%.o)))
JMK_O_OBJS_netstat+=statistics.o

JMK_ARCHCONFIG_FIRST_TASKS+=$(H_FILES)

include $(JMKE_MK)
	
config.h: $(CONFIG_FILE)
	@ln -sf $< $@

#TODO change version to be OPENRG version
version.h:
	@echo "#define RELEASE \"net-tools $(RELEASE)\"" > version.h
