/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/misc.c
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

#include <linux/vmalloc.h>
#include <asm/io.h>

#include <boot/rg_boot.h>

/* TODO. Change to common ARM config. */
#ifdef CONFIG_ARCH_IXP425
#include <asm/arch/uncompress.h>
#endif

#include <boot/rg_boot_user.h>

#ifdef DEBUG
  #define PS(s, n) { puts(s); puts(": "); puth(n); puts("\n"); }
  #define PV(n) PS(#n, n)
  #define DUMP(msg, addr) \
   {puts(msg); \
    puts("\n+0: "); puth(*(long *)addr); \
    puts("\n+1: "); puth(*(((long *)addr)+1)); \
    puts("\n+2: "); puth(*(((long *)addr)+2)); \
    puts("\n");}
#else
  #define PS(s, n)
  #define PV(n)
  #define DUMP(msg, addr)
#endif

#define puth(x) puthex((unsigned long)(x))

#ifdef CONFIG_ARCH_IXP425
#define serial_putc putc
#else
extern void serial_putc(char c);
#endif

extern void serial_init(void);

unsigned long rg_param0, rg_param1;

#ifndef HAVE_PUTS
int puts(const char *s)
{
	while (*s) {
		if (*s == 10)
			serial_putc(13);
		serial_putc(*s++);
	}
	return 0;
}
#endif

#ifndef HAVE_MEMCPY
void *memcpy(void *dest, const void *src, size_t n)
{
    void *d = dest;
    /* Trivial implementation of memcpy */
    for (; n >0 ; n--)
	*(unsigned char *)dest++ = *(unsigned char *)src++;

    return d;
}
#endif

char *shex(char *buf, unsigned long n)
{
    int i;
    for (i = sizeof(addr_t) * 2 - 1; i >= 0; i--)
    {
	unsigned char d = (n >> (i*4)& 0x0f);
	*buf = d < 0xa ? d+0x30 : d + 0x37; /* 0-9 VS A-F */
	buf++;
    }
    return buf;
}

void puthex(unsigned long n)
{
    /* 32 is long enough for 64 bit numbers */
    char buf[32], *p;

    p = shex(buf, n);
    *p = 0;
    puts(buf);
}

/* TODO: only mips is using it. It should be in the LD script... */
#define STACK_SIZE (4096)
long user_stack[STACK_SIZE];
long *stack_start = &user_stack[STACK_SIZE];

/************************************************/
/*
 * gzip declarations
 */

#define OF(args)  args
#define STATIC static

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;

#ifdef CONFIG_RG_GZIP_COMPRESSED_KERNEL
#define WSIZE 0x8000		/* Window size must be at least 32k, */
			        /* and a power of two */

static uch *inbuf;		/* input buffer */
static uch window[WSIZE];	/* Sliding window buffer */

static unsigned insize = 0;	/* valid bytes in inbuf */
static unsigned inptr = 0;	/* index of next byte to be processed in inbuf */
static unsigned outcnt = 0;	/* bytes in output buffer */

static int fill_inbuf(void);
static void flush_window(void);
static void error(char *m);
static void gzip_mark(void **);
static void gzip_release(void **);

/* gzip flag byte */
#define ASCII_FLAG   0x01	/* bit 0 set: file probably ASCII text */
#define CONTINUATION 0x02	/* bit 1 set: continuation of multi-part gzip file */
#define EXTRA_FIELD  0x04	/* bit 2 set: extra field present */
#define ORIG_NAME    0x08	/* bit 3 set: original file name present */
#define COMMENT      0x10	/* bit 4 set: file comment present */
#define ENCRYPTED    0x20	/* bit 5 set: file is encrypted */
#define RESERVED     0xC0	/* bit 6,7:   reserved */

int checksum;
int byte_count;
static unsigned char *input_data;
static int input_len;

int get_byte(void)
{
	unsigned char c = (inptr < insize ? inbuf[inptr++] : fill_inbuf());
	byte_count++;
	checksum += c;

	return c;
}

/* Diagnostic functions */
#ifdef DEBUG
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif
#endif

extern uch malloc_start;

static uch *output_data;
static size_t output_len = 0;

static void *malloc(int size) __attribute__((__unused__));
static void free(void *where) __attribute__((__unused__));
static void error(char *m);

static addr_t free_mem_ptr;
static addr_t free_mem_end_ptr;

#ifndef CONFIG_RG_GZIP_COMPRESSED_KERNEL
static void error(char *x)
{
	puts(x);
	puts("\n\n -- System halted");
	while (1);
}

#endif

#ifdef CONFIG_RG_BZIP2_COMPRESSED_KERNEL
static void bz_internal_error(int errcode)
{
    error("error during bzip");
}

