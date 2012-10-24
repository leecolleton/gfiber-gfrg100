JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_TARGET=hwclock
JMK_O_OBJS=hwclock.o cmos.o rtc.o kd.o shhopt.o
JMK_CFLAGS+=-I. -I../lib -O2

include $(JMKE_MK)
