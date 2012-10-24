JMK_RAMDISK_MODULES_FILES+=$(filter-out $(JMK_RAMDISK_MODULES_PERMANENT_FILES),$(ALL_MOBJS))

ifndef JMK_RAMDISK_SUBDIRS
JMK_RAMDISK_SUBDIRS=$(JMK_SUBDIRS) $(MOD_SUB_DIRS)
endif

# XXX: Remove this when the kernel makefiles are fixed. The kernel makefiles
# currently do not include envir.mak, where these defs are originally from
JMKE_IS_BUILDDIR:=$(if $(findstring $(JMKE_BUILDDIR),$(shell /bin/pwd)),y)
MKDIR=mkdir -p

export JMKE_PWD_SRC:=$(if $(JMKE_IS_BUILDDIR),$(CURDIR:$(JMKE_BUILDDIR)%=$(JMK_ROOT)%),$(CURDIR))
export JMKE_PWD_BUILD:=$(if $(JMKE_IS_BUILDDIR),$(CURDIR),$(CURDIR:$(JMK_ROOT)%=$(JMKE_BUILDDIR)%))
export PWD_JPKG:=$(CURDIR:$(if $(JMKE_IS_BUILDDIR),$(JMKE_BUILDDIR),$(JMK_ROOT))%=$(JPKG_DIR)%)

export PWD_REL:=$(if $(call EQ,$(CURDIR),$(JMKE_BUILDDIR)),./,$(subst $(JMKE_BUILDDIR)/,,$(CURDIR)))
export JPKG_CUR_DIR=$(JPKG_DIR)/$(PWD_REL)

include $(JMKE_COPY_DB)