#include "../../../../../os/linux/lib/bzlib.c"
#elif defined(CONFIG_RG_GZIP_COMPRESSED_KERNEL)
static long bytes_out = 0;
static void gzip_mark(void **);
static void gzip_release(void **);

#include "../../os/linux/lib/inflate.c"
#elif defined(CONFIG_RG_LZMA_COMPRESSED_KERNEL)
#define __LZMA_UNCOMPRESS_KERNEL__
#include "../../os/linux/lib/lzma_decode/LzmaDecode.c"
#include "../../os/linux/lib/lzma_decode/decode.c"
#endif

static void *malloc(int size)
{
	void *p;

	if (size < 0)
		error("Malloc error\n");

	puts("malloc: ");
	puth(size);
	
	/* Align */	
	free_mem_ptr = (addr_t)((((unsigned long)free_mem_ptr) + 3) & ~3);

	p = (void *) free_mem_ptr;
	free_mem_ptr += size;

	if (free_mem_ptr >= free_mem_end_ptr)
		error("\nOut of memory\n");
	puts("\nreturned: ");
	puth(p);
	puts("\n");

	return p;
}

static void free(void *where)
{				/* Don't care */
}

#ifdef CONFIG_RG_GZIP_COMPRESSED_KERNEL
static void gzip_mark(void **ptr)
{
	*ptr = (void *) free_mem_ptr;
}

static void gzip_release(void **ptr)
{
	free_mem_ptr = (long) *ptr;
}

/* ===========================================================================
 * Fill the input buffer. This is called only when the buffer is empty
 * and at least one byte is really needed.
 */
static int fill_inbuf(void)
{
	if (insize != 0) {
		error("ran out of input data\n");
	}

	inbuf = input_data;
	insize = input_len;
	inptr = 1;
	return inbuf[0];
}

/* ===========================================================================
 * Write the output window window[0..outcnt-1] and update crc and bytes_out.
 * (Used for the decompressed data only.)
 */
static void flush_window(void)
{
	ulg c = crc;		/* temporary variable */
	unsigned n;
	uch *in, *out, ch;

	in = window;
	out = &output_data[output_len];
	for (n = 0; n < outcnt; n++) {
		ch = *out++ = *in++;
		c = crc_32_tab[((int) c ^ ch) & 0xff] ^ (c >> 8);
	}
	crc = c;
	bytes_out += (ulg) outcnt;
	output_len += (ulg) outcnt;
	outcnt = 0;
	serial_putc('.');
}

void check_mem(void)
{
	int i;

	puts("\ncplens = ");
	for (i = 0; i < 10; i++) {
	    	puth(cplens[i]);
		puts(" ");
	}
	puts("\ncplext = ");
	for (i = 0; i < 10; i++) {
	    	puth(cplext[i]);
		puts(" ");
	}
	puts("\nborder = ");
	for (i = 0; i < 10; i++) {
	    	puth(border[i]);
		puts(" ");
	}
	puts("\n");
}

static void error(char *x)
{
	check_mem();
	puts("\n\n");
	puts(x);
	puts("byte_count = ");
    	puth(byte_count);
	puts("\n");
	puts("\n\n -- System halted");
	while (1);		/* Halt */
}
#endif

/* addr may be physical OR virtual */
#define program_to_phys(a) _program_to_phys((addr_t)(a))
static inline addr_t _program_to_phys(addr_t addr)
{
#ifdef BOOTLOADER_ADDR_SPACE_IS_VIRTUAL
    /* XXX virt_to_phys should be used, but for Cavium it doesn't work
     * as expected: address FFFFFFFF81100000 is not converted to 1100000 
     * (or 81100000) as it should, but 57FFFFFF81100000 which is neither
     * accessible from boot nor usable by ioremap. So we use 
     * MASK_ADDR_T(addr, 0x1FFFFFFFul) instead of (addr_t)virt_to_phys(addr) */
    return MASK_ADDR_T(addr, 0x1FFFFFFFul);
#else
    return addr;
#endif
}

#ifdef BOOTLOADER_ADDR_SPACE_IS_VIRTUAL
#define virt_to_program(a) ((addr_t)a)
#else
#define virt_to_program(a) ((addr_t)virt_to_phys((addr_t)(a)))
#endif 

static void variable_init(unsigned char *from, unsigned int len)
{
#ifdef CONFIG_RG_GZIP_COMPRESSED_KERNEL
	byte_count = 0;
	checksum = 0;
	input_data = from;
	input_len = len;
#endif
	output_data = virt_to_program(KERNEL_START);
#ifdef MALLOC_START
	free_mem_ptr = (addr_t)MALLOC_START;
#else
	free_mem_ptr = &malloc_start;
#endif
	free_mem_end_ptr = free_mem_ptr + 0x100000;
}

