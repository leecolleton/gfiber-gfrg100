ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

# We may enter to this package when glibc is required and|or
# syslog is required.


# There is a bug (B28967) in glibc/ulibc, which is fixed in pkg/ulibc.
# Workaround this bug in other libcs (glibc, or ulibc from toolchain) by using
# code from this glibc directory.
ifdef CONFIG_RG_SYSLOG_GLIBC
  JMK_SUBDIRS+=glibc
endif

ifndef CONFIG_RG_GPL

JMK_EXPORT_HEADERS_DIR=syslog
JMK_EXPORT_HEADERS+=syslog.h log_dev.h rg_log.h klog.h

ifeq ($(CONFIG_RG_SYSLOG),y)
  JMK_O_TARGET=sys_log.o
  JMK_O_OBJS_$(JMK_O_TARGET)=syslog.o

  ifdef CONFIG_RG_LOG_DEV
    JMK_O_OBJS_$(JMK_O_TARGET)+=rg_log.o
    JMK_MOD_TARGET=log_chardev.o
    JMK_RAMDISK_MODULES_FILES+=$(JMK_MOD_TARGET)
    JMK_OX_OBJS_$(JMK_MOD_TARGET)=log_dev.o
  endif

  ifeq ($(CONFIG_RG_LOG_DEV),y)
    JMK_MOD_2_STAT+=$(JMK_MOD_TARGET)
  endif

  ifdef CONFIG_RG_KERN_LOG
    JMK_O_OBJS_$(JMK_O_TARGET)+=klog.o
    JMK_DIST_LOG_ENTITY+=LOG_ENTITY_KERNEL
  endif

  JMK_CD_EXPORTED_FILES+=log_dev.h

  JMK_SUBDIRS+=main cli
  JMK_L_OBJS_$(JMK_O_TARGET)=main/syslog_main.o cli/cli.o

  ifneq ($(CONFIG_RG_WBM),)
    JMK_SUBDIRS+=wbm
    JMK_JPKG_EXPORTED_DIR+=wbm
    JMK_L_OBJS_$(JMK_O_TARGET)+=wbm/wbm.o
  endif

  ifneq ($(CONFIG_RG_SSI),)
    JMK_SUBDIRS+=ssi
    JMK_L_OBJS_$(JMK_O_TARGET)+=ssi/ssi.o
  endif
endif

endif

$(call JMKE_INCLUDE_RULES)
