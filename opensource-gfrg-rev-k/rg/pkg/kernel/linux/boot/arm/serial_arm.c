/* this file is stub. If vendor does not supply serial_putc,
 * create serial_<platform>.c with corresponding serial_init/serial_putc
 * and set SERIAL in envir.mk
 */

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
#error serial_putc is not implemented
}
