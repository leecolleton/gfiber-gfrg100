# (kernel) Makefile for IPCOMP zlib deflate code
# Copyright (C) 1998, 1999, 2000, 2001  Richard Guy Briggs.
# Copyright (C) 2000  Svenning Soerensen
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#

JMK_ROOT=../../../../../..
include $(JMK_ROOT)/envir.mak

JMK_MOD_O_TARGET = zlib.o
JMK_O_OBJS = adler32.o deflate.o infblock.o infcodes.o inffast.o inflate.o \
	 inftrees.o infutil.o trees.o zutil.o

ifeq ($(CONFIG_IPSEC_DEBUG),y)
  JMK_MOD_CFLAGS += -g
endif

JMK_MOD_CFLAGS += -Wall 
# JMK_MOD_CFLAGS += -Wconversion 
# JMK_MOD_CFLAGS += -Wmissing-prototypes 
JMK_MOD_CFLAGS += -Wpointer-arith 
# JMK_MOD_CFLAGS += -Wcast-qual 
# JMK_MOD_CFLAGS += -Wmissing-declarations 
JMK_MOD_CFLAGS += -Wstrict-prototypes
# JMK_MOD_CFLAGS += -pedantic
# JMK_MOD_CFLAGS += -O3
# JMK_MOD_CFLAGS += -W
# JMK_MOD_CFLAGS += -Wwrite-strings 
JMK_MOD_CFLAGS += -Wbad-function-cast 
JMK_MOD_CFLAGS += -DIPCOMP_PREFIX -DLOCAL_INFLATE_COPYRIGHT

ifdef CONFIG_M586
  JMK_O_OBJS += match586.o
  JMK_MOD_CFLAGS += -DASMV
endif

ifdef CONFIG_M586TSC
  JMK_O_OBJS += match586.o
  JMK_MOD_CFLAGS += -DASMV
endif

ifdef CONFIG_M686
  JMK_O_OBJS += match686.o
  JMK_MOD_CFLAGS += -DASMV
endif

include $(JMKE_MK)
