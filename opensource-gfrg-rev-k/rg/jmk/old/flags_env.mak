# $1 - cflags
# $2 - parts of cflags we don't want to fix
FIX_VPATH_CFLAGS_EXCEPT_FOR=$(strip \
  $(foreach f,$1, \
    $(if $(filter-out -I -I- $2,$(filter -I%,$f)), \
      -I$(call SRC2BUILD_ALLOW_RELATIVE,$(f:-I%=%)) $(if $(JMKE_DOING_MAKE_CONFIG),$f,), \
      $f) \
    ) \
)
    
FIX_VPATH_CFLAGS=$(call FIX_VPATH_CFLAGS_EXCEPT_FOR,$1,)

FIX_VPATH_LDFLAGS=$(strip \
  $(foreach f,$1, \
    $(if $(filter-out -L,$(filter -L%,$f)), \
      -L$(call SRC2BUILD_ALLOW_RELATIVE,$(f:-L%=%)) $(if $(JMKE_DOING_MAKE_CONFIG),$f,), \
      $f)\
    )\
)

# JMK_RG_GLIBC_CFLAGS & JMK_RG_ULIBC_CFLAGS can be used by Makefiles that don't 
# use rg.mak for making .o from .c (e.g. pkg/ulibc/Rules.mak, pkg/gmp/Makefile)
ifdef CONFIG_GLIBC
-include $(JMKE_BUILDDIR)/pkg/build/glibc/include/libc_config.make
JMK_RG_GLIBC_CFLAGS=-I$(RG_BUILD)/glibc/include
endif
ifdef CONFIG_ULIBC
-include $(JMKE_BUILDDIR)/pkg/build/ulibc/include/libc_config.make
JMK_RG_ULIBC_CFLAGS=-I$(RG_BUILD)/ulibc/include -I$(JMKE_BUILDDIR)/pkg/ulibc/include
endif

