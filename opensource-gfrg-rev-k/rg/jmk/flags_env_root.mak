ifdef CONFIG_GLIBC
-include $(JMKE_BUILDDIR)/pkg/build/glibc/include/libc_config.make
endif
ifdef CONFIG_ULIBC
-include $(JMKE_BUILDDIR)/pkg/build/ulibc/include/libc_config.make
endif
