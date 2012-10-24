SHELL=/bin/bash
# all is the first target.
all:

# make old code ignore the -j of the new flat makefiles
.NOTPARALLEL:

# CURDIR is an automatic variable of make. We will soon need to replace it for
# none recursive make logic (cwd is always starting point of compilation).
CUR_DIR:=$(CURDIR)
include $(JMK_ROOT)/jmk/old/config_env.mak
include $(JMK_ROOT)/jmk/old/build_dir_env.mak

JMK_JPKG_EXPORTED_FILES+=Makefile
JMK_JPKG_EXPORTED_IF_EXIST+=envir.subdirs.mak

include $(JMK_ROOT)/jmk/old/locations.mak
-include $(CONFIG_FILE)

ifdef JMKE_IS_BUILDDIR
  include $(JMK_ROOT)/jmk/old/envir.subdirs.mak
  VPATH:=$(if $(JMK_NEED_VPATH),$(JMKE_PWD_SRC))
endif

include $(JMK_ROOT)/jmk/old/flags_env.mak

MKDIR=mkdir -p

# Return y if GCC_VERSION_NUM is greater or equal to $1
# $1 - GCC version formatted as "major * 1000 + minor"
JMKF_GCC_VER_GE = $(shell [ $(GCC_VERSION_NUM) -ge $1 ] && echo -n y)

include $(JMKE_COPY_DB_ENVIR)
-include $(JMKE_BUILDDIR)/os/kernel/envir.mak

include $(JMK_ROOT)/jmk/old/lang_env.mak
