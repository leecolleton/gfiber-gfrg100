ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS=cli fib_lookup genl route

JMK_EXPORT_HEADERS_DIR=libnl/netlink
JMK_EXPORT_HEADERS=addr.h attr.h cache-api.h cache.h data.h errno.h \
  handlers.h list.h msg.h netlink-compat.h netlink.h netlink-kernel.h \
  object-api.h object.h socket.h types.h utils.h version.h

$(call JMKE_INCLUDE_RULES)
