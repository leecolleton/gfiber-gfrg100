/* File from: vendor/broadcom/bcm963xx/boot/romInit.S */
/*
<:copyright-broadcom 
 
 Copyright (c) 2002 Broadcom Corporation 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom Corporation 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom Corporation 
 company private, proprietary, and trade secret. 
:>
*/
/* romInit.s - template MIPS ROM initialization module */

/*
DESCRIPTION
This module contains the entry code for VxWorks images that start
running from ROM, such as 'bootrom' and 'vxWorks_rom'.
The entry point, romInit(), is the first code executed on power-up.
It performs the minimal setup needed to call
the generic C routine romStart() with parameter BOOT_COLD.

RomInit() typically masks interrupts in the processor, sets the initial
stack pointer (to STACK_ADRS which is defined in configAll.h), and
readies system memory by configuring the DRAM controller if necessary.
Other hardware and device initialization is performed later in the
BSP's sysHwInit() routine.

A second entry point in romInit.s is called romInitWarm(). It is called
by sysToMonitor() in sysLib.c to perform a warm boot.
The warm-start entry point must be written to allow a parameter on
the stack to be passed to romStart().

WARNING:
This code must be Position Independent Code (PIC).  This means that it
should not contain any absolute address references.  If an absolute address
must be used, it must be relocated by the macro ROM_ADRS(x).  This macro
will convert the absolute reference to the appropriate address within
ROM space no matter how the boot code was linked. (ROM_ADRS() can't be
used with the MIPS assembler at this time).

This code should not call out to subroutines declared in other modules,
specifically sysLib.o, and sysALib.o.  If an outside module is absolutely
necessary, it can be linked into the system by adding the module 
to the makefile variable BOOT_EXTRA.  If the same module is referenced by
other BSP code, then that module must be added to MACH_EXTRA as well.
Note that some C compilers can generate code with absolute addresses.
Such code should not be called from this module.  If absolute addresses
cannot be avoided, then only ROM resident code can be generated from this
module.  Compressed and uncompressed bootroms or VxWorks images will not
work if absolute addresses are not processed by the macro ROM_ADRS.

WARNING:
The most common mistake in BSP development is to attempt to do too much
in romInit.s.  This is not the main hardware initialization routine.
Only do enough device initialization to get memory functioning.  All other
device setup should be done in sysLib.c, as part of sysHwInit().

Unlike other RTOS systems, VxWorks does not use a single linear device
initialization phase.  It is common for inexperienced BSP writers to take
a BSP from another RTOS, extract the assembly language hardware setup
code and try to paste it into this file.  Because VxWorks provides 3
different memory configurations, compressed, uncompressed, and rom-resident,
this strategy will usually not work successfully.

WARNING:
The second most common mistake made by BSP writers is to assume that
hardware or CPU setup functions done by romInit.o do not need to be
repeated in sysALib.s or sysLib.c.  A vxWorks image needs only the following
from a boot program: The startType code, and the boot parameters string
in memory.  Each VxWorks image will completely reset the CPU and all
hardware upon startup.  The image should not rely on the boot program to
initialize any part of the system (it may assume that the memory controller
is initialized).

This means that all initialization done by romInit.s must be repeated in
either sysALib.s or sysLib.c.  The only exception here could be the
memory controller.  However, in most cases even that can be
reinitialized without harm.

Failure to follow this rule may require users to rebuild bootrom's for
minor changes in configuration.  It is WRS policy that bootroms and vxWorks
images should not be linked in this manner.
*/

#define _ASMLANGUAGE

        /* includes */

#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>

#include "6345_cpu.h"
#include "6345_common.h"
#include "board.h"
#include "compat.h"

        /* defines */

#define TIMER_MASK      2
#define TIMER_INTS      3
#define TIMER_CTL0      4
#define TIMER_CTL1      8
#define TIMER_CTL2      12

