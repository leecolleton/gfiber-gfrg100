#
# Makefile for the Marvell Gigabit Ethernet driver
#
ifeq ($(CONFIG_ARCH_FEROCEON_KW2),y)
	include $(CURDIR)/arch/arm/mach-feroceon-kw2/config/mvRules.mk
endif

obj-$(CONFIG_MV_ETH_HWF) += hwf_sysfs.o

ifndef CONFIG_MV_ETH_BM_CPU
obj-$(CONFIG_MV_ETH_HWF) += hwf_bm.o
endif
