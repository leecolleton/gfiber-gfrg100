/* File is originaly from vendor/broadcom/bcm963xx/linux-2.6/boot/serial.c */
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
/*
 *  Broadcom bcm6345 serial port initialization.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/console.h>
#include <linux/sched.h>

#include <asm/addrspace.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/gdb-stub.h>
#include <asm/mc146818rtc.h>
#include <bcm_map_part.h>
#include <board.h>
#include <bcmtypes.h>

#define  SER63XX_DEFAULT_BAUD      115200
#define BD_BCM63XX_TIMER_CLOCK_INPUT    (FPERIPH)
#define stUart ((volatile Uart * const) UART_BASE)

// Transmit interrupts
#define TXINT       (TXFIFOEMT | TXUNDERR | TXOVFERR)
// Receive interrupts
#define RXINT       (RXFIFONE | RXOVFERR)

/* --------------------------------------------------------------------------
    Name: serial_init
 Purpose: Initalize the UART
-------------------------------------------------------------------------- */
void serial_init(void)
{
    UINT32 tmpVal = SER63XX_DEFAULT_BAUD;
    ULONG clockFreqHz;    

#if defined(CONFIG_BCM96345)
    // Make sure clock is ticking
    PERF->blkEnables |= UART_CLK_EN;
#endif
		
    /* Dissable channel's receiver and transmitter.                */
    stUart->control &= ~(BRGEN|TXEN|RXEN);
		
    /*--------------------------------------------------------------------*/
    /* Write the table value to the clock select register.                */
    /* DPullen - this is the equation to use:                             */
    /*       value = clockFreqHz / baud / 32-1;                           */
    /*   (snmod) Actually you should also take into account any necessary */
    /*           rounding.  Divide by 16, look at lsb, if 0, divide by 2  */
    /*           and subtract 1.  If 1, just divide by 2                  */
    /*--------------------------------------------------------------------*/
    clockFreqHz = BD_BCM63XX_TIMER_CLOCK_INPUT;
    tmpVal = (clockFreqHz / tmpVal) / 16;
    if( tmpVal & 0x01 )
        tmpVal /= 2;  //Rounding up, so sub is already accounted for
    else
        tmpVal = (tmpVal / 2) - 1; // Rounding down so we must sub 1
    stUart->baudword = tmpVal;
        
    /* Finally, re-enable the transmitter and receiver.            */
    stUart->control |= (BRGEN|TXEN|RXEN);

    stUart->config   = (BITS8SYM | ONESTOP);
    // Set the FIFO interrupt depth ... stUart->fifocfg  = 0xAA;
    stUart->fifoctl  =  RSTTXFIFOS | RSTRXFIFOS;
    stUart->intMask  = 0;       
}

/* prom_putc()
 * Output a character to the UART
 */
void serial_putc(char c)
{
	/* Wait for Tx uffer to empty */
	while (! (READ16(stUart->intStatus) & TXFIFOEMT));
	/* Send character */
	stUart->Data = c;
}
