JMK_ROOT=../../../../../../../
include $(JMK_ROOT)/envir.mak

JMK_MOD_O_TARGET=libsha1.o
JMK_CFLAGS_$(JMK_MOD_O_TARGET)+=-I../../libcrypto/include -DNO_ASM
JMK_O_OBJS_$(JMK_MOD_O_TARGET)=sha_dgst.o sha1_one.o hmac_sha1.o

include $(JMKE_MK)