#if defined(BOOT_TARGET)
#define BOOT_START_TYPE  BOOT_CLEAR
#else
#define BOOT_START_TYPE  BOOT_NORMAL
#endif

        /* internals */

        .globl  romInit                /* branch to start of system code    */
        .globl  romReboot               /* direct entry to reboot            */

        /* externals */

        .globl  romStart                /* system initialization routine     */

        /* external defs */

        .extern end     4               /* end of program                    */
        .extern etext   4               /* end of text section               */
        .extern _fbss   4               /* start of bss section              */
        .extern _fdata  4               /* start of data section             */

#define RVECENT(f,n) \
        b f; \
	nop

#define XVECENT(f,bev) \
        b f; \
	li k0,bev

        .text
romInit:
_romInit:
        .set    noreorder
        RVECENT(__romInit,0)            /* PROM entry point */
        RVECENT(romReboot,1)            /* software reboot */
        RVECENT(romReserved,2)
        RVECENT(romReserved,3)
        RVECENT(romReserved,4)
        RVECENT(romReserved,5)
        RVECENT(romReserved,6)
        RVECENT(romReserved,7)
        RVECENT(romReserved,8)
        RVECENT(romReserved,9)
        RVECENT(romReserved,10)
        RVECENT(romReserved,11)
        RVECENT(romReserved,12)
        RVECENT(romReserved,13)
        RVECENT(romReserved,14)
        RVECENT(romReserved,15)
        RVECENT(romReserved,16)
        RVECENT(romReserved,17) 
        RVECENT(romReserved,18)
        RVECENT(romReserved,19)
        RVECENT(romReserved,20)
        RVECENT(romReserved,21)
        RVECENT(romReserved,22)
        RVECENT(romReserved,23)
        RVECENT(romReserved,24)
        RVECENT(romReserved,25)
        RVECENT(romReserved,26)
        RVECENT(romReserved,27)
        RVECENT(romReserved,28)
        RVECENT(romReserved,29)
        RVECENT(romReserved,30)
        RVECENT(romReserved,31)
#if     (CPU==R3000)
        XVECENT(romExcHandle,0x100)     /* bfc00100: R3000 utlbmiss vector */
#else
        RVECENT(romReserved,32)
#endif  /* CPU==R3000 */
        RVECENT(romReserved,33)
        RVECENT(romReserved,34)
        RVECENT(romReserved,35)
        RVECENT(romReserved,36)
        RVECENT(romReserved,37)
        RVECENT(romReserved,38)
        RVECENT(romReserved,39)
#if (CPU==R33000 || CPU==R33020)
        XVECENT(romExcHandle,0x140)     /* bfc00140: LR330x0 debug vector */
#else
        RVECENT(romReserved,40)
#endif  /* CPU==R33000 || CPU==R33020 */
        RVECENT(romReserved,41)
        RVECENT(romReserved,42)
        RVECENT(romReserved,43)
        RVECENT(romReserved,44)
        RVECENT(romReserved,45)
        RVECENT(romReserved,46)
        RVECENT(romReserved,47)
#if (CPU==R3000 || CPU==R33000 || CPU==R33020)
        XVECENT(romExcHandle,0x180)     /* bfc00180: R3000 general vector */
#elif ((CPU==RC32364) || (CPU==MIPS32))
        XVECENT(romExcHandle,0x380)
#else
        RVECENT(romReserved,48)
#endif  /* CPU==R3000 || CPU==R33000 || CPU==R33020 */
        RVECENT(romReserved,49)
        RVECENT(romReserved,50)
        RVECENT(romReserved,51)
        RVECENT(romReserved,52)
        RVECENT(romReserved,53)
        RVECENT(romReserved,54)
        RVECENT(romReserved,55)
        RVECENT(romReserved,56)
        RVECENT(romReserved,57)
        RVECENT(romReserved,58)
        RVECENT(romReserved,59)
        RVECENT(romReserved,60)
        RVECENT(romReserved,61)
        RVECENT(romReserved,62)
        RVECENT(romReserved,63) 
#if ((CPU==RC32364) || (CPU==MIPS32))
        XVECENT(romExcHandle,0x200)     /* bfc00200: R4000 tlbmiss vector */
#else
        RVECENT(romReserved,64)
