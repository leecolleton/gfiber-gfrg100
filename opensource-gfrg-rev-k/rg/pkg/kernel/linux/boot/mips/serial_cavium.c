#include <boot/rg_boot.h>

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    volatile long *p = (long*)0x8001180000000840ull;
    volatile int i;
    /* Just a small delay... */
    for (i = 0; i < 9999; i++);
    *p = c;
}
