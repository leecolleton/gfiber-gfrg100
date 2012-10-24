ifeq ($(CONFIG_RG_JPKG_SRC)$(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY),)
  cfg_first_tasks: __create_includes
endif

config: $(if $(CONFIG_RG_JPKG_SRC),,\
  $(if $(CONFIG_RG_UML),$(if $(CONFIG_RG_OS_LINUX_24),__os__setup)))
__os__setup: archconfig
	$(MAKE) -C $(JMKE_BUILDDIR)/os/linux/ setup JMK_ROOT=$(JMK_ROOT)

MKDIR_SUBDIRS+=$(DEBUG_PATH) $(RG_LIB) $(RG_LIB)/include \
  $(if $(CONFIG_GLIBC),$(RG_BUILD)/glibc/include) \
  $(if $(CONFIG_ULIBC),$(RG_BUILD)/ulibc/include) \
  $(RG_BUILD)/local/include

# rg_gcc is created in pkg/build/Makefile and its depenancy (through gcc) is
# from create_includes.mak
include $(JMK_ROOT)/create_includes.mak

.PHONY: __libc__links
