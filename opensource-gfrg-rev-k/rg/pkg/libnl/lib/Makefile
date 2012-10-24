ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS=fib_lookup genl route

ifdef CONFIG_DYN_LINK
  LIB_NAME=libnl.so
  JMK_SO_TARGET=$(LIB_NAME)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  LIB_NAME=libnl.a
  JMK_A_TARGET=$(LIB_NAME)
endif

JMK_EXPORT_LIBS+=$(LIB_NAME)

JMK_O_OBJS_$(LIB_NAME)=addr.o attr.o cache.o cache_mngr.o cache_mngt.o data.o \
  doc.o error.o handlers.o msg.o nl.o object.o socket.o utils.o

$(call JMKE_INCLUDE_RULES)
