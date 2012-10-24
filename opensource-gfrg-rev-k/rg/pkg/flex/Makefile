ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_CFLAGS+=-O
JMK_LOCAL_CFLAGS+=-O

JMK_A_TARGET=libfl.a
JMK_EXPORT_LIBS+=$(JMK_A_TARGET)
JMK_TARGET=flex
JMK_WARN2ERR=n

JMK_RAMDISK_BIN_FILES=flex

JMK_CREATE_LOCAL=$(JMK_A_TARGET) $(JMK_TARGET)

JMK_O_OBJS_flex+=ccl.o dfa.o ecs.o gen.o main.o misc.o nfa.o skel.o \
  sym.o tblcmp.o

ifndef CONFIG_RG_JPKG_SRC
  JMK_O_OBJS_flex+=parse.o scan.o
endif

JMK_O_OBJS_flex+=yylex.o

ifneq ($(JMKE_IS_DISTRIBUTION),y)
  $(JMKE_PWD_BUILD)/yylex.o $(JMKE_PWD_BUILD)/scan.o: $(JMKE_PWD_BUILD)/parse.c
endif

JMK_O_OBJS_libfl.a=libmain.o libyywrap.o

JMK_CD_EXPORTED_FILES+=flexdef.h config.h
JMK_AUTOGEN_SRC=parse.h parse.c scan.c
JMK_CLEAN+=$(JMK_AUTOGEN_SRC)
JMK_INTERNAL_HEADERS+=version.h parse.y scan.l

JMK_YFLAGS+=-d

$(call JMKE_INCLUDE_RULES)
