JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

JMK_O_TARGET=libalg.o

FREESWANDIR=$(JMK_ROOT)/pkg/freeswan
LIBCRYPTO=$(FREESWANDIR)/libcrypto

JMK_CFLAGS+=-I.. -I$(FREESWANDIR)/lib -I$(FREESWANDIR)/libdes -I$(LIBCRYPTO)

JMK_O_OBJS=ike_alginit.o

ifeq ($(CONFIG_IPSEC_ENC_1DES),y)
  JMK_O_OBJS+=ike_alg_1des.o
endif

ifeq ($(CONFIG_IPSEC_ENC_AES),y)
  JMK_SUBDIRS+=libaes
  JMK_O_OBJS+=ike_alg_aes.o
  JMK_LDFLAGS_$(JMK_O_TARGET)+=-Llibaes -laes
endif

include $(JMKE_MK)
