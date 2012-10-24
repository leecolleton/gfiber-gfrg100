# Makefile -- Makefile for util-linux Linux utilities


JMK_ROOT := ../../../
include ../MCONFIG
JMK_LDFLAGS = -lcrypt

all: login

login: login.o checktty.o
	$(CC) $(JMK_LDFLAGS) -o $@ $< ../lib/setproctitle.o checktty.o 

clean: dummy
	rm -f login *.o

dummy:
