#define MEMBASE 0xb9020000
#define UART_TX 0
#define UART_LSR 5
#define EMPTY 0x60

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    unsigned int status;
    while(1)
    {
	status = *(unsigned char *)(MEMBASE + UART_LSR*4);
	if ((status & EMPTY) == EMPTY)
	    break;
	__asm__ __volatile__("": : :"memory");
    }
    *(unsigned char *)(MEMBASE + UART_TX) = c;
}
