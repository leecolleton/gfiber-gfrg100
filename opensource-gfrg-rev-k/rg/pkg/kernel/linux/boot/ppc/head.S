
.text
	.globl	__start
__start:
	/* Check where we are vs. where we are linked at */
	bl	1f
1:
	mflr	r3
	lis	r4,1b@ha
	addi	r4,r4,1b@l
	mr	r5,r1
	b	do_boot
