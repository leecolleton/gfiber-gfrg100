KERNEL_START=$(call GET_SYMBOL, _stext)
KERNEL_ENTRY=$(call GET_SYMBOL, _stext)
KERNEL_END=$(call GET_SYMBOL, _end)

ifeq ($(CONFIG_CPU_XSCALE),y)
  LOADADDR=0x01000000
endif

# BB-ROUTER
ifeq ($(CONFIG_COMCERTO),y)
  SERIAL=serial_comcerto.o
  LOADADDR=0x82000000
endif

ifeq ($(CONFIG_IXP425),y)
  SERIAL=serial_ixp425.o
  HAVE_PUTS=y
endif

ifeq ($(CONFIG_MACH_PUMA5),y)
  SERIAL=serial_8250.o
  LOADADDR=0x81000000
endif

# Platform hasn't defined its SERIAL.
# We use stub and hope these functions are defined elsewhere
ifeq ($(SERIAL),)
   SERIAL=serial_arm.o
   HAVE_PUTS=y
endif

