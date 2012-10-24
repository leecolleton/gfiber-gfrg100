export PWD_REL:=$(if $(filter $(CUR_DIR),$(JMKE_BUILDDIR)),.,$(subst $(JMK_ROOT)/,,$(subst $(JMKE_BUILDDIR)/,,$(CUR_DIR))))
export JPKG_CUR_DIR=$(JPKG_DIR)/$(PWD_REL)

JMKE_MK=$(JMK_ROOT)/jmk/old/rg.mak

CONFIG_FILE=$(JMKE_BUILDDIR)/rg_config.mk
RG_CONFIG_H=$(JMKE_BUILDDIR)/rg_config.h
RG_CONFIG_C=$(JMKE_BUILDDIR)/pkg/util/rg_c_config.c

export JMKE_COPY_DB=$(JMK_ROOT)/jmk/old/ramdisk.mak
export JMKE_COPY_DB_ENVIR=$(JMK_ROOT)/jmk/old/ramdisk_env.mak
export CONFIGURATION_FILE=$(JMKE_BUILDDIR)/config

CONFIG_STARTED_FILE=$(JMKE_BUILDDIR)/.make_config_running

DEBUG_PATH=$(JMKE_BUILDDIR)/debug
