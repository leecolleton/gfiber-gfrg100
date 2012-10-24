/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/mips/c-r4k.c
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

#include <linux/types.h>
#include <linux/kernel.h>

#include <asm/addrspace.h>
#include <asm/asm.h>
#include <asm/cacheops.h>
#include <asm/mipsregs.h>

#define INDEX_BASE CKSEG0

extern int puts(const char *s);
extern void puthex(unsigned long n);

#define cache_op(op, addr)			\
	__asm__ __volatile__(			\
	"	.set push		\n"	\
	"	.set noreorder		\n"	\
	"	.set mips3		\n"	\
	"	cache %1, 0(%0)		\n"	\
	"	.set pop		\n"	\
	:					\
	: "r" (addr), "i" (op))

#define cache_all_index_op(cache_size, linesz, op) do {			\
	unsigned long addr = INDEX_BASE;				\
	for (; addr < INDEX_BASE + (cache_size); addr += (linesz))	\
		cache_op(op, addr);					\
} while (0)

static void dcache_writeback(const unsigned long cache_size,
	const unsigned long linesz)
{
	puts("dcache writeback, cachesize ");
	puthex(cache_size);
	puts(" linesize ");
	puthex(linesz);
	puts("\n");

	cache_all_index_op(cache_size, linesz, Index_Writeback_Inv_D);
}

static void icache_invalidate(const unsigned long cache_size,
	const unsigned long linesz)
{
	puts("icache invalidate, cachesize ");
	puthex(cache_size);
	puts(" linesize ");
	puthex(linesz);
	puts("\n");

	cache_all_index_op(cache_size, linesz, Index_Invalidate_I);
}

void flush_cache_all(void)
{
	volatile unsigned long config1;
	unsigned long linesz;
	unsigned long ways;
	unsigned long sets;
	unsigned long cache_size;

	if (!(read_c0_config() & MIPS_CONF_M))
	{
	    puts("error flushing caches\n");
	    return;
	}
	config1 = read_c0_config1();

	if ((linesz = ((config1 >> 10) & 7)))
	    linesz = 2 << linesz;
	sets = 64 << ((config1 >> 13) & 7);
	ways = 1 + ((config1 >> 7) & 7);
	cache_size = sets * ways * linesz;
	dcache_writeback(cache_size, linesz);

	if ((linesz = ((config1 >> 19) & 7)))
	    linesz = 2 << linesz;
	sets = 64 << ((config1 >> 22) & 7);
	ways = 1 + ((config1 >> 16) & 7);
	cache_size = sets * ways * linesz;

	icache_invalidate(cache_size, linesz);
}
