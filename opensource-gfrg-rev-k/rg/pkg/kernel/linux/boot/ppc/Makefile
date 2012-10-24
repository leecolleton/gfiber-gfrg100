JMK_ROOT=../../../../../
include $(JMK_ROOT)/envir.mak

include envir.mk

SERIAL=serial_mpc8349itx.o
OBJS_FOR_JPKG=head.o $(SERIAL)
OBJS=$(OBJS_FOR_JPKG)
JMK_CD_EXPORTED_FILES+=$(SERIAL:%.o=%.c) ld.script

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR

boot.o: $(OBJS)
	$(LD) -r $(OBJS) -o $@
	
endif
