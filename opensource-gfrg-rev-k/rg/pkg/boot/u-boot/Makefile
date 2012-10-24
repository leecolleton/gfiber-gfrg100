JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_INTERNAL_HEADERS+=image.h
MKIMAGE=mkimage

JMK_LOCAL_TARGET=$(MKIMAGE)
JMK_O_OBJS_$(JMK_LOCAL_TARGET)=mkimage.o crc32.o

JMK_CD_EXPORTED_FILES+=u-boot.README u-boot.patch

VMLINUX=$(JMKE_BUILDDIR)/os/linux/vmlinux
VMLINUX_BIN=vmlinux.bin
SYSTEM_MAP=$(JMKE_BUILDDIR)/os/linux/System.map
GET_SYMBOL=$(shell grep -w $1 $(SYSTEM_MAP) | awk '{ printf "0x%s", $$1 }')

ifdef CONFIG_DANUBE
  LZMA=$(JMK_ROOT)/vendor/infineon/danube/tools/lzma e -so
else
  LZMA=$(error LZMA is not defined for this arch)
endif
GZIP=gzip -f -9
BZIP2=bzip2

ifeq ($(ARCH),ppc)
  LOADADDR=0x00000000
  KERNEL_ENTRY=0x00000000
endif
ifeq ($(ARCH),mips)
ifdef CONFIG_LOADADDR_SYM_TEXT
  LOADADDR=$(call GET_SYMBOL, _text)
else
  LOADADDR=$(call GET_SYMBOL, _ftext)
endif
  KERNEL_ENTRY=$(call GET_SYMBOL, kernel_entry)
endif
ifeq ($(ARCH),arm)
  ifneq ($(CONFIG_FEROCEON),)
    include $(JMK_ROOT)/vendor/marvell/feroceon/linux-2.6/arch/arm/mach-feroceon-kw2/Makefile.boot
    LOADADDR=$(zreladdr-y)
    KERNEL_ENTRY=$(zreladdr-y)
  else
    LOADADDR=$(call GET_SYMBOL, _stext)
    KERNEL_ENTRY=$(call GET_SYMBOL, stext)
  endif
endif
ifdef CONFIG_ARCH_M83XXX
  ifndef CONFIG_RG_JPKG_SRC
    include $(JMKE_BUILDDIR)/os/linux-2.6/arch/arm/mach-comcerto/Makefile.boot
  endif
  MAX_KERNEL_SIZE:=0x01000000
  LOADADDR:=0x$(shell printf %x $$((${zreladdr-y}+${MAX_KERNEL_SIZE})))
  KERNEL_ENTRY:=${LOADADDR}
endif
ifndef CONFIG_BOOTLDR_UBOOT_COMP
  CONFIG_BOOTLDR_UBOOT_COMP=gzip
endif

ifeq ($(CONFIG_BOOTLDR_UBOOT_COMP),none)
  ifdef CONFIG_ARM
    ZIMAGE=$(JMKE_BUILDDIR)/os/linux/arch/arm/boot/zImage
  else
    ZIMAGE=$(VMLINUX_BIN)
  endif
endif
ifeq ($(CONFIG_BOOTLDR_UBOOT_COMP),gzip)
  ZIMAGE=$(VMLINUX_BIN).gz
endif
ifeq ($(CONFIG_BOOTLDR_UBOOT_COMP),bzip2)
  ZIMAGE=$(VMLINUX_BIN).bz2
endif
ifeq ($(CONFIG_BOOTLDR_UBOOT_COMP),lzma)
  ZIMAGE=$(VMLINUX_BIN).lzma
endif

JMK_LOCAL_TARGET=$(MKIMAGE)
JMK_O_OBJS_$(MKIMAGE)=mkimage.o crc32.o

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR
%.lzma: %
	$(LZMA) $^ > $@

%.gz: %
	$(GZIP) $^

%.bz2: %
	$(BZIP2) $^

$(VMLINUX_BIN): $(VMLINUX)
	$(OBJCOPY) -O binary $^ $@

$(OPENRG_IMG): $(MKIMAGE) $(ZIMAGE)
	$(MKIMAGE) -n 'OpenRG' -A $(ARCH) -O linux -T kernel -C \
	$(CONFIG_BOOTLDR_UBOOT_COMP) -a $(LOADADDR) -e $(KERNEL_ENTRY) \
	-d $(ZIMAGE) $@
endif