#endif
        RVECENT(romReserved,65)
        RVECENT(romReserved,66)
        RVECENT(romReserved,67)
        RVECENT(romReserved,68)
        RVECENT(romReserved,69)
        RVECENT(romReserved,70)
        RVECENT(romReserved,71)
        RVECENT(romReserved,72)
        RVECENT(romReserved,73)
        RVECENT(romReserved,74)
        RVECENT(romReserved,75)
        RVECENT(romReserved,76)
        RVECENT(romReserved,77)
        RVECENT(romReserved,78)
        RVECENT(romReserved,79) 
#if ((CPU==RC32364) || (CPU==MIPS32)) 
        XVECENT(romExcHandle,0x280)     /* bfc00280: R4000 xtlbmiss vector */
#else
        RVECENT(romReserved,80)
#endif
        RVECENT(romReserved,81)
        RVECENT(romReserved,82)
        RVECENT(romReserved,83)
        RVECENT(romReserved,84)
        RVECENT(romReserved,85)
        RVECENT(romReserved,86)
        RVECENT(romReserved,87)
        RVECENT(romReserved,88)
        RVECENT(romReserved,89)
        RVECENT(romReserved,90)
        RVECENT(romReserved,91)
        RVECENT(romReserved,92)
        RVECENT(romReserved,93)
        RVECENT(romReserved,94)
        RVECENT(romReserved,95) 
#if ((CPU==RC32364) || (CPU==R4000) || (CPU==R4650) || (CPU==MIPS32)) 
        XVECENT(romExcHandle,0x300)     /* bfc00300: R4000 cache vector */
#else
        RVECENT(romReserved,96)
#endif
        RVECENT(romReserved,97)
        RVECENT(romReserved,98)
        RVECENT(romReserved,99)
        RVECENT(romReserved,100)
        RVECENT(romReserved,101)
        RVECENT(romReserved,102)
        RVECENT(romReserved,103)
        RVECENT(romReserved,104)
        RVECENT(romReserved,105)
        RVECENT(romReserved,106)
        RVECENT(romReserved,107)
        RVECENT(romReserved,108)
        RVECENT(romReserved,109)
        RVECENT(romReserved,110)
        RVECENT(romReserved,111)
#if ((CPU==RC32364) || (CPU==R4000) || (CPU==R4650) || (CPU==MIPS32)) 
        XVECENT(romExcHandle,0x380)     /* bfc00380: R4000 general vector */
#else
        RVECENT(romReserved,112)
#endif
        RVECENT(romReserved,113)
        RVECENT(romReserved,114)
        RVECENT(romReserved,115)
        RVECENT(romReserved,116)
        RVECENT(romReserved,116)
        RVECENT(romReserved,118)
        RVECENT(romReserved,119)
        RVECENT(romReserved,120)
        RVECENT(romReserved,121)
        RVECENT(romReserved,122)
        RVECENT(romReserved,123)
        RVECENT(romReserved,124)
        RVECENT(romReserved,125)
        RVECENT(romReserved,126)
        RVECENT(romReserved,127)

        /* We hope there are no more reserved vectors!
         * 128 * 8 == 1024 == 0x400
         * so this is address R_VEC+0x400 == 0xbfc00400
         */

        .set    reorder

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType  /@ only used by 2nd entry point @/
*     )

*/

__romInit:
        .set    noreorder
        mtc0    zero,C0_SR
        li      v0,SR_BEV
        mtc0    v0,C0_SR                # state unknown on reset
        mtc0    zero,C0_CAUSE           # clear software interrupts
        nop                             # paranoia
        .set    reorder

        /**--------------------------------------------------------------**/
        /** platform specific code                                       **/
        /**--------------------------------------------------------------**/

        .set    noreorder
        /**----- Disable Caches -----------------------------------------**/
        mfc0    t1, C0_BCM_CONFIG
        nop
        and     t1, t1, ~(CP0_CFG_ICSHEN|CP0_CFG_DCSHEN)
        mtc0    t1, C0_BCM_CONFIG      # disable I&D Caches
        .set    reorder

        /**----- Initialize EBI -----------------------------------------**/
        bal     InitEbi

        /**----- Initialize MIPS Clock ----------------------------------**/
		li              t0, PERF_BASE           # The address of the PLL register
        lw              t1, FMSEL(t0)
        and             t1, ~FMDIV_MASK
        sw              t1, FMSEL(t0)           # Set Mips Clock

        /**----- Init Serial port and send message-----------------------**/
        bal     InitSerial
	nop
        bal     BcmMsg

       /**----- Wait for a while before we can Init SDRAM---------------*/
        li      t5, 0x400
