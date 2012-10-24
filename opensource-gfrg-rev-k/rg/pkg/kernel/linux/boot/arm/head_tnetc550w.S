/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/arm/head_tnetc550w.S
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

#include <linux/linkage.h>

#define DEBUG 1
#ifdef DEBUG
  #define MARK(n) mov r0, n; bl __aputc;
#else
  #define MARK(n)
#endif

.section ".start", #alloc, #execinstr

start:
	MARK(#'0')
	bl _start;
dummy_lable:
	MARK(#'X')
	
.globl __relocate_data
__relocate_data:
	.word dummy_lable
	.word _edata @ end of copy

.globl __clr_bss_data
__clr_bss_data:
	.word clear_bss
	.word _bss_start
	.word _bss_end

_start:
	mov	r4, lr			@ r9 runtime address

@	r4 runtime address of dummy_lable
@	r5 linked time address of dummy_lable
@	r6 end of copy

	MARK(#'1')

	adr	r0, __relocate_data
	ldmia	r0!, {r5-r6}
	MARK(#'2')

	/* If runtime address of dummy_jump is not equal to its compile
	 * address, relocate ourself
	 */
	cmp	r4, r5
	beq	pre_clear_bss
	MARK(#'3')

	/* We need relocate ourself to our liked address.
	 * Note: Assuming addresses are not overlap.
	 */
	mov	r0, r4		@ src
	mov	r1, r5		@ dst

relocate_loop:
	ldmia	r0!, {r3,r7-r9}
	stmia	r1!, {r3,r7-r9}

	@ We copy 4*4 bytes at a time. E.g.: We might copy a few extra bytes
	cmp	r1,r6
	blo	relocate_loop
	MARK(#'4')

pre_clear_bss:
	MARK(#'5')
	adr	r2, __clr_bss_data
	ldmia	r2!, {r7, r8, r9}
	/* Jump according to link time address. Works for both relocate and
	 * normal cases
	 */
	mov	pc, r7

clear_bss:
	@ R0 - start of BSS
	@ R1 - End of BSS
	MARK(#'6')
	mov	r0, r8
	mov	r1, r9
	mov	r2, #0x00000000		/* clear			    */

clbss_l:str	r2, [r0]		/* clear loop...		    */
	add	r0, r0, #4
	cmp	r0, r1
	bne	clbss_l
	MARK(#'7')
	.section .text
	mov	r0, r4
	mov	r1, r5
	b	do_boot

#ifdef DEBUG
@ Print single char to serial
@ Input: r0 - char
@ Note: R1 is modified

.type	__aputc, %function
__aputc:
	ldr	r1, [pc, #4]
	strb	r0, [r1, #3]
	mov pc, lr
.word	0x08610F00
#endif
