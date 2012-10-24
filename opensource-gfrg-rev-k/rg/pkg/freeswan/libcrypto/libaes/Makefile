JMK_ROOT=../../../../../
include $(JMK_ROOT)/envir.mak

JMK_A_TARGET=libaes.a
JMK_CFLAGS_$(JMK_A_TARGET)+=-I$(JMK_ROOT)/pkg/freeswan/libcrypto/include
JMK_O_OBJS_$(JMK_A_TARGET)=aes_cbc.o aes.o

include $(JMKE_MK)

