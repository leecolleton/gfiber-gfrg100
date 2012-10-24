# JMK_RG_GLIBC_CFLAGS & JMK_RG_ULIBC_CFLAGS can be used by Makefiles that don't 
# use rg.mak for making .o from .c (e.g. pkg/ulibc/Rules.mak, pkg/gmp/Makefile)
ifdef CONFIG_GLIBC
  JMK_RG_GLIBC_CFLAGS=-I$(RG_BUILD)/glibc/include
endif
ifdef CONFIG_ULIBC
  JMK_RG_ULIBC_CFLAGS=-I$(RG_BUILD)/ulibc/include -I$(JMKE_BUILDDIR)/pkg/ulibc/include
endif
