include $(JMK_ROOT)/jmk/old/multiple.mak

IS_LOCAL_OBJ=$(strip $(foreach t,$(LOCAL_TARGETS),$(if $(filter $(notdir $1),$(notdir $(_O_OBJS_$t))),$t,)))
IS_SO_OBJ=$(strip $(foreach t,$(JMK_SO_TARGET),$(if $(filter $(notdir $1),$(notdir $(_O_OBJS_$t))),$t,)))
FIX_OBJ_NAME=$(if $(call IS_SO_OBJ,$1),$($(notdir 1):%_pic.o=%.o),$(patsubst %_mod_24.o,%.o,$(patsubst %_mod_26.o,%.o,$(notdir $1))))

GET_MODULE=$(strip $(foreach m,$(JMK_MOD_TARGET),$(if $(filter $1,$(_O_OBJS_$m) $(_OX_OBJS_$m)),$m)))

IS_XOBJ=$(strip $(foreach m,$(JMK_MOD_TARGET),$(if $(filter $1,$(_OX_OBJS_$m)),$m)))

ifdef JMK_CREATE_LOCAL
  $(foreach t,$(JMK_CREATE_LOCAL),$(eval \
      $(if $(filter $t,$(JMK_TARGET)),JMK_O_OBJS_local_$t,\
          JMK_O_OBJS_$(t:lib%.a=liblocal_%.a)):=$(strip \
          $(if $(JMK_O_OBJS_$t),$(JMK_O_OBJS_$t),$(JMK_O_OBJS)))))

  $(foreach t,$(JMK_CREATE_LOCAL),$(eval \
      $(if $(filter $t,$(JMK_TARGET)),JMK_L_OBJS_local_$t, \
          JMK_L_OBJS_$(t:lib%.a=liblocal_%.a)):=$(strip \
          $(if $(JMK_L_OBJS_$t),$(JMK_L_OBJS_$t),$(JMK_L_OBJS)))))
endif

# Using _[OL]_OBJS_% as the internal variable of all objs for each target.
# This variable contain the real object names for compilation including local_
# prefix for local targets.
# User shouldn't use _O_OBJS or _L_OBJS directly!

FIXLOCALA=$(1:lib%=liblocal_%)
ISA=$(call EQ,.a,$(1:%.a=.a))
# Create _O_OBJS_% for any target either from the JMK_O_OBJS_% or the default
# JMK_O_OBJS

ADD_LOCAL_PREFIX=$(foreach o,$1,$(dir $o)$(if $(call ISA,$o),$(call FIXLOCALA,$(notdir $o)),local_$(notdir $o)))

