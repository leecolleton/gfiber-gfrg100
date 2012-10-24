/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/arm/serial_8250.c
 * 
 * Copyright (C) Jungo LTD 2004
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02111-1307, USA.
 *
 * Developed by Jungo LTD.
 * Residential Gateway Software Division
 * www.jungo.com
 * info@jungo.com
 */

void serial_init(void)
{
    /* Nothing to be done here :) */
}

void serial_putc(char c)
{
    volatile unsigned char *p = (volatile unsigned char *)0x08610F03;
#if 0
    /* WTF this does not work? For now the 'for' delay is working... */
    unsigned char status;
    do
    {
	status = *(unsigned char *)(p + 5);
    } while ((status & 0x60) != 0x60);
#endif
    volatile int i;
    for (i = 0; i < 999; i++)
	;

    *p = c;
}
