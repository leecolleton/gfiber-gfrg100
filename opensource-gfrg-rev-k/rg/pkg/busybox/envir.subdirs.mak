PROG      := busybox
VERSION   := 1.01
BUILDTIME := $(shell TZ=UTC date -u "+%Y.%m.%d-%H:%M%z")

# What OS are you compiling busybox for?  This allows you to include
# OS specific things, syscall overrides, etc.
TARGET_OS=linux

JMK_LOCAL_CFLAGS=-Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

# Ensure consistent sort order, 'gcc -print-search-dirs' behavior, etc.
LC_ALL:=C

WARNINGS=-Wall -Wstrict-prototypes -Wshadow
JMK_CFLAGS+=-I$(JMK_ROOT)/pkg/busybox/include -g
ARFLAGS=cru

JMK_ARCHCONFIG_LAST_TASKS+=update_libs_mk
update_libs_mk:
	$(if $(strip $(JMK_A_TARGET)),echo BUSYBOX_LIBS+=$(patsubst $(JMKE_BUILDDIR)/pkg/busybox/%,./%,$(CURDIR))/$(JMK_A_TARGET) >> $(JMKE_BUILDDIR)/pkg/busybox/busybox_libs.mk)

export VERSION BUILDTIME TOPDIR HOSTCC HOSTCFLAGS CROSS CC AR AS LD NM STRIP CPP
ifeq ($(strip $(TARGET_ARCH)),)
TARGET_ARCH:=$(shell $(REAL_GCC) -dumpmachine | sed -e s'/-.*//' \
		-e 's/i.86/i386/' \
		-e 's/sparc.*/sparc/' \
		-e 's/arm.*/arm/g' \
		-e 's/m68k.*/m68k/' \
		-e 's/ppc/powerpc/g' \
		-e 's/v850.*/v850/g' \
		-e 's/sh[234]/sh/' \
		-e 's/mips-.*/mips/' \
		-e 's/mipsel-.*/mipsel/' \
		-e 's/cris.*/cris/' \
		)
endif

-include $(JMK_ROOT)/pkg/busybox/.config

SECHO=@echo

# Some nice architecture specific optimizations
ifeq ($(strip $(TARGET_ARCH)),arm)
    OPTIMIZATION+=-fstrict-aliasing
endif
ifeq ($(strip $(TARGET_ARCH)),i386)
    OPTIMIZATION+=$(call check_gcc,-march=i386,)
    OPTIMIZATION+=$(call check_gcc,-mpreferred-stack-boundary=2,)
    OPTIMIZATION+=$(call check_gcc,-falign-functions=0 -falign-jumps=0 -falign-loops=0,\
    -malign-functions=0 -malign-jumps=0 -malign-loops=0)
endif
OPTIMIZATIONS=$(OPTIMIZATION) -fomit-frame-pointer

ifeq ($(strip $(CONFIG_DEBUG)),y)
    JMK_CFLAGS+=$(WARNINGS) -g -D_GNU_SOURCE
    JMK_LDFLAGS+=-Wl,-warn-common
    STRIPCMD:=/bin/true -Not_stripping_since_we_are_debugging
else
    JMK_CFLAGS+=$(WARNINGS) $(OPTIMIZATIONS) -D_GNU_SOURCE -DNDEBUG
    JMK_LDFLAGS+=-Wl,-warn-common
    STRIPCMD:=$(STRIP) -s --remove-section=.note --remove-section=.comment
endif
ifeq ($(strip $(CONFIG_STATIC)),y)
    JMK_LDFLAGS+=--static
endif

ifeq ($(strip $(PREFIX)),)
    PREFIX:=`pwd`/_install
endif

ifeq ($(strip $(CONFIG_LFS)),y)
    # For large file summit support
    JMK_CFLAGS+=-D_FILE_OFFSET_BITS=64
endif

ifeq ($(CONFIG_SYSCTL),y)
    JMK_CFLAGS+=-DCONFIG_SYSCTL=1
endif

JMK_CFLAGS+=$(CROSS_CFLAGS)
ifdef BB_INIT_SCRIPT
    JMK_CFLAGS+=-DINIT_SCRIPT='"$(BB_INIT_SCRIPT)"'
endif

