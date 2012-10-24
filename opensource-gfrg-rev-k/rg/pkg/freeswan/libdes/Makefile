JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_A_TARGET=libdes.a

JMK_CFLAGS+=-O3 -fomit-frame-pointer -DLIBDES_LIT -DRAND -DTERMIO

DES_ENC=des_enc.o fcrypt_b.o
JMK_O_OBJS=cbc_enc.o ecb_enc.o $(DES_ENC) fcrypt.o set_key.o

JMK_EXPORT_HEADERS_DIR=freeswan/libdes
JMK_EXPORT_HEADERS+=des.h des_locl.h des_ver.h podd.h sk.h spr.h

JMK_CD_EXPORTED_FILES=$(call VPATH_WILDCARD,*.[ch])

include $(JMKE_MK)
