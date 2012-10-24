JMK_ROOT=../../
include $(JMK_ROOT)/envir.mak

ifdef JMKE_IS_BUILDDIR

JMK_EXPORT_HEADERS_DIR=gmp
JMK_EXPORT_HEADERS+=gmp.h

CONFIG_FLAGS = --host=$(TARGET_MACHINE) --target=$(TARGET_MACHINE)
ifeq ($(ARCH),arm)
  CONFIG_FLAGS = --host=arm-linux --target=arm-linux --build=i386-linux
endif
ifeq ($(ARCH),mips)
  ifdef CONFIG_64BIT
    CONFIG_FLAGS = --host=mips64-unknown-linux --target=mips64-unknown-linux --build=i386-linux
  else
    CONFIG_FLAGS = --host=mips-linux --target=mips-linux --build=i386-linux
  endif
endif
ifeq ($(ARCH),i386)
  CONFIG_FLAGS = --host=i386-linux --target=i386-linux --build=i386-linux
endif
ifeq ($(ARCH),um)
  CONFIG_FLAGS = --host=i386-linux --target=i386-linux --build=i386-linux
endif
ifeq ($(ARCH),ppc)
  CONFIG_FLAGS = --host=powerpc-linux --target=powerpc-linux --build=powerpc-linux
endif
gmpMakefile=gmp.created_by_configure.mak
export gmpMakefile

all: archconfig dummy
	if test -f $(gmpMakefile); then \
	$(MAKE) -f $(gmpMakefile) JMK_CFLAGS="$(call FIX_VPATH_CFLAGS,$(JMK_CFLAGS)) \
	   $(if $(CONFIG_ULIBC),$(JMK_RG_ULIBC_CFLAGS),$(JMK_RG_GLIBC_CFLAGS))" \
	   JMK_LDFLAGS="$(JMK_LDFLAGS)" \
	AR=$(AR) RANLIB=$(RANLIB) Makefile=$(gmpMakefile);\
	fi

archconfig: .configure export_headers dummy

#XXX a hack so dep will not fail.
dep:
run_unittests:

export_headers:
	$(MKDIR) $(JMKE_BUILDDIR)/pkg/include/$(JMK_EXPORT_HEADERS_DIR)
	$(foreach f,$(JMK_EXPORT_HEADERS),$(JMKE_LN) $(JMKE_PWD_SRC)/$f \
	  $(JMKE_BUILDDIR)/pkg/include/$(JMK_EXPORT_HEADERS_DIR)/$f)

.configure: 
ifndef DIST
	@echo 'error - give param DIST' && false
endif	
	@touch .top_level
	./configure $(CONFIG_FLAGS)
	@rm -f .top_level
	@touch .configure

clean:
	if test -f $(gmpMakefile); then \
	$(MAKE) -f $(gmpMakefile) Makefile=$(gmpMakefile) clean; \
	fi
	@rm -f .configure .depend

ifdef CONFIG_DYN_LINK
  JMK_RAMDISK_LIB_FILES=libgmp.so.2.0.2
endif
include $(JMKE_COPY_DB)

ifdef CONFIG_DYN_LINK
ramdisk::
	$(call RAMDISK_LN_FUNC,libgmp.so.2.0.2,$(JMKE_RAMDISK_RW_DIR)/lib/libgmp.so.2)
endif

force:
.PNONY: check install install-bsdmp install-info-files install-strip uninstall
.PHONY: all clean archconfig ramdisk

else # JMKE_IS_BUILDDIR
# XXX This code is taken from rg.mak. Its a hack until this Makefile will be
# writen properly.
# 
# This hack enables compiling from within the pkg/gmp directory.
# In the normal flow of compilation we will always enter the 'if' side of
# 'ifdef JMKE_IS_BUILDDIR'. Only when doing 'cd pkg/gmp ; make' we will enter here
# (the 'else' side of 'ifdef JMKE_IS_BUILDDIR').
#
# IMPORTANT: just doing 'make' will not work,
# only 'make all <or some other target>' will work

%:
	@echo "pwd: `/bin/pwd` PWD:$(PWD) [$@:$^]"
	mkdir -p $(JMKE_BUILDDIR)/pkg/gmp
	make -C $(JMKE_BUILDDIR)/pkg/gmp $(MAKECMDGOALS) \
	  JMK_ROOT=$(JMK_ROOT)
endif # JMKE_IS_BUILDDIR