1:      
        addi    t5, -1
        bne     t5, zero, 1b
        nop

        /**----- Initialize SDRAM ---------------------------------------**/
        bal     InitSdram
        li      a0, 'A'
        bal     _writeasm
        nop

        /**--------------------------------------------------------------**/
        /** end of platform specific code                                **/
        /**--------------------------------------------------------------**/

        li      a0, 'F'
        bal     _writeasm
        nop

        /* set start type */
        li      a0, BOOT_START_TYPE

romReboot:
        /* second entry point -- sw reboot inherits a0 start type            */

        .set    noreorder
        li      t0, SR_BEV
        nop
        mtc0    t0, C0_SR               /* disable all interrupts fpa on,    */
                                        /* prom exception handlers           */
        mtc0    zero, C0_CAUSE          /* clear all interrupts              */
        nop
        .set    reorder

        move    s1, a0                  /* save start type                   */

        li      a0, 'K'
        bal     _writeasm
        nop

1:
        /**----- Initialize Timers ---------------------------------------**/
        bal     InitTimers
        li      a0, 'P'
        bal     _writeasm
	nop

        /* clear memory, if cold boot */
        andi    a0, s1, BOOT_CLEAR      /* check start type                  */
        beq     a0, zero, 1f
	nop
        bal     romClearMem
1:

        li      a0, 'T'
        bal     _writeasm
        nop
        li      a0, '\r'
        bal     _writeasm
        nop
        li      a0, '\n'
        bal     _writeasm
        nop

	b	head_start
	nop

/*
#************************************************************************
#*     InitEbi: Initializes the EBI for proper chip-select operation    *
#*                                                                      *
#*      SYNTAX: void  InitEbi(void);                                    *
#*     RETURNS:                                                         *
#*                                                                      *
#*     Configuration                                                    *
#*       CS[0] - Flash        @ 1fc0_0000 BASE_ADDR must already be set *
#*                                      because we have boot from flash *
#*                                                                      *
#************************************************************************
*/
        .ent InitEbi
InitEbi:
        .set    noreorder
        li      t1,EBIC_BASE            
        li      t2,PHYS_FLASH_BASE|EBI_SIZE_8M
        sw      t2,CS0BASE(t1)        # CS[0] Base
        li      t2,THREEWT|EBI_WORD_WIDE|EBI_ENABLE
        sw      t2,CS0CNTL(t1)        # CS[0] Control
        j       ra
        nop
        .set    reorder
        .end InitEbi
/*ENDFRAME  InitEbi */

/*******************************************************************************
*
* romClearMem - clear memory
*
* This routine clears the memory
* logic by doing word writes to each DRAM location.
*/

        .ent    romClearMem
        .set    noreorder
romClearMem:
        nop
        nop

        .set    reorder

        li      t0, 16 * 1024 * 1024   /* BCM96345 */

        subu    t0, USER_RESERVED_MEM
        li      t1, K1BASE      /* access dram uncached              */
        addu    t1, t0

clearLoop:
        sw      zero, -4(t1)
        sw      zero, -8(t1)
        sw      zero, -12(t1)
        sw      zero, -16(t1)
        sw      zero, -20(t1)
        sw      zero, -24(t1)
        sw      zero, -28(t1)
        sw      zero, -32(t1)
        sw      zero, -36(t1)
        sw      zero, -40(t1)
        sw      zero, -44(t1)
        sw      zero, -48(t1)
        sw      zero, -52(t1)
        sw      zero, -56(t1)
        sw      zero, -60(t1)
        sw      zero, -64(t1)
        subu    t1, 64
        bne     t1, K1BASE, clearLoop
        nop

        j       ra
        .end    romClearMem


