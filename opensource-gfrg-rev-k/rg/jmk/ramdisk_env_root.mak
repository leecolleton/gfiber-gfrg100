# TODO clean non-ramdisk related stuff from this file

# these are leftovers from old logic that assumed ramdisk_*.mak can be used
# withouth envir.mak/rg.mak
# maybe we'll need it in the new logic as well?
ifdef RAMDISK_ENV_MAK
  $(error double inclusion of ramdisk_env_root.mak)
endif
RAMDISK_ENV_MAK=1

ifneq ($(wildcard $(JMKE_BUILDDIR)/.rgsrc),)
  JMK_ROOT:=$(shell cat $(JMKE_BUILDDIR)/.rgsrc)
endif

export DISK_IMAGE_DIR=$(JMKE_BUILDDIR)/pkg/build/disk_image
export CRAMFS_DIR=$(DISK_IMAGE_DIR)/cramfs_dir
export MODFS_DIR=$(DISK_IMAGE_DIR)/modfs_dir
export JMKE_RAMDISK_RO_DIR=$(CRAMFS_DIR)
export JMKE_RAMDISK_RW_DIR=$(DISK_IMAGE_DIR)/ramdisk_dir
export RAMDISK_MOUNT_POINT=$(DISK_IMAGE_DIR)/ramdisk_mount_point
export JMKE_RAMDISK_DEV_DIR=$(JMKE_RAMDISK_RW_DIR)/dev
export JMKE_CRAMFS_RUNTIME_MOUNT_POINT=/mnt/cramfs
export MODFS_RUNTIME_MOUNT_POINT=/mnt/modfs

export JMKE_CP=cp -avf
export JMKE_CP_LN=cp -srf
export JMKE_LN=ln -sfn
export RG_SHELL_FUNCS=. $(JMK_ROOT)/jmk/mk_funcs.sh
export JMKE_LN_NOFAIL=$(RG_SHELL_FUNCS) && rg_lnf
export RG_VPATH_CP=$(RG_SHELL_FUNCS) && rg_vpath_cp

export JMKE_MKDIR=mkdir -p
export EXEC_AS_ROOT=$(JMKE_BUILDDIR)/pkg/build/exec_as_root
export JMKE_MKNOD=$(RG_SHELL_FUNCS) && make_node_rm

export JMKE_CHMOD=$(EXEC_AS_ROOT) chmod
export RAMDISK_REPORT_FILE=$(DISK_IMAGE_DIR)/report.txt
# This file exists only on a distribution tree
export DISTRIBUTION_NAME_FILE=$(JMK_ROOT)/.distribution

export JMKE_IS_DISTRIBUTION=$(if $(wildcard $(DISTRIBUTION_NAME_FILE)),y)

export DISK_IMG_FILE=$(JMKE_BUILDDIR)/pkg/build/disk_image_file.txt
JSMIN=$(JMKE_BUILDDIR)/pkg/util/jsmin
DISK_IMAGE_FUNC=echo $(1) " -> " $(2) >> $(DISK_IMG_FILE)

DIST?=$(CONFIG_RG_DIST)

_RAMDISK_CP_FUNC=rm -f $2 && $(JMKE_CP) -L $1 $2
ifndef CONFIG_RG_FILES_ON_HOST
  RAMDISK_CP_FUNC=$(_RAMDISK_CP_FUNC)
else
  RAMDISK_CP_FUNC_LINK= \
    $(JMKE_LN) \
      $(if $(filter $(JMKE_BUILDDIR)/%,$1),\
	$(subst $(JMKE_BUILDDIR),/mnt/tree/build,$1),\
	$(subst $(JMK_ROOT),/mnt/tree/src,$1)) \
      $2
  RAMDISK_CP_FUNC=$(if $(filter %/bin/init,$2),\
    $(_RAMDISK_CP_FUNC),\
    $(call RAMDISK_CP_FUNC_LINK,$(shell readlink -f $1),$2))
