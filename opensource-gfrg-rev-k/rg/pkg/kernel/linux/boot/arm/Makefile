JMK_ROOT=../../../../../
include $(JMK_ROOT)/envir.mak

include envir.mk

JMK_CFLAGS+=$(JMK_RG_LINUX_CFLAGS) -DHEAD_ADDR=$(LOADADDR) -DKERNEL_END=$(KERNEL_END)

ifeq ($(CONFIG_MACH_PUMA5),y)
  OBJS = head_tnetc550w.o
else
  OBJS = head.o
endif
OBJS += $(SERIAL)

ifeq ($(CONFIG_CPU_XSCALE),y)
  OBJS += head-xscale.o
endif

# Yet not supported
#CONFIG_ARCH_SL2312, (CONFIG_ARCH_CX8620X)

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
  OBJS += big-endian.o
endif

OBJS_FOR_JPKG=$(OBJS)
JMK_CD_EXPORTED_FILES+=$(SERIAL:%.o=%.c) ld.script

JMK_CLEAN+=boot.o $(OBJS)

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR
    boot.o: $(OBJS)
	$(LD) -r $(OBJS) -o $@ --defsym kernel_start=$(KERNEL_START) 
endif