/*******************************************************************************
*
* BcmMsg - Message that is displayed to serial terminal if _writeasm function
*          enabled.
*
*/
        .align  2
        .ent BcmMsg
BcmMsg:
        move    a1, ra

        li      a0, '\r'
        bal     _writeasm
        nop
        li      a0, '\n'
        bal     _writeasm
        nop
        li      a0, 'B'
        bal     _writeasm
        nop
        li      a0, 'C'
        bal     _writeasm
        nop
        li      a0, 'M'
        bal     _writeasm
        nop
        li      a0, '6'
        bal     _writeasm
        nop
        li      a0, '3'
        bal     _writeasm
        nop
        li      a0, '4'
        bal     _writeasm
        nop
#ifdef CONFIG_BCM96348
	li	a0, '8'
#else
        li      a0, '5'
#endif
        bal     _writeasm
        nop
        li      a0, '\r'
        bal     _writeasm
        nop
        li      a0, '\n'
        bal     _writeasm
        nop

        move    ra, a1
        jr      ra
        .end BcmMsg
/*
#************************************************************************
#* _writeasm: Write character to Serial Port                            *
#*                                                                      *
#*      Syntax: _writeasm (char)                                        *
#*        Note:                                                         *
#************************************************************************
*/
        .align  2
        .ent _writeasm
_writeasm:
#if defined(BOOT_TARGET) || defined(COPY_TO_FLASH)
        li      t0, UART_BASE
        li      t2, TXFIFOEMT
1:      lh      t1, UART0INTSTAT(t0)
        and     t1, t2
        bne     t1, t2, 1b
        nop
        sb      a0, UART0DATA(t0)
#endif
        j       ra
        nop
        .end _writeasm

/*******************************************************************************
*
* romReserved -  Handle a jump to an unknown vector
*

* romReserved ()

*
*/

        .ent    romReserved
romReserved:
1:
        b       romInit        /* just start over */
        .end    romReserved


/*      
#************************************************************************
#* RC32364
#*
#* InitTimers: Initialize timers to a disabled state                    *
#*                                                                      *
#*      Syntax: void TimerInit()                                        *
#*        Note:                                                         *       
#************************************************************************
*/
        .ent    InitTimers
InitTimers:
        .set    noreorder
        li      t0,TIMR_BASE
        li      v0,0x00
        sb      v0,TIMER_MASK(t0)
        li      v0,0x07
        sb      v0,TIMER_INTS(t0)

        sw      zero,TIMER_CTL0(t0)
        sw      zero,TIMER_CTL1(t0)
        sw      zero,TIMER_CTL2(t0)
        j       ra
        .set    reorder
        .end    InitTimers

/*
#************************************************************************
#* RC32364
#*     InitSdram: Initializes the Sdram for proper operation            *
#*                                                                      *
#*      SYNTAX: void  InitSdram(void);                                  *
#*     RETURNS:                                                         *
#*  Note:  We must be running UnCached before executing this routine    *
#*         for delay purposes                                           *
#************************************************************************
*/
        .align  2
        .ent InitSdram

InitSdram:
        .set    noreorder
        li      t1,SDRAM_BASE          

        /* Bypass SDRAM initialization if SDRAM is already enabled. 
	 *
	 * BCM9638 based boards have different SDRAM then the BCM96345 board.
	 * Therefor, the init procedure for the SDRAM is different.
	 * We do not have the original bootstrap code or the proper header
	 * files for BCM9638. So, the following code was extracted from the
	 * disassembly of an original CFE image downloaded from the boards
	 * flash.
	 * This code should be changed to using proper defines when we get
	 * the appropriate header files from Broadcom */
#ifdef CONFIG_BCM96348
        lw      t2,0(t1)
	lui	t4,0x8000
	or	t2,t2,t4
	sw	t2,0(t1)
