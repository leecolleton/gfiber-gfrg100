JMK_ROOT=$(IPROUTE2SRC)/../..
include $(JMK_ROOT)/envir.mak

JMK_CFLAGS+=-D_GNU_SOURCE -I$(IPROUTE2SRC)/include -DRESOLVE_HOSTNAMES
JMK_LDFLAGS+=-L$(IPROUTE2SRC)/lib -lnetlink -lutil

LIBNETLINK=$(IPROUTE2SRC)/lib/libnetlink.a $(IPROUTE2SRC)/lib/libutil.a

