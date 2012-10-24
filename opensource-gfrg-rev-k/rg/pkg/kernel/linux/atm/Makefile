JMK_ROOT=../../../..
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_RG_JPKG_MIPSEB
  # don't try compiling jpkgs for linux 2.6
  CONFIG_RG_OS_LINUX_26=
endif

ifdef CONFIG_ATM_CLIP
  JMK_MOD_TARGET=clip_mod.o
  JMK_MOD_2_STAT+=$(JMK_MOD_TARGET)
endif

JMK_O_OBJS_clip_mod.o=clip.o
JMK_CFLAGS_clip.o+=-I$(JMK_ROOT)/pkg

JMK_EXPORT_HEADERS+=clip.h

include $(JMKE_MK)
