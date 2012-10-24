JMK_ROOT=../../../../
include $(JMK_ROOT)/envir.mak

ARCH_ENVIR=$(wildcard $(JMK_ROOT)/pkg/kernel/linux/boot/$(ARCH)/envir.mk)
ifneq ($(ARCH_ENVIR),)
  include $(JMK_ROOT)/pkg/kernel/linux/boot/$(ARCH)/envir.mk
endif

JMK_EXPORT_HEADERS_DIR=boot
JMK_EXPORT_HEADERS=rg_boot.h rg_cramfs_location.h rg_boot_user.h

ifndef CONFIG_RG_UML
  JMK_SUBDIRS = $(ARCH)
  JMK_LINK_DIRS = $(ARCH)
  
  JMK_MOD_TARGET = rg_boot.o
  JMK_O_OBJS_rg_boot.o = rg_prom.o \
    $(if $(CONFIG_RG_MAINFS_IN_UBI),cramfs_in_ubi.o)
  JMK_MOD_2_STAT = $(JMK_MOD_TARGET)
endif

LZMA=$(JMKE_BUILDDIR)/pkg/lzma/SRC/lzma e -so
GZIP=gzip -f -9
BZIP2=bzip2

HAVES=MEMCPY PUTS

JMK_LOCAL_TARGET += calc_crc
JMK_O_OBJS_calc_crc += calc_crc.o

JMK_MOD_CFLAGS+=$(JMK_RG_LINUX_CFLAGS) $(BOOT_ARCH_CFLAGS) 
JMK_CFLAGS+=$(JMK_MOD_CFLAGS)
JMK_CFLAGS_misc.o=-DKERNEL_START=$(KERNEL_START) -DKERNEL_ENTRY=$(KERNEL_ENTRY) \
  -DHEAD_ADDR=$(LOADADDR) -DKERNEL_END=$(KERNEL_END) \
  -DCRAMFS_CRC=0x$(shell cat cramfs_crc) \
  $(if $(MALLOC_START),-DMALLOC_START=$(MALLOC_START)) \
  $(foreach h,$(HAVES),$(if $(HAVE_$h),-DHAVE_$h))

JMK_CFLAGS_kernel.o=-DKERNEL_IMG=\"$(ZIMAGE)\"
JMK_CFLAGS_rofs.o=-DMAINFS_IMG=\"$(COMPRESSED_DISK)\"

JMK_CLEAN=$(wildcard *.o vmlinux.*) image.elf image.bin cramfs_crc

include $(JMKE_MK)

export VMLINUX=$(JMKE_BUILDDIR)/os/linux/vmlinux
export VMLINUX_BIN:=$(JMKE_PWD_BUILD)/vmlinux.bin
export BOOTIMG:=$(JMKE_PWD_BUILD)/bootimg

ifeq ($(CONFIG_RG_KERNEL_COMP_METHOD),none)
  ZIMAGE=$(VMLINUX_BIN)
endif
ifeq ($(CONFIG_RG_KERNEL_COMP_METHOD),gzip)
  ZIMAGE=$(VMLINUX_BIN).gz
endif
ifeq ($(CONFIG_RG_KERNEL_COMP_METHOD),bzip2)
  ZIMAGE=$(VMLINUX_BIN).bz2
endif
ifeq ($(CONFIG_RG_KERNEL_COMP_METHOD),lzma)
  ZIMAGE=$(VMLINUX_BIN).lzma
endif
export ZIMAGE
ifndef LDSCRIPT
  LDSCRIPT=ld.script
endif

ifdef JMKE_IS_BUILDDIR
cramfs_crc: $(JMKE_BUILDDIR)/pkg/build/disk_image/cramfs.img
	./calc_crc $^ > cramfs_crc

misc.o: cramfs_crc

VMLINUX=$(JMKE_BUILDDIR)/os/linux/vmlinux

%.lzma: %
	$(LZMA) $^ > $@

%.gz: %
	$(GZIP) $^

%.bz2: %
	$(BZIP2) $^

$(VMLINUX_BIN): $(VMLINUX)
	$(OBJCOPY) -O binary $^ $@

$(ZIMAGE): $(VMLINUX_BIN)

rofs.o: $(COMPRESSED_DISK)

kernel.o: $(ZIMAGE)

image.bin: image.elf
	$(OBJCOPY) -O binary -R .note -R .comment -R __ex_table -S $< $@

OBJS_FOR_JPKG=misc.o kernel.o rofs.o
OBJS=$(ARCH)/boot.o $(OBJS_FOR_JPKG)

$(OBJS): $(SYSTEM_MAP)

$(ARCH)/boot.o: dummy
	$(MAKE) -C $(ARCH) boot.o

ifndef CONFIG_64BIT
  O_FORMAT = --oformat $(shell $(OBJDUMP) -i | head -2 | grep elf32)
endif

image.elf: $(ARCH)/$(LDSCRIPT) $(OBJS) dummy
	$(LD) $(OBJS) -T $< -Ttext $(LOADADDR) -Map image.map $(O_FORMAT) -o $@

ifdef CONFIG_RG_IMAGE_ELF
  ifdef CONFIG_CAVIUM_OCTEON
$(OPENRG_IMG): $(VMLINUX)
  else
$(OPENRG_IMG): image.elf
  endif
	cp $^ $@
	$(STRIP) $@
else
ifdef CONFIG_BOOTLDR_CFE
  ifdef CONFIG_BCM963XX
    CONFIG_RG_JMPBOOT=y
  endif
endif
$(OPENRG_IMG): image.bin
# TODO CONFIG_RG_JMPBOOT or something NO IFDEF ON PLATFORM!
ifdef CONFIG_RG_JMPBOOT
	$(MAKE) -C $(ARCH) jmp_boot.img
	dd if=$(ARCH)/jmp_boot.img of=$@
	dd if=$< of=$@ obs=$(KERNEL_LDR_OFFSET) seek=1
else
	cp $< $@
endif
endif
endif
