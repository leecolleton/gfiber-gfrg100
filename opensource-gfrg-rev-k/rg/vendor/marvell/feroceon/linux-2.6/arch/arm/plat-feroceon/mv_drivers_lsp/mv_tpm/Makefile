#
# Makefile for the Marvell TPM driver
#

ifeq ($(CONFIG_ARCH_FEROCEON),y)
	include $(CURDIR)/$(MACHINE)/config/mvRules.mk
endif

ifdef CONFIG_MV_HAL_RULES_PATH
include $(CURDIR)/include/config/auto.conf
include $(CURDIR)/$(subst ",,$(CONFIG_MV_HAL_RULES_PATH))
endif

TPM_OBJS += 	core/tpm_api.o \
		core/tpm_db.o \
		core/tpm_counter.o \
		core/tpm_mempool.o \
		core/tpm_init.o \
		core/tpm_pkt_proc_logic.o \
		core/tpm_tm.o \
		core/tpm_pnc_logic.o \
		core/tpm_print.o \
		core/tpm_trace.o \
		core/tpm_sysfs_trace.o \
		core/tpm_sysfs_print.o \
		core/tpm_mtu.o\
		core/tpm_ctc_cm.o\
		plat/tpm_modz2_mgr.o \
		plat/tpm_pnc_mgr.o \
		perf/tpm_bring_up.o \
		perf/tpm_mng_if.o \
		perf/tpm_usr_if.o \
		perf/tpm_sysfs_setup.o \
		perf/tpm_sysfs_rule_db.o \
		perf/tpm_sysfs_hwcall.o \
		perf/tpm_sysfs_help.o \
		perf/tpm_sysfs_utils.o\
		perf/tpm_self_check.o

ifdef CONFIG_MV_TPM_XML_SETUP
TPM_OBJS +=	core/tpm_setup.o \
		core/tpm_xml_params.o \
		perf/tpm_sysfs_setup.o
endif

ifdef CONFIG_MV_INCLUDE_SWITCH
TPM_OBJS +=	plat/tpm_switch_mgr.o \
		plat/tpm_alarm.o
endif

mv_tpm-objs := $(TPM_OBJS)
obj-$(CONFIG_MV_TPM) := mv_tpm.o

