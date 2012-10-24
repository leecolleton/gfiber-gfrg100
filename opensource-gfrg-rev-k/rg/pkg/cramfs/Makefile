ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../..
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_LOCAL_CXX_TARGET=mkcramfs
JMK_O_OBJS_mkcramfs=mkcramfs.o

JMK_LOCAL_CFLAGS=-Wall -O2 -I$(JMK_ROOT)/pkg/lzma/SRC
ifdef CONFIG_ULIBC
  JMK_LOCAL_CFLAGS+=-I$(RG_BUILD)/ulibc
endif
ifdef CONFIG_GLIBC
  JMK_LOCAL_CFLAGS+=-I$(RG_BUILD)/glibc
endif
JMK_LOCAL_LIBS=$(JMKE_BUILDDIR)/pkg/zlib/liblocal_z.a \
  $(JMKE_BUILDDIR)/pkg/lzma/SRC/liblocal_lzma.a

$(call JMKE_INCLUDE_RULES)
