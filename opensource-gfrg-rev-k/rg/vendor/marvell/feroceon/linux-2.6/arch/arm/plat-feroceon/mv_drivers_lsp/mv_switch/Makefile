#
# Makefile for the Marvell Key
#
ifeq ($(CONFIG_ARCH_FEROCEON),y)
	include $(CURDIR)/$(MACHINE)/config/mvRules.mk
endif

obj-$(CONFIG_MV_INCLUDE_SWITCH)	+= mv_switch.o mv_switch_sysfs.o mv_switch_wrap.o
