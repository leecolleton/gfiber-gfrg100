#include <asm/asm.h>
#include <asm/regdef.h>
	
	.text
	.set	noreorder
	.globl 	jmp_head_start

jmp_head_start:
	la	a0, openrg_start
	j 	a0
	nop
	.set	reorder
