$(call Dir_Dep,pkg/xml,pkg/expat)
$(call Dir_Dep,pkg/ntfsprogs,pkg/ntfs-3g)
$(call Dir_Dep,pkg/voip/sox,pkg/av/libmad)
ifdef CONFIG_RG_HW_QOS
  $(JMKE_BUILDDIR)/pkg/include/vendor/o/hw_qos.o: \
    $(if $(CONFIG_DANUBE), $(call Legacy_Dir_Target,vendor/infineon/danube)) \
    $(if $(CONFIG_RG_HW_QOS_PLAT_IKANOS), \
      $(call Legacy_Dir_Target,vendor/ikanos))
endif
ifdef CONFIG_FREESWAN
  $(call Dir_Dep,pkg/freeswan,pkg/gmp)
  ifdef CONFIG_RG_SYSLOG_GLIBC
    $(call Dir_Dep_File,pkg/freeswan,$(JMKE_BUILDDIR)/pkg/syslog/glibc/libulibc_syslog.a)
  endif
endif
ifdef CONFIG_RG_PSE
  ifdef CONFIG_DANUBE
    $(call Dir_Dep,pkg/fastpath,vendor/infineon/danube)
  endif
  ifdef CONFIG_CAVIUM_OCTEON
    $(call Dir_Dep,pkg/fastpath,vendor/cavium/octeon)
  endif
  ifdef CONFIG_ARCH_AVALANCHE
    $(call Dir_Dep,pkg/fastpath,vendor/ti/avalanche)
  endif
  ifdef CONFIG_MINDSPEED_COMMON
    $(call Dir_Dep,pkg/fastpath,vendor/mindspeed)
  endif
  ifneq ($(CONFIG_FUSIV_VX160)$(CONFIG_FUSIV_VX180),)
    $(call Dir_Dep,pkg/fastpath,vendor/ikanos)
  endif
endif
ifdef CONFIG_TI_DOCSIS
  $(call Dir_Dep,vendor/ti/rg,vendor/ti/docsis_30)
endif

ifndef CONFIG_RG_JPKG_SRC
  ifdef CONFIG_RG_FLEX
    $(call Dir_Dep_File,pkg/usagi,$(JMKE_BUILDDIR)/pkg/flex/libfl.a)
    ifdef CONFIG_TI_DOCSIS
      $(call Dir_Dep_File,vendor/ti/docsis_30,$(JMKE_BUILDDIR)/pkg/flex/libfl.a)
    endif
  endif
endif

ifdef CONFIG_IXP425_ATM
  $(call Dir_Dep,pkg/atm,vendor/intel)
endif
ifdef CONFIG_BCM963XX_ATM
  $(call Dir_Dep,pkg/atm,vendor/broadcom)
endif
ifneq ($(CONFIG_MACH_ADI_FUSIV)$(CONFIG_MACH_IKAN_MIPS),)
  $(call Dir_Dep,pkg/atm,vendor/ikanos)
endif
ifdef CONFIG_ATM_NULL
  $(call Dir_Dep,pkg/atm,vendor/jungo)
endif
ifdef CONFIG_ATM_DANUBE
  $(call Dir_Dep,pkg/atm,vendor/infineon/danube)
endif
ifdef CONFIG_P400_ATM
  $(call Dir_Dep,pkg/atm,vendor/centillium)
endif

ifeq ($(JMKE_IS_DISTRIBUTION),y)
  $(call Legacy_Dir_Target,pkg/kos): $(JMK_ROOT)/pkg/kos/conv_base
else
  $(call Legacy_Dir_Target,pkg/kos): $(JMKE_BUILDDIR)/pkg/kos/conv_base
endif

ifeq ($(CONFIG_MIPS_BRCM)$(CONFIG_OPENRG),yy)
  $(call Dir_Dep,pkg/mgt/dev,vendor/broadcom/rg/dev)
endif
