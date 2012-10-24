#define COMCERTO_UART0_DR	(*(volatile unsigned long *)0x10090000)	/* Tx/Rx Register */
#define M825XX1_UART0_SSR	(*(volatile unsigned long *)0x10090010)	/* Status register */
#define COMCERTO_UART0_LSR	(*(volatile unsigned long *)0x10090014)	/* Line Status Register (read only) */

#if defined(CONFIG_ARCH_M828XX) || defined(CONFIG_ARCH_M825XX2) || defined(CONFIG_ARCH_M829XX) || defined(CONFIG_ARCH_M821XX)
#define UART_DR		COMCERTO_UART0_DR
#define UART_LSR	COMCERTO_UART0_LSR
#else
#define UART_DR		COMCERTO_UART0_DR
#define UART_LSR	M825XX1_UART0_SSR
#endif

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    int i = 60000;
    while (!(UART_LSR & 0x20) && (i-- > 0))
#ifdef CONFIG_COMCERTO_BB_ROUTER
	barrier()
#endif
	;
    UART_DR = c;
}
