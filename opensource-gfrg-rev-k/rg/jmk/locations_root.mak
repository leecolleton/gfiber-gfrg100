JMKE_MK=$(error old rg.mak referenced from new JMK [$(CUR_DIR)])
JMKE_MK2=$(JMK_ROOT)/jmk/rg_root.mak

CONFIG_FILE=$(JMKE_BUILDDIR)/rg_config.mk
RG_CONFIG_H=$(JMKE_BUILDDIR)/rg_config.h
RG_CONFIG_C=$(JMKE_BUILDDIR)/pkg/util/rg_c_config.c

CONFIGURATION_FILE=$(JMKE_BUILDDIR)/config

CONFIG_STARTED_FILE=$(JMKE_BUILDDIR)/.make_config_running

DEBUG_PATH=$(JMKE_BUILDDIR)/debug