static int decompress_kernel(unsigned char *from, unsigned int len)
{
        int res __attribute__((__unused__));
	variable_init(from, len);
#ifdef CONFIG_ARCH_IXP425
	arch_decomp_setup();
#endif
	puts("Uncompressing Linux...");
#ifdef CONFIG_RG_BZIP2_COMPRESSED_KERNEL
	output_len = 8192*1024; /* output buffer size, 8MB for big kernels */
	res = BZ2_bzBuffToBuffDecompress(output_data, &output_len, from,
	    len, 1, 0);
	
	if (res != BZ_OK)
	    error("error while unzipping image  ");	
#elif defined(CONFIG_RG_GZIP_COMPRESSED_KERNEL)
	gunzip();		// ...see inflate.c
#elif defined(CONFIG_RG_LZMA_COMPRESSED_KERNEL)
	/* output buffer size */
	output_len = (char*)HEAD_ADDR - (char*)output_data;
	/* debug */
	puts("\n");
	PV(output_data);
	PV(output_len);
	/* end debug */
	res = lzma_decode(output_data, output_len, from, len);
	PV(res);
	if (res < 0)
	    error("error while uncompressing");
#else
	memcpy(output_data, from, len);
	output_len = len;
#endif
	puts("\nDone\n");
	return 0;
}

/* Copy cramfs from the loader image to the end of the kernel */
static void copy_cramfs(addr_t to, addr_t from, unsigned int len)
{
    unsigned long *f = (unsigned long *)from;
    unsigned long *t = (unsigned long *)to;
    int i;
    unsigned long crc = 0;
    for (i = 0; i < len/sizeof(*f); i++)
    {
	crc ^= f[i];
	t[i] = f[i];
    }
    PV(crc);
}

static void check_cramfs_crc(addr_t cramfs_start, unsigned int len)
{
    char fan[4] = {'|', '/', '-', '\\'};
    u32 *f = (u32 *)cramfs_start;
    u32 crc = 0, tmp;

    puts("Check CRC ... |");
    len >>= 2;
    for (; len; f++, len--)
    {
	tmp = *f;
	crc += tmp & 0xff;
	tmp >>= 8;
	crc += tmp & 0xff;
	tmp >>= 8;
	crc += tmp & 0xff;
	tmp >>= 8;
	crc += tmp & 0xff;

	if (!(len & 0xffff))
	{
	    serial_putc('\b');
	    serial_putc(fan[(len>>16)&0x3]);
	}
    }
    puth(crc);
    if (crc != CRAMFS_CRC)
	error(" CRAMFS damaged\n");

    puts(" OK\n");
}

#define IS_IN_PHYS_MAPPED_FLASH(addr) \
    (((unsigned long)addr > (unsigned long)cCONFIG_MTD_PHYSMAP_START) && \
    ((unsigned long)addr < ((unsigned long)cCONFIG_MTD_PHYSMAP_START + \
    cCONFIG_MTD_PHYSMAP_LEN)))

/* start_addr: The address we have actually started at
 * head_start_addr: The address we were linked to start at
 */
