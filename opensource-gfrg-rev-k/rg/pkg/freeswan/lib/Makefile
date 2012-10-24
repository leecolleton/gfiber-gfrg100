JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

ifdef CONFIG_DYN_LINK
  JMK_SO_TARGET = libfreeswan.so
  LIB = $(JMK_SO_TARGET)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  JMK_A_TARGET = libfreeswan.a
  LIB = $(JMK_A_TARGET)
endif

OBJS=ultoa.o addrtoa.o subnettoa.o subnetof.o goodmask.o datatot.o \
  prng.o rangetoa.o satoa.o pfkey_v2_parse.o pfkey_v2_build.o \
  pfkey_v2_ext_bits.o pfkey_v2_debug.o 

JMK_O_OBJS_$(LIB)=$(OBJS) addrtot.o addrtypeof.o anyaddr.o atoaddr.o \
  atoasr.o atosa.o atosubnet.o atoul.o initaddr.o initsaid.o initsubnet.o \
  keyblobtoid.o optionsfrom.o portof.o rangetosubnet.o sameaddr.o satot.o \
  subnettot.o subnettypeof.o ttoaddr.o ttodata.o ttosa.o ttosubnet.o ttoul.o \
  ultot.o ttoprotoport.o version.o copyright.o

JMK_CFLAGS+=-O3 -I. -I../libdes -I../../gmp -Wbad-function-cast -Wpointer-arith -Wcast-qual -Wstrict-prototypes
ifeq ($(CONFIG_IPSEC_NAT_TRAVERSAL),y)
    JMK_CFLAGS+=-DNAT_TRAVERSAL
endif
JMK_LDFLAGS+=-L../libdes/
JMK_LDLIBS:=-ldes $(JMK_LDLIBS)

override JMK_CFLAGS+=-I. -Wall -Wconversion -Wmissing-prototypes -Wpointer-arith \
		 -Wcast-qual -Wmissing-declarations -Wstrict-prototypes \
		 -Wbad-function-cast 

JMK_EXPORT_HEADERS+=freeswan.h
#header files are included with <> so the automatic dependancy will not get 
#them
JMK_CD_EXPORTED_FILES=Makefile.kernel $(call VPATH_WILDCARD,*.[ch])

include $(JMKE_MK)

