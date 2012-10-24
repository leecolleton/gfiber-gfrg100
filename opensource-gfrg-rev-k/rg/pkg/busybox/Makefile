JMK_ROOT=../..
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_RG_RGLOADER
  JMK_SUBDIRS=applets modutils init procps util-linux libbb
else
  JMK_SUBDIRS=applets archival archival/libunarchive coreutils console-tools \
    debianutils editors findutils init miscutils modutils networking \
    networking/libiproute networking/udhcp procps loginutils shell \
    sysklogd util-linux libpwdgrp coreutils/libcoreutils libbb
endif

busybox.links: applets/busybox.mkll include/config.h include/applets.h
ifdef JMKE_IS_BUILDDIR
	$^ >$@
endif

JMK_ARCHCONFIG_FIRST_TASKS+=del_busybox_libs.mk
del_busybox_libs.mk:
	rm -f busybox_libs.mk

ifndef JMKE_DOING_MAKE_CONFIG
ifdef JMKE_IS_BUILDDIR
include busybox_libs.mk
endif
endif

JMK_OTHER_TARGETS+=busybox.links

JMK_RAMDISK_LAST_TASKS+=ramdisk_links
ramdisk_links: ./applets/install.sh
	$(call RAMDISK_MKDIR_FUNC,$(JMKE_RAMDISK_RW_DIR)/bin)
	$(call RAMDISK_MKDIR_FUNC,$(JMKE_RAMDISK_RO_DIR)/bin)
	$< $(PREFIX)

JMK_TARGET+=busybox
JMK_RAMDISK_BIN_FILES+=busybox

JMK_LDFLAGS+=-Wl,--start-group $(BUSYBOX_LIBS) -Wl,--end-group

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR
busybox: $(BUSYBOX_LIBS)
endif
