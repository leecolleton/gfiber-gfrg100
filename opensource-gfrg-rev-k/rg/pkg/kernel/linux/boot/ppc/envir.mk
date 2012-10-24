LOADADDR=0xa00000
KERNEL_START=$(call GET_SYMBOL, _start)
KERNEL_ENTRY=$(call GET_SYMBOL, __start)
KERNEL_END=$(call GET_SYMBOL, _end)

# XXX need to fix...
BOOT_ARCH_CFLAGS=-I$(JMK_ROOT)/..//os/linux-2.6/include -I$(JMK_ROOT)/../os/linux-2.6/arch/ppc/ 
