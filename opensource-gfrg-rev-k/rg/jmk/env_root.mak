# see rg/jmk/jmk.txt
# This file is included once from the $PWD/Makefile. All the rest of the subdirs
# Makeifle should be protected by ifdef JMKE_ENV_INCLUDED

ifdef JMKE_ENV_INCLUDED
  $(error double inclusion)
endif
JMKE_ENV_INCLUDED=y

export JMK_ROOT
include $(JMK_ROOT)/jmk/debug_root.mak

all:

SHELL=/bin/bash

# include JMKE_MK2 with double inclusion protection
# usage example:
#   GOOD
#   $(call JMKE_INCLUDE_RULES)
#   BAD
#   $(JMKE_INCLUDE_RULES)
JMKE_INCLUDE_RULES_y=
JMKE_INCLUDE_RULES_=$(eval JMKE_MK_INCLUDED=y)$(eval include $(JMKE_MK2))
JMKE_INCLUDE_RULES=$(JMKE_INCLUDE_RULES_$(JMKE_MK_INCLUDED))

CUR_DIR=$(CURDIR)

include $(JMK_ROOT)/jmk/utils_env_root.mak
include $(JMK_ROOT)/jmk/config_env_root.mak
include $(JMK_ROOT)/jmk/build_dir_env_root.mak
include $(JMK_ROOT)/jmk/locations_root.mak

ifndef JMKE_DOING_MAKE_CONFIG
  # check that make config was done successfully
  $(if $(filter $(MAKECMDGOALS),echovar distclean oldconfig),, \
    $(if $(wildcard $(CONFIGURATION_FILE)),,\
      $(error [$(CONFIGURATION_FILE)]Tree is not configured. Run make config)) \
    $(if $(wildcard $(CONFIG_STARTED_FILE)), \
      $(error Tree did not complete configuration successfully. Run make config),))
endif

include $(wildcard $(CONFIG_FILE))
include $(JMK_ROOT)/jmk/envir_subdirs_root.mak
include $(JMK_ROOT)/jmk/flags_env_root.mak
include $(JMK_ROOT)/jmk/ramdisk_env_root.mak
ifdef CONFIG_RG_DLM
  include $(JMK_ROOT)/jmk/dlm_env_root.mak
endif
include $(JMK_ROOT)/jmk/lang_env_root.mak
include $(JMK_ROOT)/jmk/legacy_env_root.mak
ifdef CONFIG_RG_JNET_SERVER
  include $(JMK_ROOT)/jmk/jrms_env.mak
endif
ifdef CONFIG_HEADEND_OSAP
  include $(JMK_ROOT)/jmk/osap_env.mak
endif

ifeq ($(CONFIG_ULIBC)-$(ULIBC_IN_TOOLCHAIN),y-)
  JMKE_PKG_LIBC+=$(JMK_ROOT)/$(ULIBC_DIR)
endif
ifeq ($(CONFIG_GLIBC)-$(GLIBC_IN_TOOLCHAIN),y-)
  JMKE_PKG_LIBC+=$(JMK_ROOT)/pkg/glibc
endif

ifndef JMKE_DOING_MAKE_CONFIG
  ifeq ($(ENV_RG_PARALLEL_MAKE),)
    MAKEFLAGS+=-j$(CONFIG_RG_PARALLEL_MAKE)
  else
    ifeq ($(filter n no,$(ENV_RG_PARALLEL_MAKE)),)
      MAKEFLAGS+=-j$(ENV_RG_PARALLEL_MAKE)
    endif
  endif
endif

# For all subdirs, env_subdir.mak and rg_subdir.mak are being called from the
# subdirs engine. For the root Makefile we need to call them manually.
include $(JMK_ROOT)/jmk/env_subdir.mak
