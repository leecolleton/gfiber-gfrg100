JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

JMK_TARGET=spi eroute spigrp tncfg klipsdebug
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

PLUTODIR=$(JMK_ROOT)/pkg/freeswan/pluto

JMK_O_OBJS_spi=spi.o
ifeq ($(CONFIG_RG_IPSEC_ESP_ALG),y)
  JMK_O_OBJS_spi+=constants.o alg_info.o kernel_alg.o
  JMK_CFLAGS_spi+=-I$(PLUTODIR)
  JMK_CFLAGS_constants.o+=-UDEBUG
  JMK_CFLAGS_alg_info.o+=-DNO_PLUTO
  JMK_CFLAGS_kernel_alg.o+=-DNO_PLUTO
  JMK_LINKS=constants.c alg_info.c kernel_alg.c
$(JMK_LINKS):
	$(JMKE_LN) $(JMK_ROOT)/pkg/freeswan/pluto/$@
endif
JMK_O_OBJS_eroute=eroute.o
JMK_O_OBJS_spigrp=spigrp.o
JMK_O_OBJS_tncfg=tncfg.o
JMK_O_OBJS_pf_key=pf_key.o
JMK_O_OBJS_klipsdebug=klipsdebug.o

JMK_CFLAGS+=-I$(JMK_ROOT)/pkg/freeswan/klips/net/ipsec -I$(JMK_ROOT)/pkg/freeswan/lib -I$(JMK_ROOT)/pkg/gmp
JMK_LDFLAGS+=-L$(JMK_ROOT)/pkg/freeswan/lib/
JMK_LDLIBS:=-lfreeswan $(JMK_LDLIBS)

include $(JMKE_MK)

