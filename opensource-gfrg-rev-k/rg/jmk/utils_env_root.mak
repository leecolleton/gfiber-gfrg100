# General help function and variables.

## username function

JMKE_WHOAMI=$(shell whoami)

# $1 username
# returns the username if it is the current user. Else empty variable
JMKF_CHECK_USER=$(filter $1,$(call GET_REAL_USER))

# Exit if user is root
JMKE_ERROR_FOR_ROOT=$(if $(call JMKF_CHECK_USER,root), \
  $(error JMK: Please do not execute as root.))

# Return y if GCC_VERSION_NUM is greater or equal to $1
# $1 - GCC version formatted as "major * 1000 + minor"
JMKF_GCC_VER_GE = $(shell [ $(GCC_VERSION_NUM) -ge $1 ] && echo -n y)

## internal functions

# add/del local prefix
ADD_LOCAL_A = $(1:lib%=liblocal_%)
DEL_LOCAL_A = $(1:liblocal_%=lib%)
ADD_LOCAL = local_$1
DEL_LOCAL = $(1:local_%=%)
ISA = $(filter %.a,$1)
ADD_LOCAL_PREFIX = \
  $(foreach o,$1,$(dir $o)$(call \
    $(if $(call ISA,$o),ADD_LOCAL_A,ADD_LOCAL),$(notdir $o)))
DEL_LOCAL_PREFIX = \
  $(foreach o,$1,$(dir $o)$(call \
    $(if $(call ISA,$o),DEL_LOCAL_A,DEL_LOCAL),$(notdir $o)))

# separate <src>__<target>

# $1 - file in the <src>__<target> format
# $2 - 1 - return the src, 2 return the target
_GET_FILE_FROM_TO=$(word $2,$(subst __, ,$1))
GET_FILE_FROM_TO=$(if $(call IS_FROM_TO,$1),$(call _GET_FILE_FROM_TO,$1,$2),$1)
IS_FROM_TO=$(findstring __,$1)
# $1 - files in the <src>__<target> format
# $2 - path
GET_FILE_FROM=$(call Rel_Path_To_Abs,$(call GET_FILE_FROM_TO,$1,1),$2)
GET_FILE_TO=$(if $(call IS_FROM_TO,$1),$(call _GET_FILE_FROM_TO,$1,2),$(notdir $1))
