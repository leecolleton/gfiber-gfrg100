include 	  $(CURDIR)/$(MACHINE)/config/mvRules.mk

ifneq ($(CONFIG_MTD),)
obj-$(CONFIG_MV_FLASH_CTRL)	+= mv_drivers_lsp/mv_mtd/
obj-$(CONFIG_MTD_FEROCEON)      += flashmap.o
ifneq ($(CONFIG_MTD_FEROCEON)$(CONFIG_ARCH_FEROCEON_KW2),)
	flashmap-objs = ../mach-feroceon-kw2/flashmap.o
endif
obj-$(CONFIG_MV_NAND)		+= mvnand.o
ifneq ($(CONFIG_MTD_NAND)$(CONFIG_ARCH_FEROCEON_KW2),)
	mvnand-objs += ../mach-feroceon-kw2/nand.o
endif
endif

obj-$(CONFIG_MV_ETH_LEGACY) 	+= mv_drivers_lsp/mv_network/mv_ethernet/
obj-$(CONFIG_MV_ETH_NETA) 	+= mv_drivers_lsp/mv_neta/net_dev/

ifneq ($(CONFIG_SCSI),)
obj-$(CONFIG_SCSI_MVSATA)       += mv_drivers_lsp/mv_sata/
endif

ifneq ($(CONFIG_PHONE),)
obj-$(CONFIG_MV_PHONE) 		+= mv_drivers_lsp/mv_phone/
endif

obj-$(CONFIG_FEROCEON_WATCHDOG)	+= mv_drivers_lsp/mv_watchdog/
