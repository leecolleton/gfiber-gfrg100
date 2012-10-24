JMK_ROOT=../../../../../../../
include $(JMK_ROOT)/envir.mak

JMK_MOD_O_TARGET=libmd5.o

JMK_CFLAGS_$(JMK_MOD_O_TARGET)+=-I../../libcrypto/include -DNO_ASM
JMK_O_OBJS_$(JMK_MOD_O_TARGET)=md5_dgst.o md5_one.o hmac_md5.o

include $(JMKE_MK)
