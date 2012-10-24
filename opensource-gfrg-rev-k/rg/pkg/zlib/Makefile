ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

ifdef CONFIG_DYN_LINK
  LIB_Z=libz.so
  JMK_SO_TARGET=$(LIB_Z)
  JMK_RAMDISK_LIB_FILES+=$(LIB_Z)
else
  LIB_Z=libz.a
  JMK_A_TARGET=$(LIB_Z)
endif

JMK_LOCAL_A_TARGET=liblocal_z.a
JMK_EXPORT_LIBS+=$(LIB_Z) $(JMK_LOCAL_A_TARGET)

JMK_O_OBJS = adler32.o compress.o crc32.o gzio.o uncompr.o deflate.o \
  trees.o zutil.o inflate.o infblock.o inftrees.o infcodes.o infutil.o \
  inffast.o

EXPORT_LIB_SRC = adler32.c zutil.h zutil.c infblock.h inflate.c inftrees.h \
  infcodes.h infutil.h infblock.c inffixed.h inftrees.c inffast.h infcodes.c \
  infutil.c inffast.c

ALL_C+=$(filter %.c,$(EXPORT_LIB_SRC)) uncompr.c deflate.c trees.c

ifndef CONFIG_RG_JPKG_SRC
  ALL_C+=$(JMKE_PWD_BUILD)/zlib_export.c
  JMK_EXPORT_HEADERS+=$(JMKE_PWD_BUILD)/zlib_export.h
endif

JMK_DONT_EXPORT=deflate.o local_deflate.o

JMK_EXPORT_HEADERS_DIR=zlib
JMK_EXPORT_HEADERS+=zlib.h zconf.h trees.h deflate.h \
  $(foreach f,$(ALL_C),$f__c/$(notdir $f)) $(filter %.h,$(EXPORT_LIB_SRC))

#use awk to remove all local includes: #include "*.?"
AWK_RM_INCLUDES='/\#include ["].*[.][hc]["]/ {print "/*",$$0,"*/"} \
  !/\#include ["].*[.][hc]["]/ {print}'
$(JMKE_PWD_BUILD)/zlib_export.h: \
  $(JMKE_PWD_SRC)/zconf.h $(JMKE_PWD_SRC)/zlib.h
	@cat $(JMKE_DEPS) > $@__
	@awk $(AWK_RM_INCLUDES) $@__ > $@
	@rm -f $@__

$(JMKE_PWD_BUILD)/zlib_export.c: $(addprefix $(JMKE_PWD_SRC)/,$(EXPORT_LIB_SRC))
	@cat $(JMKE_DEPS) > $@__
	@echo "#define _ZLIB_EXPORT_C" > $@
	@echo "#include \"zlib.h\"" >> $@  # won't remove by awk
	@awk $(AWK_RM_INCLUDES) $@__ >> $@
	@rm -f $@__

JMK_ARCHCONFIG_FIRST_TASKS+=$(JMKE_PWD_BUILD)/zlib_export.c \
  $(JMKE_PWD_BUILD)/zlib_export.h

$(call JMKE_INCLUDE_RULES)
