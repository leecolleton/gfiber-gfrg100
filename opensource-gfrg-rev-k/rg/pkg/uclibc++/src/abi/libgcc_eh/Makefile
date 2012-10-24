JMK_ROOT=../../../../..
include $(JMK_ROOT)/envir.mak

JMK_ARCHCONFIG_FIRST_TASKS=libgcc_eh

libgcc_eh:: unwind-c.o

unwind-c.o::
	$(AR) x $(shell $(CC) -print-file-name=libgcc_eh.a)

include $(JMKE_MK)