#else
        lw      t2,SDR_REF_CTL(t1)
        li      t3,SDR_REF_EN|0x40
        beq     t2, t3, 2f
        nop

        /* SDRAM settings for RTA770W board (16MB) */
        li      t3,SDR_ENABLE|SDR_MRS_CMD|SDR_128MEG|SDR_9BIT_COL
#endif
#ifdef CONFIG_BCM96348
        li      t3,0x2c1c
        li      t2,SDR_FULL_PG
#else
        li      t2,PHYS_DRAM_BASE|DRAM16MBSPC

1:
        sw      t2,SDR_MEM_BASE(t1)   # Set Memory Base address and size
        li      t2,SDR_FULL_PG|SDR_FAST_MEM
#endif
        sw      t2,SDR_CFG_REG(t1)    # Full page burst, slow memory
        li      t2,SDR_ENABLE|SDR_PRE_CMD
        sw      t2,SDR_INIT_CTL(t1)   # Pre-charge powerup sequence
        li      t2,SDR_ENABLE|SDR_CBR_CMD
        sw      t2,SDR_INIT_CTL(t1)   # 8 or more auto-refresh cmds
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t3,SDR_INIT_CTL(t1)   # Mode Register set, 
        li      t2,SDR_REF_EN|0x40     
#ifdef CONFIG_BCM96348
	sw      t2,0xc(t1)
#else
        sw      t2,SDR_REF_CTL(t1)    # Refresh period 0x40, Enable
#endif

2:
        j       ra
        nop
        .set    reorder
        .end InitSdram

/*
#************************************************************************
#* RC32364
#* InitSerial:  Initialize Serial Port to 115200,N,8,1                  *
#*                                                                      *
#*      Syntax: InitSerial(void)                                        *
#*        Note:                                                         *
#************************************************************************
*/
        .align  2
        .ent InitSerial


InitSerial:
        .set    noreorder

        li              t3, ((FPERIPH / 115200) / 16)
        /*
        #  Baudword = (FPeriph)/Baud/32-1.  We have to perform rounding
        #  and subtraction.  Above we divided by 16 (instead of 32).  If
        #  bit0 is set, we round up.  However, we then subtract 1, so final
        #  result should be t3/2.  If bit0 is 0, then we truncate and subtract
        #  1, t3=t3/2-1.
        */
        andi            t0, t3, 0x1
        bne             t0,zero,1f         		# do shift only (in delay slot)
                                                # and jump to apply
        srl             t3,1                    # do divide by 2
        addiu           t3, -1                  # subtract 1
1:		
		li              t0, PERF_BASE           # Clock Control Register
        lhu             t1, BLKEN(t0)
        or              t1, UART_CLK_EN
        sh              t1, BLKEN(t0)           # Turn-on UART clock

        # t3 contains the UART BAUDWORD
        li              t0, UART_BASE
        sw              t3, UART0BAUD(t0)       # Store BaudRate
        li              t1, BITS8SYM|ONESTOP    
        sb              t1, UART0CONFIG(t0)     # 8 Bits/1 Stop
        li              t1, TX4|RX4             
        sb              t1, UART0FIFOCFG(t0)    # FifoThreshold
        li              t1, 0x5
        sb              t1, UART0RXTIMEOUT(t0)  # RxTimeout
        li              t1, TXEN|RXEN|BRGEN|TXPARITYEVEN|RXPARITYEVEN
        sb              t1, UART0CONTROL(t0)    # Enable, No Parity
        move            t1, zero
        sh              t1, UART0INTMASK(t0)

        j       ra
        nop
        .set    reorder
        .end InitSerial


/*******************************************************************************
* RC32364
*
* romExcHandle - rom based exception/interrupt handler
*
* This routine is invoked on an exception or interrupt while
* the status register is using the bootstrap exception vectors.
* It saves a state frame to a known uncached location so someone
* can examine the data over the VME.  It also displays a summary of the
* error on the boards alphanumeric display.
*
* THIS ROUTIINE IS NOT CALLABLE FROM "C"
*
*/

#define ROM_ISP_BASE    0xa0100000

        .ent    romExcHandle
romExcHandle:
	b 	romExcHandle
	nop
        .end    romExcHandle            /* that's all folks */

