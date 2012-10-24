#include <linux/types.h>
#include <asm/serial.h>
#include <asm/hardware.h>
#include <asm-arm/arch/ixp425.h>
#include <linux/serial_reg.h>

#if CONFIG_IXDP425_KGDB_UART == 0
#define UART_BASE ((volatile u32*)IXP425_UART1_BASE_VIRT)
#else
#if CONFIG_IXDP425_KGDB_UART == 1
#define UART_BASE ((volatile u32*)IXP425_UART2_BASE_VIRT)
#else
#error INVALID CONFIG_IXDP425_KGDB_UART
#endif
#endif

#define TX_DONE	(UART_LSR_TEMT|UART_LSR_THRE)

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    /* Check THRE and  TEMT bits before we transmit the character */
    while ((UART_BASE[UART_LSR] & TX_DONE) != TX_DONE);

    *UART_BASE = c;
}

