ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

ifdef CONFIG_DYN_LINK
  LIB_NAME=libnl-genl.so
  JMK_SO_TARGET=$(LIB_NAME)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  LIB_NAME=libnl-genl.a
  JMK_A_TARGET=$(LIB_NAME)
endif

JMK_EXPORT_LIBS+=$(LIB_NAME)

JMK_O_OBJS_$(LIB_NAME)=ctrl.o family.o genl.o mngt.o

$(call JMKE_INCLUDE_RULES)
