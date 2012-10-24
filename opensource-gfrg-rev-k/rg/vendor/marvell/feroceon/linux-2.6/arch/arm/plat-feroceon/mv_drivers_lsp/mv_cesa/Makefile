#
# Makefile for the Marvell CESA driver
#
ifeq ($(CONFIG_ARCH_FEROCEON),y)
	include $(CURDIR)/$(MACHINE)/config/mvRules.mk
endif

ifeq ($(CONFIG_MV_CESA_OCF),y)
EXTRA_CFLAGS    += -I$(CURDIR)/crypto/ocf -DCESA_OCF_TASKLET \
  -DNO_KERNEL_CRYPTO_FRAMEWORK
endif

obj-$(CONFIG_MV_CESA_TOOL) += cesa_dev.o
obj-$(CONFIG_MV_CESA_OCF)  += cesa_ocf_drv.o

