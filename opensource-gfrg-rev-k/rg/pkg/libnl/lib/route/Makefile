ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS=cls link sch

ifdef CONFIG_DYN_LINK
  LIB_NAME=libnl-route.so
  JMK_SO_TARGET=$(LIB_NAME)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  LIB_NAME=libnl-route.a
  JMK_A_TARGET=$(LIB_NAME)
endif

JMK_EXPORT_LIBS+=$(LIB_NAME)

JMK_O_OBJS_$(LIB_NAME)=addr.o class.o class_api.o class_obj.o neigh.o \
  neightbl.o nexthop.o qdisc.o qdisc_api.o qdisc_obj.o route.o route_obj.o \
  route_utils.o rtnl.o rule.o tc.o pktloc_syntax.o pktloc_grammar.o pktloc.o \
  link.o
JMK_O_OBJS_$(LIB_NAME)+=cls.o cls_api.o cls_obj.o 

JMK_O_OBJS_$(LIB_NAME)+=../fib_lookup/fib_lookup.o
JMK_O_OBJS_$(LIB_NAME)+=sch/route-sch.o
JMK_O_OBJS_$(LIB_NAME)+=link/route-link.o
JMK_O_OBJS_$(LIB_NAME)+=cls/route-cls.o

# have yacc generate pktloc_syntax.h as well
JMK_YFLAGS+=-d
JMK_LEXFLAGS=--header-file=$(JMKE_PWD_BUILD)/pktloc_grammar.h
JMK_CFLAGS_pktloc_grammar.o+=-DYY_NO_INPUT

LIBNL_AUTOGEN_TARGETS+=$(JMKE_PWD_BUILD)/pktloc_grammar.c
LIBNL_AUTOGEN_TARGETS+=$(JMKE_PWD_BUILD)/pktloc_syntax.c
JMK_ARCHCONFIG_FIRST_TASKS+=$(LIBNL_AUTOGEN_TARGETS)

LIBNL_AUTOGEN_FILES=$(LIBNL_AUTOGEN_TARGETS)
LIBNL_AUTOGEN_FILES+=$(JMKE_PWD_BUILD)/pktloc_syntax.h
LIBNL_AUTOGEN_FILES+=$(JMKE_PWD_BUILD)/pktloc_grammar.h

JMK_AUTOGEN_SRC+=$(LIBNL_AUTOGEN_FILES)
JMK_CLEAN+=$(LIBNL_AUTOGEN_FILES)

JMK_INTERNAL_HEADERS=pktloc_grammar.l pktloc_syntax.y

$(call JMKE_INCLUDE_RULES)
