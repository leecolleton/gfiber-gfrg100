ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

obj-$(CONFIG_OCF_OCF) += ocf_mod.o
JMK_O_OBJS_ocf_mod.o += crypto.o criov.o
JMK_MOD_CFLAGS_ocf_mod.o += -DNO_KERNEL_CRYPTO_FRAMEWORK

OCF_HEADERS = cryptodev.h ocf-compat.h uio.h

JMK_INTERNAL_HEADERS += $(OCF_HEADERS)

OCF_LINKS_DIR = $(JMKE_BUILDDIR)/os/linux-2.6/crypto/ocf/

OCF_LINKS = $(addprefix $(OCF_LINKS_DIR),$(OCF_HEADERS))

ifdef CONFIG_OCF_RANDOMHARVEST
  JMK_O_OBJS_ocf_mod.o += random.o
endif

ifdef CONFIG_OCF_FIPS
  JMK_O_OBJS_ocf_mod.o += rndtest.o
  JMK_MOD_CFLAGS_ocf_mod.o += -DFIPS_TEST_RNG=1
  JMK_INTERNAL_HEADERS+=rndtest.h
endif

obj-$(CONFIG_OCF_CRYPTODEV) += cryptodev_mod.o
JMK_O_OBJ_cryptodev_mod.o += cryptodev.o
obj-$(CONFIG_OCF_CRYPTOSOFT) += cryptosoft_mod.o
JMK_O_OBJ_cryptosoft_mod.o += cryptosoft.o
obj-$(CONFIG_OCF_BENCH) += ocf-bench_mod.o
JMK_O_OBJ_ocf-bench_mod.o += ocf-bench.o

JMK_MOD_TARGET+=$(obj-y) $(obj-m)
JMK_MOD_2_STAT+=$(obj-y)
JMK_RAMDISK_MODULES_FILES+=$(obj-m)

JMK_ARCHCONFIG_FIRST_TASKS += $(OCF_LINKS)

MV_OCF_DIR = $(JMK_ROOT)/vendor/marvell/feroceon/linux-2.6/crypto/ocf

$(OCF_LINKS):
	$(JMKE_MKDIR) $(dir $@)
	$(JMKE_CP_LN) $(MV_OCF_DIR)/$(patsubst $(OCF_LINKS_DIR)%,%,$@) $@

$(call JMKE_INCLUDE_RULES)
