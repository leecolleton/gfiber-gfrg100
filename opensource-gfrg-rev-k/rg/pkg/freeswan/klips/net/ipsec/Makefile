# Makefile for KLIPS kernel code
# Copyright (C) 1998, 1999  Richard Guy Briggs.
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
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The JMK_CFLAGS definition is now in the main makefile...

JMK_ROOT=../../../../..
include $(JMK_ROOT)/envir.mak

JMK_SUBDIRS = zlib libfreeswan

JMK_INTERNAL_HEADERS += ipsec_compat.h

JMK_MOD_TARGET = ipsec.o
JMK_O_OBJS_$(JMK_MOD_TARGET) = ipsec_init.o sysctl_net_ipsec.o \
	pfkey_v2.o pfkey_v2_parser.o ipsec_sa.o ipsec_life.o ipsec_proc.o \
	ipsec_rcv_common.o ipsec_tunnel_common.o

JMK_OX_OBJS_$(JMK_MOD_TARGET) = radij.o ipsec_log.o ipsec_radij.o
JMK_CD_EXPORTED_FILES = radij.h ipsec_encap.h ipsec_netlink.h ipsec_kversion.h \
  ipsec_log.h ipsec_radij.h ipsec_tunnel.h ipsec_rcv_common.h \
  ipsec_tunnel_common.h

JMK_EXPORT_HEADERS_DIR=freeswan/klips/net/ipsec
JMK_EXPORT_HEADERS+=ipsec_encap.h ipsec_tunnel.h radij.h ipsec_radij.h \
                ipsec_eroute.h

JMK_MOD_CFLAGS += -Ilibfreeswan

ifeq ($(CONFIG_IPSEC_NAT_TRAVERSAL),y)
  JMK_MOD_CFLAGS+=-DNAT_TRAVERSAL
endif

ifeq ($(CONFIG_IPSEC_USE_IXP4XX_CRYPTO),y)
  JMK_EXPORT_HEADERS+=ipsec_glue_mbuf.c__c/ipsec_ixp_glue_mbuf.c \
    ipsec_hwaccel.c__c/ipsec_ixp_hwaccel.c
  JMK_CD_EXPORTED_FILES+=ipsec_hwaccel.h
endif

ifeq ($(CONFIG_IPSEC_USE_IXP4XX_OLD_CRYPTO),y)
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipsec_glue.o ipsec_glue_mbuf.o ipsec_glue_desc.o \
	ipsec_rcv_hwaccel.o
  JMK_OX_OBJS_$(JMK_MOD_TARGET) += ipsec_hwaccel.o ipsec_tunnel_hwaccel.o
  IXP425_SW := $(JMK_ROOT)/vendor/intel/ixp425/csr
  JMK_MOD_CFLAGS += -I. -I$(IXP425_SW)/include -I$(IXP425_SW)/linux
  # Added for the possibility to remove HW Acceleration
  JMK_CD_EXPORTED_FILES += ipsec_hwaccel.h
else
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipsec_rcv.o
  JMK_OX_OBJS_$(JMK_MOD_TARGET) += ipsec_tunnel.o
endif

ifeq ($(CONFIG_IPSEC_DEBUG),y)
  JMK_MOD_CFLAGS += -g
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipsec_reject_debug.o
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

ifeq ($(CONFIG_IPSEC_AUTH_HMAC_MD5),y)
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipsec_md5c.o
endif

ifeq ($(CONFIG_IPSEC_AUTH_HMAC_SHA1),y)
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipsec_sha1.o
endif

ifeq ($(CONFIG_IPSEC_IPCOMP),y)
  JMK_O_OBJS_$(JMK_MOD_TARGET) += ipcomp.o 
  JMK_L_OBJS += zlib/zlib.o
endif

ifneq ($(CONFIG_IPSEC_ENC_1DES)$(CONFIG_IPSEC_ENC_3DES),)
  JMK_SUBDIRS += libdes
  JMK_MOD_CFLAGS += -Ilibdes
  JMK_L_OBJS += libdes/libdes.o
endif

JMK_L_OBJS += libfreeswan/libfreeswan.o

ifeq ($(CONFIG_IPSEC_DRIVER),y)
  JMK_MOD_2_STAT+=$(JMK_MOD_TARGET)
else
  JMK_RAMDISK_MODULES_FILES+=$(JMK_MOD_TARGET)
endif

ifeq ($(CONFIG_RG_IPSEC_ESP_ALG),y)
  JMK_SUBDIRS += alg
  JMK_OX_OBJS_$(JMK_MOD_TARGET) += ipsec_alg.o
  JMK_L_OBJS += alg/ipsec_alg_static.o
endif

ifdef CONFIG_RG_GPL
JMK_MOD_TARGET:=
endif

include $(JMKE_MK)