_TARGET_O_OBJS=$(if $(JMK_O_OBJS_$1),$(JMK_O_OBJS_$1),$(JMK_O_OBJS))
_TARGET_O_OBJS_WITH_LOCAL=$(strip \
  $(if $(filter $1,$(LOCAL_TARGETS)), \
    $(call ADD_LOCAL_PREFIX,$(_TARGET_O_OBJS)), \
    $(_TARGET_O_OBJS)\
  )\
)
# Fix problem of .// in the obj name. (x.c, ./x.c, .//x.c are different names)
# TODO: this operates only on the first obj, check if needed for others
_TARGET_O_OBJS_FIXED1=$(_TARGET_O_OBJS_WITH_LOCAL:.//%=%)
_TARGET_O_OBJS_FIXED2=$(_TARGET_O_OBJS_FIXED1:./%=%)

$(foreach t,$(ALL_TARGETS),\
    $(eval _O_OBJS_$t:=$(call _TARGET_O_OBJS_FIXED2,$t)))

# Change module objects from x.o to x_mod_24.o and/or x_mod_26.o
__MOD_OBJS=$(if $(JMK_O_OBJS_$t),$(JMK_O_OBJS_$t),$(JMK_O_OBJS))
$(foreach t, $(JMK_MOD_TARGET),\
    $(eval _MOD_O_OBJS_$t:=$(strip \
        $(if $(CONFIG_RG_OS_LINUX_24),$(__MOD_OBJS:%.o=%_mod_24.o))\
        $(if $(CONFIG_RG_OS_LINUX_26),$(__MOD_OBJS:%.o=%_mod_26.o)))))

ifdef CONFIG_RG_OS_LINUX_24
__MOD_24_OBJS=$(if $(JMK_MOD_24_O_OBJS_$t),$(JMK_MOD_24_O_OBJS_$t),$(JMK_MOD_24_O_OBJS))
$(foreach t,$(JMK_MOD_TARGET), \
    $(eval _MOD_24_O_OBJS_$t:=$(__MOD_24_OBJS:%.o=%_mod_24.o)))
endif

ifdef CONFIG_RG_OS_LINUX_26
__MOD_26_OBJS=$(if $(JMK_MOD_26_O_OBJS_$t),$(JMK_MOD_26_O_OBJS_$t),$(JMK_MOD_26_O_OBJS))
$(foreach t,$(JMK_MOD_TARGET), \
    $(eval _MOD_26_O_OBJS_$t:=$(__MOD_26_OBJS:%.o=%_mod_26.o)))
endif

$(foreach t,$(JMK_MOD_TARGET), $(eval \
    _O_OBJS_$t:=$(_MOD_O_OBJS_$t) $(_MOD_24_O_OBJS_$t) $(_MOD_26_O_OBJS_$t)))

# Create _O_OBJS_%_pic
__PIC_OBJS=$(if $(JMK_O_OBJS_$t),$(JMK_O_OBJS_$t),$(JMK_O_OBJS))
$(foreach t,$(JMK_SO_TARGET) $(if $(JMK_A_TARGET_PIC), $(JMK_A_TARGET)), \
    $(eval _O_OBJS_$t:=$(__PIC_OBJS:%.o=%_pic.o)))

# Create _L_OBJS_%
__L_OBJS=$(if $(JMK_L_OBJS_$t),$(JMK_L_OBJS_$t),$(JMK_L_OBJS))
$(foreach t,$(ALL_TARGETS), \
    $(eval _L_OBJS_$t:=$(strip $(if $(filter $t,$(LOCAL_TARGETS)), \
        $(call ADD_LOCAL_PREFIX,$(__L_OBJS)), \
        $(__L_OBJS)))))

# Create _OX_OBJS_% (for JMK_MOD_TARGET)
__MOD_OXBJS=$(if $(JMK_OX_OBJS_$t),$(JMK_OX_OBJS_$t),$(JMK_OX_OBJS))
$(foreach t,$(JMK_MOD_TARGET), \
    $(eval _MOD_OX_OBJS_$t:=$(strip \
    $(if $(CONFIG_RG_OS_LINUX_24),$(__MOD_OXBJS:%.o=%_mod_24.o)) \
    $(if $(CONFIG_RG_OS_LINUX_26),$(__MOD_OXBJS:%.o=%_mod_26.o)))))

ifdef CONFIG_RG_OS_LINUX_24
__MOD_24_OXBJS=$(if $(MOD_24_OX_OBJS_$t),$(MOD_24_OX_OBJS_$t),$(MOD_24_OX_OBJS))
$(foreach t,$(JMK_MOD_TARGET), \
    $(eval _MOD_24_OX_OBJS_$t:=$(__MOD_24_OXBJS:%.o=%_mod_24.o)))
endif

ifdef CONFIG_RG_OS_LINUX_26
__MOD_26_OXBJS=$(if $(MOD_26_OX_OBJS_$t),$(MOD_26_OX_OBJS_$t),$(MOD_26_OX_OBJS))
$(foreach t,$(JMK_MOD_TARGET), \
    $(eval _MOD_26_OX_OBJS_$t:=$(__MOD_26_OXBJS:%.o=%_mod_26.o)))
endif

$(foreach t,$(JMK_MOD_TARGET), $(eval \
    _OX_OBJS_$t:=$(_MOD_OX_OBJS_$t) $(_MOD_24_OX_OBJS_$t) $(_MOD_26_OX_OBJS_$t)))

_OTHER_OBJS=$(JMK_OTHER_OBJS)

# Create the TARGET_CFLAGS_% cflags. This variable is the JMK_CFLAGS_target of the
# target that %.o is connected to.
# Note that if the same object is used for more than one target the last 
# target JMK_CFLAGS will overrun the previes cflags.
$(foreach t,$(ALL_TARGETS), \
    $(foreach o,$(JMK_O_OBJS_$t) $(JMK_OX_OBJS_$t), \
        $(eval TARGET_CFLAGS_$o:=$(JMK_CFLAGS_$t))))
