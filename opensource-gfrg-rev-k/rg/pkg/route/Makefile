ifndef JMKE_ENV_INCLUDED
  JMK_ROOT= ../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

ifndef CONFIG_RG_GPL

JMK_A_TARGET=libroute.a

JMK_SUBDIRS=main
JMK_L_OBJS_$(JMK_A_TARGET)+=main/route_main.o

ifdef CONFIG_RG_WBM
  JMK_SUBDIRS+=wbm
  JMK_JPKG_EXPORTED_DIR+=wbm
  JMK_L_OBJS_$(JMK_A_TARGET)+=wbm/wbm.o
endif

endif

ifneq ($(CONFIG_RG_QUAGGA),)
  JMK_SUBDIRS+=quagga
endif

$(call JMKE_INCLUDE_RULES)