addr_t do_boot(register addr_t start_addr, register addr_t head_start_addr)
{
    addr_t cramfs_start_src = 0; /* From where to copy */
    addr_t cramfs_start = 0;
    addr_t (*kernel)(addr_t p1, addr_t p2, size_t p3, unsigned long p4);
    addr_t compressed_kernel_start;
    size_t compressed_kernel_len, offset;
    cramfs_location_t cramfs_location;
    extern addr_t _rofs_start, _rofs_end, _compressed_kernel_start,
	   _compressed_kernel_end; 

/* In ld.script this address is aligned to 4k, but page is usually 16k.
 * Is it correct to realign it to page boundary or use explicit align to 4k?*/
#define CRAMFS_TARGET_ADDR virt_to_program(PAGE_ALIGN(KERNEL_END)) 
#define CRAMFS_LEN (((addr_t)&_rofs_end) - ((addr_t)&_rofs_start))
    serial_init();

    offset = start_addr - head_start_addr;
    compressed_kernel_start = ((addr_t)&_compressed_kernel_start) + offset;
    compressed_kernel_len = ((addr_t)&_compressed_kernel_end) -
	((addr_t)&_compressed_kernel_start);

    /* Some debugs prints before uncompressing the kernel */
    puts("\n");
    PV(sizeof(char));
    PV(sizeof(short));
    PV(sizeof(int));
    PV(sizeof(size_t));
    PV(sizeof(long));
    PV(sizeof(addr_t));
    PV(cCONFIG_MTD_PHYSMAP_START);
    PV(cCONFIG_MTD_PHYSMAP_LEN);
    PV(KERNEL_START);
    PV(virt_to_program(KERNEL_START));
    PV(HEAD_ADDR);
    PV(CRAMFS_TARGET_ADDR);
   
    PV(head_start_addr);
    PV(start_addr);
    PV(program_to_phys(start_addr));

    PV(offset);
    PV(&_compressed_kernel_start);
    PV(&_compressed_kernel_end);
    PV(compressed_kernel_start);
    PV(compressed_kernel_len);

    /* rg_param0 contains CRAMFS_PERMST_MAGIC in case the bootloader
     *   loaded us from flash.
     * rg_param1 contains the offset in the MTD partition the 
     *   bootloader loaded us from.
     */
    PV(rg_param0);
    PV(rg_param1);

    decompress_kernel(compressed_kernel_start, compressed_kernel_len);

    PV(&_rofs_start);
    PV(&_rofs_end);

    /* cramfs is linked to the image immediately after the kernel.
     * If CONFIG_RG_MAINFS_IN_FLASH is not set or we know we are running from
     * RAM, we need to copy it to the end of the uncompressed kerenl.
     * kernel arg2: cramfs address
     * kernel arg3: cramfs len
     * kernel arg4: is cramfs located in flash
     * Note: We can use the len of the CramFS from the supper block (second
     * long) if we will need the arg.
     */

    if (!cCONFIG_RG_MAINFS_IN_FLASH)
	cramfs_location = CRAMFS_LOCATION_RAM;
    else if (cCONFIG_RG_MAINFS_IN_UBI)
    {
	if (rg_param0 == CRAMFS_PERMST_MAGIC)
	    cramfs_location = CRAMFS_LOCATION_UBI;
	else 
	    cramfs_location = CRAMFS_LOCATION_RAM;
    }
    else if (IS_IN_PHYS_MAPPED_FLASH(program_to_phys(start_addr)))
	cramfs_location = CRAMFS_LOCATION_PHYS_MAPPED_FLASH;
    else 
	cramfs_location = CRAMFS_LOCATION_RAM;

    switch (cramfs_location)
    {
    case CRAMFS_LOCATION_RAM:
	puts("CramFS in RAM: copy image to proper location\n");
	cramfs_start_src = ((addr_t)&_rofs_start) + offset;
	cramfs_start = CRAMFS_TARGET_ADDR;
	PV(cramfs_start_src);
	copy_cramfs(cramfs_start, cramfs_start_src, CRAMFS_LEN);
	break;
    case CRAMFS_LOCATION_PHYS_MAPPED_FLASH:
	puts("CramFS in physically mapped flash\n");
	cramfs_start = ((addr_t)&_rofs_start) + offset;
	check_cramfs_crc(cramfs_start, CRAMFS_LEN);
	break;
    case CRAMFS_LOCATION_UBI:
	puts("CramFS in '" RG_PROD_STR "' mtd device. Image offset:");
	puth(rg_param1);
	puts("\n");
	cramfs_start = ((addr_t)&_rofs_start) - (addr_t)HEAD_ADDR + 
	    (addr_t)rg_param1;
	break;
    default:
	/* We should never get here! */
	puts("FATAL: CramFS Location UNKNOWN!!!!");
	break;
    }
    puts("Running from : (0x");	puth(start_addr);
    puts(") CramFS will be at: 0x");	puth(cramfs_start);
    puts("\n");

    if (cramfs_location == CRAMFS_LOCATION_RAM || 
	cramfs_location == CRAMFS_LOCATION_PHYS_MAPPED_FLASH)
    {
	DUMP("CramFS dump (offsets in words):", cramfs_start);
	cramfs_start = program_to_phys(cramfs_start);
    }
    kernel = (void*) virt_to_program(KERNEL_ENTRY); 
    PV(kernel);
    DUMP("Kernel dump: ", kernel);

    puts("Booting the kernel\n");

#if defined(CONFIG_ARCH_IXP425)
    {
	extern void cache_clean_flush(void);
	extern void cache_off(void);

	cache_clean_flush();
	cache_off();
    }
#elif (defined(CONFIG_CPU_MIPS32) || defined(CONFIG_CPU_MIPS64)) && !defined(CONFIG_RG_OS_LINUX_24)
    {
        extern void flush_cache_all(void);

        flush_cache_all();
    }
#endif
    
    return kernel(0x0, cramfs_start, CRAMFS_LEN, 
	(unsigned long)cramfs_location);
}