endif

RAMDISK_LN_FUNC = \
  $(JMKE_MKDIR) $(dir $2) && $(JMKE_LN) $1 $2 && \
  echo '- $(patsubst $(JMK_ROOT)/%,%,$2) is linked to $1' >> $(RAMDISK_REPORT_FILE)

# By default strip does not remove all useless (for us) sections. If you found 
# such one - add it into the list below
STRIPABLE_SECTIONS=.comment .note .pdr .gnu.warning.get .gnu.warning.tmpnam \
  .gnu.warning.siggetmask .gnu.warning.mktemp .gnu.warning.gets \
  .gnu.warning.__stub2 .gnu.warning.__fpending

STRIP_LIST=$(foreach s, $(STRIPABLE_SECTIONS), -R $s)

ifdef CONFIG_RG_NO_OPT
  RAMDISK_STRIP=true
else
  RAMDISK_STRIP=$(STRIP) $(STRIP_LIST) $1 && echo '- stripping $1' >> \
    $(RAMDISK_REPORT_FILE)
endif
ifdef CONFIG_RG_JSMIN
  RAMDISK_STRIP_JS=$(JSMIN) $1 && echo '- minifing $1' >> $(RAMDISK_REPORT_FILE)
else
  RAMDISK_STRIP_JS=true
endif
RAMDISK_STRIP_DEBUG_SYM=$(STRIP) -g $(STRIP_LIST) $1 && \
  echo '- stripping debug symbols $1' >> $(RAMDISK_REPORT_FILE)
ifdef CONFIG_RG_FILES_ON_HOST
  RAMDISK_STRIP_DEBUG_SYM=true
  RAMDISK_STRIP=true
  RAMDISK_STRIP_JS=true
endif
RAMDISK_NO_STRIP=true

# $1 - source file + path
# $2 - destination path in ramdisk (/lib/modules/one_module.o)
# $3 - name of the strip function
#      RAMDISK_STRIP / RAMDISK_NO_STRIP / RAMDISK_STRIP_DEBUG_SYM /
#      RAMDISK_STRIP_JS
# $4 - base destination dir
# $5 - runtime mount-point dir
ifdef CONFIG_RG_JNET_SERVER
    _RAMDISK_CP_RO_FUNC= \
    $(JMKE_MKDIR) $4/$(dir $2) && \
    $(call RAMDISK_CP_FUNC,$1,$4/$2) && \
    echo '- $(patsubst $(JMK_ROOT)/%,%,$1) -> $5/$2' >> \
      $(RAMDISK_REPORT_FILE)
else
    _RAMDISK_CP_RO_FUNC= \
    $(JMKE_MKDIR) $4/$(dir $2) && \
    $(call RAMDISK_LN_FUNC,$5/$2,$(JMKE_RAMDISK_RW_DIR)/$2) && \
    $(call RAMDISK_CP_FUNC,$1,$4/$2) && \
    echo '- $(patsubst $(JMK_ROOT)/%,%,$1) -> $5/$2' >> \
      $(RAMDISK_REPORT_FILE) && \
    $(call $3,$4/$2) && \
    $(call DISK_IMAGE_FUNC, $(subst $(JMKE_BUILDDIR),'rg',$1), $4$2)
endif
      
# $1 - source file + path
# $2 - destination path in ramdisk (/lib/modules/one_module.o)
# $3 - name of the strip function
#      RAMDISK_STRIP / RAMDISK_NO_STRIP / RAMDISK_STRIP_DEBUG_SYM /
#      RAMDISK_STRIP_JS
#      $3 is only for RAMDISK_CP_RO_FUNC and RAMDISK_CP_MOD_FUNC
#
# it is no longer ok to call RAMDISK_CP_*_FUNC from Makefiles
# use the JMK_RAMDISK_FILES/JMK_RAMDISK_RW_FILES instead!

