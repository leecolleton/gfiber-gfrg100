JMK_ROOT=../../../../../..
include $(JMK_ROOT)/envir.mak

JMK_MOD_TARGET=lzma_decode.o
JMK_MOD_2_STAT=lzma_decode.o
JMK_O_OBJS=LzmaDecode.o decode.o

LINK_DIR=$(JMKE_BUILDDIR)/os/linux/lib/lzma_decode

include $(JMKE_MK)

ifdef CONFIG_RG_LZMA_COMPRESSED_KERNEL
$(LINK_DIR):
	mkdir -p $(JMKE_BUILDDIR)/os/linux/lib
	$(JMKE_LN) $(JMKE_PWD_SRC) $@

archconfig::$(LINK_DIR)
endif
