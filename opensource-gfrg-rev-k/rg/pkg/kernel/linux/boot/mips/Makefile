JMK_ROOT=../../../../../
include $(JMK_ROOT)/envir.mak

include envir.mk

JMK_CFLAGS=$(JMK_RG_LINUX_CFLAGS) -DHEAD_ADDR=$(LOADADDR) -DKERNEL_END=$(KERNEL_END)

OBJS_FOR_JPKG=$(if $(CONFIG_BCM963XX_BOOTSTRAP),$(BOOTSTRAP)) head.o $(SERIAL)

ifndef CONFIG_RG_OS_LINUX_24
ifneq ($(CONFIG_CPU_MIPS32)$(CONFIG_CPU_MIPS64),)
  OBJS_FOR_JPKG+=c-r4k.o
endif
endif

OBJS+=$(OBJS_FOR_JPKG)

JMK_CD_EXPORTED_FILES+=$(SERIAL:%.o=%.c) ld.script

JMK_CLEAN+=$(OBJS) boot.o
JMK_CLEAN+=dummy jmp_boot.o jmp_boot.img jmp_boot.elf jmp_boot.bin jmp_boot.lz

include $(JMKE_MK)

ifdef JMKE_IS_BUILDDIR
boot.o: $(OBJS)
	$(LD) -r $(OBJS) -o $@

# TODO CONFIG_JMPBOOT or something
ifdef CONFIG_BCM963XX
ifdef CONFIG_BOOTLDR_CFE
JMP_BOOT_ADDR=0x80010000
OPENRG_SECTION_OFFSET=$(shell $(JMKE_BUILDDIR)/pkg/perm_storage/image_flash_offset)
# The address of the kernel.o on the flash
KERNEL_ADDR=$$(( $(CONFIG_MTD_PHYSMAP_START)|0xb0000000 + $(OPENRG_SECTION_OFFSET) + $(KERNEL_LDR_OFFSET)))

BCM_DIR=$(JMK_ROOT)/vendor/broadcom/bcm963xx/linux-2.6/
BCM_CFLAGS=-I$(BCM_DIR)/shared/opensource/include/bcm963xx \
           -I$(BCM_DIR)/bcmdrivers/opensource/include/bcm963xx

JMK_CFLAGS_$(SERIAL:%.c=%.o)+=$(BCM_CFLAGS)

jmp_boot.img: jmp_boot.elf
	$(OBJCOPY) -O binary -R .note -R .comment -R __ex_table -S $< \
		jmp_boot.bin
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/lib:/usr/lib:/lib/i386-linux-gnu:$(I386_TOOLS_PREFIX)/$(I386_TARGET_MACHINE)/lib \
		$(BCM_DIR)/hostTools/cmplzma -k -2 \
		jmp_boot.elf jmp_boot.bin jmp_boot.lz
	touch dummy
	$(BCM_DIR)/hostTools/bcmImageBuilder \
		--output $@ \
		--chip $(CONFIG_BCM963XX_CHIP) \
		--board $(CONFIG_BCM963XX_BOARD_ID) \
		--blocksize $(if $(filter DWV_BCM96358 CT6382D,$(CONFIG_RG_HW)),64,128) \
		--cfefile $(JMK_ROOT)/vendor/broadcom/bcm963xx/linux-2.6/cfe/cfe$(CONFIG_BCM963XX_CHIP).bin \
		--rootfsfile dummy --kernelfile jmp_boot.lz

jmp_boot.elf: ld_jmp_boot.script jmp_boot.o 
	$(LD) jmp_boot.o -T $< -Ttext $(JMP_BOOT_ADDR) -o $@ --defsym \
		openrg_start=$(KERNEL_ADDR)
endif
endif

endif