RAMDISK_CP_RO_FUNC= \
  $(call _RAMDISK_CP_RO_FUNC,$1,$2,$3,$(JMKE_RAMDISK_RO_DIR),$(JMKE_CRAMFS_RUNTIME_MOUNT_POINT),cramfs_dir)

RAMDISK_CP_MOD_FUNC= \
  $(call _RAMDISK_CP_RO_FUNC,$1,$2,$3,$(MODFS_DIR),$(MODFS_RUNTIME_MOUNT_POINT),modfs_dir)

RAMDISK_CP_RW_FUNC= \
  $(JMKE_MKDIR) $(JMKE_RAMDISK_RW_DIR)/$(dir $2) && \
  $(call RAMDISK_CP_FUNC,$1,$(JMKE_RAMDISK_RW_DIR)/$2) && \
  echo '* $(patsubst $(JMK_ROOT)/%,%,$1) -> $2' >> $(RAMDISK_REPORT_FILE) && \
  $(call DISK_IMAGE_FUNC, $(subst $(JMKE_BUILDDIR),'rg',$1), $2)

# formatting for ramdisk
# $1 src list.
# $2 dest path, from the intallation dir root.
# $3 if exists, filename of the target, and $1 must not be a list
# Note: args for this function can be variables that are cleaned during the
# subdirs logic. In order for their value to be saved before they are clean,
# JMK_RAMDISK_FILES must have its real value i.e. :=
JMKF_RAMDISK_FILES= \
  $(eval JMK_RAMDISK_FILES:=$(JMK_RAMDISK_FILES) \
    $(foreach f,$1,$f__$(strip $2)/$(or $(strip $3),$(notdir $f))))

# $1 - source file + path
# $2 - destination path in ramdisk
JMKF_CP_RAMDISK_FILES= \
  $(call RAMDISK_CP_RO_FUNC,$1,$2,RAMDISK_NO_STRIP)

PACKAGE_MANDATORY_FILES=control
PACKAGE_OPTIONAL_FILES=preinst postinst prerm postrm conffiles templates

# $1 - packages names
JMKF_PACKAGES= \
  $(foreach f, $(PACKAGE_MANDATORY_FILES), \
    $(foreach p,$1, \
      $(eval JMK_CD_EXPORTED_FILES+=$f_$p) \
      $(call JMKF_RAMDISK_FILES,$f_$p,$p/DEBIAN,$f))) \
  $(foreach f, $(PACKAGE_OPTIONAL_FILES), \
    $(foreach p,$1,$(if $(wildcard $(JMKE_PWD_SRC)/$f_$p), \
      $(eval JMK_CD_EXPORTED_FILES+=$f_$p) \
      $(call JMKF_RAMDISK_FILES,$f_$p,$p/DEBIAN,$f))))

# $1 - File prefix
# $2 - Destination
# $3 - Package Names
# $4 - File suffix
JMKF_PACKAGE_FILES= \
  $(eval JMK_CD_EXPORTED_FILES+=$(addsuffix $4,$(addprefix $1,$3))) \
  $(foreach p,$3, \
    $(call JMKF_RAMDISK_FILES,$1$p$4,$p$2,$p$4))

# $1 - Package Names
# $2 - Dependencies
JMKF_PACKAGE_DEPENDS= \
  $(eval PKG:=$(subst -,_,JMK_PACAKGE_$1_DEPENDS)) \
  $(foreach d,$2,$(eval export $(PKG)=$($(PKG)), $d))

# $1 - packages names
JMKF_SITES= \
  $(foreach s,$1, \
    $(eval JMK_RAMDISK_DIRS+=$(JMKE_RAMDISK_RO_DIR)/$s/usr/share/$s/htdocs))

# $1 - packages names
JMKF_LOGROTATE= \
  $(call JMKF_PACKAGE_FILES,logrotate_,/etc/logrotate.d,$1)
