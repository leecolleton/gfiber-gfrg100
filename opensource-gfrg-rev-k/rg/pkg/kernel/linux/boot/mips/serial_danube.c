void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    unsigned int status;
    volatile unsigned long *p = (volatile unsigned long *)0xbe100c00;
    
    if (c == '\n')
	serial_putc ('\r');

    do
    {
	status = (p[0x48/4] & 0x3F00) >> 8;
    }
    while ( status == 16 );

    p[0x20/4] = c;

    /* check for errors */
    if (status & 0x00100000)
	p[0x14/4] |= 0x00000040;
}
