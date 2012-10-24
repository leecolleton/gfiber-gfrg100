/* File is originaly from vendor/broadcom/bcm9636x/bcmdrivers/opensource/char/\
 *                                                  serial/impl1/bcm63xx_cons.c
 */
/*
<:copyright-gpl 
 Copyright 2002 Broadcom Corp. All Rights Reserved. 
 
 This program is free software; you can distribute it and/or modify it 
 under the terms of the GNU General Public License (Version 2) as 
 published by the Free Software Foundation. 
 
 This program is distributed in the hope it will be useful, but WITHOUT 
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 for more details. 
 
 You should have received a copy of the GNU General Public License along 
 with this program; if not, write to the Free Software Foundation, Inc., 
 59 Temple Place - Suite 330, Boston MA 02111-1307, USA. 
:>
*/

#include <bcm_map.h>
#include <bcm_hwdefs.h>

void serial_init(void)
{
    unsigned long baudwd = (FPERIPH / 115200) >> 4;

    UART->baudword = (baudwd >> 1) - (1 - (baudwd & 1));

    UART->control = BRGEN|TXEN|RXEN;
    UART->config = BITS8SYM|ONESTOP;
    UART->fifoctl = RSTTXFIFOS|RSTRXFIFOS;
    UART->intMask  = 0;
}

void serial_putc(char c)
{
    while (!(UART->intStatus & TXFIFOEMT)) {}

    UART->Data = c;

    if (UART->intStatus & (TXOVFERR|TXUNDERR))
        UART->fifoctl |= RSTTXFIFOS;
}
