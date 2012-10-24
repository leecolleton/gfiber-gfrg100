# DBGFR: print a function arguments, and return value
# $1 - function body 
# $2 - function name
# $3 - original arg 1
# $4 - original arg 2
# $5 - original arg 3
DBGFR=$(if $(MAKEDEBUG),$(warning $2: ENTER:$$1:[$3] $$2:[$4] $$3:[$5]))$(strip $1)$(if $(MAKEDEBUG),$(warning $2 : RETURN [$(strip $1)]))

$(if $(JMKE_ENV_INCLUDED),$(error JMK: Old rg.mak from new system))
 
include $(JMK_ROOT)/jmk/sanity_check.mak
include $(JMK_ROOT)/jmk/old/osap.mak
include $(JMK_ROOT)/jmk/old/flags.mak
include $(JMK_ROOT)/jmk/old/targets.mak
include $(JMK_ROOT)/jmk/old/log_entity.mak
include $(JMK_ROOT)/jmk/old/sec_perm.mak
include $(JMK_ROOT)/jmk/old/objs.mak
include $(JMK_ROOT)/jmk/old/jpkg_target.mak
include $(JMK_ROOT)/jmk/old/dirs.mak
include $(JMK_ROOT)/jmk/old/lang.mak
ifdef JMKE_IS_BUILDDIR
  include $(JMK_ROOT)/jmk/old/docs.mak
endif
include $(JMK_ROOT)/jmk/old/config.mak
include $(JMK_ROOT)/jmk/old/mod2stat.mak
ifdef JMKE_IS_BUILDDIR
  # All dependencies including all: Are in general_dep.mak
  include $(JMK_ROOT)/jmk/old/general_dep.mak
endif
include $(JMK_ROOT)/jmk/old/compile.mak

ifdef JMKE_IS_BUILDDIR
  include $(JMKE_COPY_DB)
  include $(JMK_ROOT)/jmk/old/jpkg.mak
  include $(JMK_ROOT)/jmk/old/export.mak
  include $(JMK_ROOT)/jmk/old/test.mak
endif

include $(JMK_ROOT)/jmk/old/clean.mak

ifndef JMKE_IS_BUILDDIR
  # do not use default rules, so that we do not generate by mistake any output
  # into the source directory (the non JMKE_BUILDDIR directory).
  # like running "make --no-builtin-rules" 
  .SUFFIXES:

endif # JMKE_IS_BUILDDIR
