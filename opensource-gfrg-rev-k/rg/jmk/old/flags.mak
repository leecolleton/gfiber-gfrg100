export JMK_WARN2ERR

ifeq ($(JMK_WARN2ERR),)
  JMK_WARN2ERR=y
endif

ifeq ($(JMK_WARN2ERR),y)
  WARN2ERR_MARK=y
endif

# JMK_MOD_CFLAGS is used both in _mod_[24|26].o and module.o
ifdef WARN2ERR_MARK
  JMK_CFLAGS+=-Werror
  JMK_LOCAL_CFLAGS+=-Werror
  JMK_MOD_CFLAGS+=-Werror
endif

# this works around many places in rg tree which contain this line:
# #include <file.h> 
# where file.h is in the current directory (the correct way is to use "", 
# and then there's no need for this JMK_CFLAGS fix)
JMK_CFLAGS+=-I.
JMK_LOCAL_CFLAGS+=-I.
JMK_MOD_CFLAGS+=-I.

ifdef CONFIG_RG_OS_LINUX
  MODFLAGS+=-DMODULE
  ifdef CONFIG_RG_OS_LINUX_24
    MOD_24_CFLAGS+=-DKBUILD_BASENAME=$(@:%.o=%) 
  endif
  ifdef CONFIG_RG_OS_LINUX_26
    MOD_26_CFLAGS+=-D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR($(@:%.o=%))" -D"KBUILD_MODNAME=KBUILD_STR($(*F))"
  endif
endif

JMK_RG_LINUX_CFLAGS+=$(JMK_LINUX_CFLAGS)

ifdef CONFIG_RG_OS_LINUX
  GET_MODFLAGS=$(call DBGFR,\
    $(JMK_RG_LINUX_CFLAGS) $(JMK_MOD_CFLAGS) \
    $(if $(filter $(call GET_MODULE,$1),$(JMK_MOD_2_STAT)),\
      ,\
      $(MODFLAGS) \
    ) \
    $(if $(filter %_mod_24.o,$1),$(MOD_24_CFLAGS))\
    $(if $(filter %_mod_26.o,$1),$(MOD_26_CFLAGS))\
    $(if $(call IS_XOBJ,$1),-DEXPORT_SYMTAB)\
  ,GET_MODFLAGS,$1)
else
  GET_MODFLAGS=-D__KERNEL__ $(JMK_CFLAGS) $(JMK_MOD_CFLAGS)
endif

JMK_MOD_CFLAGS+=-I$(JMK_ROOT)/pkg/include

_ULIBC_FLAG=--rg-use-ulibc
_GLIBC_FLAG=--rg-use-glibc

# $1 - existing flags
ifdef CONFIG_RG_JPKG_BIN
  _IS_COMPILING_ULIBC=$(if $(filter $(_ULIBC_FLAG),$1),yes)
else
  _IS_COMPILING_ULIBC=$(CONFIG_ULIBC)
endif

# $1 - existing flags
# $2 - obj name
# $3 - src
# - add the libc flag only if needed
# - JMK_RG_ULIBC_CFLAGS & JMK_RG_GLIBC_CFLAGS are already added in rg_gcc for c files, 
#   therefore they are added here only for cpp files.
_GET_CFLAGS_ULIBC=$(call DBGFR,\
  $(if $(call _IS_CPP_FILE,$3),\
    $(JMK_RG_ULIBC_CFLAGS),\
    $(if $(filter $(_ULIBC_FLAG),$1),\
      ,\
      $(_ULIBC_FLAG)\
    )\
  )\
  $(JMK_CFLAGS) \
,_GET_CFLAGS_ULIBC,$1,$2,$3)

_GET_CFLAGS_GLIBC=$(call DBGFR,\
  $(if $(call _IS_CPP_FILE,$3),\
    $(JMK_RG_GLIBC_CFLAGS),\
    $(if $(filter $(_GLIBC_FLAG),$1),\
      ,\
      $(_GLIBC_FLAG)\
    )\
  )\
  $(JMK_CFLAGS) \
,_GET_CFLAGS_GLIBC,$1,$2,$3)

# $1 - existing flags
# $2 - obj name
# $3 - src
# - locals don't pass through rg_gcc, so we fix the include path here.
_GET_OBJ_CFLAGS=$(call DBGFR,\
  $(if $(call _IS_CPP_FILE,$3),\
    $(JMK_CXXFLAGS) $(filter-out $(_ULIBC_FLAG) $(_GLIBC_FLAG),$1),\
    $1\
  )\
  $(if $(call IS_LOCAL_OBJ,$2),\
    $(JMK_LOCAL_CFLAGS) -I$(RG_BUILD)/local/include,\
    $(if $(call GET_MODULE,$2),\
      $(call GET_MODFLAGS,$2),\
      $(if $(call _IS_COMPILING_ULIBC,$1),\
        $(call _GET_CFLAGS_ULIBC,$1,$2,$3),\
        $(call _GET_CFLAGS_GLIBC,$1,$2,$3)\
      )\
    )\
  )\
  -DENTITY_ID=$(ENTITY_ID)\
  $(if $(GET_SEC_PERM_ID),-DSEC_PERM_ID=$(GET_SEC_PERM_ID))\
  $(TARGET_CFLAGS_$(call FIX_OBJ_NAME,$2)) $(JMK_CFLAGS_$(call FIX_OBJ_NAME,$2))\
  ,_GET_OBJ_CFLAGS,$1,$2,$3)

# $1 - existing flags
# $2 - obj name
# $3 - src
# linux 2.6 includes are already fixed in config_host.c, don't fix them again
GET_OBJ_CFLAGS=$(call DBGFR,\
  $(call FIX_VPATH_CFLAGS_EXCEPT_FOR,$(filter-out $(JMK_CFLAGS_REMOVE_$2),\
  $(if $(filter %_mod_24.o,$2),\
  $(filter-out $(RG_LINUX_26_CFLAGS),$(call _GET_OBJ_CFLAGS,$1,$2,$3)),\
  $(if $(filter %_mod_26.o,$2),\
  $(filter-out $(RG_LINUX_24_CFLAGS),$(call _GET_OBJ_CFLAGS,$1,$2,$3)),\
  $(if $(filter %.cpp,$3),\
  $(filter-out -Wstrict-prototypes,$(call _GET_OBJ_CFLAGS,$1,$2,$3)),\
  $(call _GET_OBJ_CFLAGS,$1,$2,$3))))),\
  $(if $(filter %_mod_26.o,$2),$(RG_LINUX_26_CFLAGS)))\
,GET_OBJ_CFLAGS,$1,$2,$3)

# PIC files
FPIC_FLAG=-fpic

