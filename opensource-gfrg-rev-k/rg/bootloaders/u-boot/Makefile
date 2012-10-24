JMK_ROOT=../../
include $(JMK_ROOT)/envir.mak

JMK_ARCHCONFIG_FIRST_TASKS+=uboot_make_config

uboot_make_config: uboot_clean
ifeq ($(AEI_HW), REVI)
	$(MAKE) -f Makefile.u-boot mi424wr_i_config
else
ifeq ($(AEI_HW), REVJ)
	$(MAKE) -f Makefile.u-boot mi424wr_j_config
else
ifeq ($(AEI_HW), REVK0)
	$(MAKE) -f Makefile.u-boot mi424wr_k0_config
else
ifeq ($(AEI_HW), REVK1)
	$(MAKE) -f Makefile.u-boot mi424wr_k1_config
else
ifeq ($(AEI_HW), REVK2)
	$(MAKE) -f Makefile.u-boot mi424wr_k2_config
else
ifeq ($(AEI_HW), REVK4)
	$(MAKE) -f Makefile.u-boot mi424wr_k4_config
else
	$(MAKE) -f Makefile.u-boot db88f6500bp_config NANDBOOT=1 NAND=1 DDR3=1 UBI=1
endif
endif
endif
endif
endif
endif

JMK_OTHER_TASKS+=uboot_make

uboot_make:
	$(MAKE) -f Makefile.u-boot

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR
clean:: uboot_clean

uboot_clean:
	$(MAKE) -f Makefile.u-boot distclean
	$(MAKE) -f Makefile.u-boot mrproper
endif
