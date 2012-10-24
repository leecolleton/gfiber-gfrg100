/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/rg_prom.c
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

#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/io.h>

#include <boot/rg_boot.h>
#include <boot/rg_cramfs_location.h>

/* rofs_start_addr is virtual. Used by MM to exclude the cramdisk data from its
 * managed memory pool */
extern addr_t rofs_start_addr;
extern unsigned long rofs_len;

#ifdef CONFIG_RG_OS_LINUX_24
#define PHYS_MAPPED_FLASH_NAME "phys_mapped_flash"
#else
#define PHYS_MAPPED_FLASH_NAME "physmap-flash.0"
#endif

static void mtdpart_add(char *mtdparts, char *str)
{
    if (!str || !*str)
	return;
    sprintf(mtdparts+strlen(mtdparts), "%s%s", *mtdparts ? ";" : "", str);
}

/* 
 * rofs_addr is physical, i.e. acceptable for ioremap, and points to the
 * beginning of the cramdisk.
 * flash_start_addr is the physical address of the start of the flash.
 */
void openrg_commandline_parse(char *cmdline, addr_t rofs_addr,
    unsigned long len, unsigned long cramfs_location, 
    addr_t flash_start_addr, unsigned long flash_len)
{
    static char mtdparts[256];
    static char slram[256];
    static char phys_mapped_flash[128];
    static char cramfs_in_ubi[128];
#ifdef CONFIG_RG_OS_LINUX_24
#ifdef CONFIG_RG_INITFS_CRAMFS
    extern unsigned char __crd_init_start, __crd_init_end;
#endif
#endif
    DEBUGP("rofs_addr: %p (%p), len: %lx, "
	"flash_start_addr: %p (%p) flash_len: %lx\n",
	rofs_addr, phys_to_virt((unsigned long)rofs_addr), len, 
	flash_start_addr, (void*)virt_to_phys(flash_start_addr), flash_len);

    if (flash_len)
    {
	/* mtdparts: define the whole flash as a single partition */
	sprintf(phys_mapped_flash, "%s:0x%lx@0(%s)", PHYS_MAPPED_FLASH_NAME,
	    flash_len, CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION);
    }

    sprintf(cmdline,"cramfs=%d ", (int)cramfs_location);

    if (cramfs_location == CRAMFS_LOCATION_RAM)
    {
	/* rofs (ReadOnly FS) is in RAM. */
	printk("CramFS found in RAM\n");

#ifndef CONFIG_RG_MAINFS_CRAMFS
	/* CONFIG_RG_MAINFS_CRAMFS places mainfs image inside kernel,
	 * no adjustment of the end-of-kernel symbols is needed.
	 */
	rofs_start_addr = phys_to_virt((unsigned long)rofs_addr);
	rofs_len = len;
#endif

	/* slram: define a virtual block-device (slram) on cramdisk data, 
	 * so it can be mounted */
	sprintf(slram, "slram=%s,0x%p,+%lu",
	    CONFIG_RG_MAINFS_MTDPART_NAME, rofs_addr, len);
    }
    else if (cCONFIG_RG_MAINFS_IN_FLASH)
    {
	addr_t cramfs_offset = (addr_t)(rofs_addr - flash_start_addr);

	printk("CramFS in Flash\n");
	sprintf(cmdline+strlen(cmdline),"cramfs_offset=%p ", cramfs_offset);
	/* TODO: phys_mapped_flash -> something... */

	/* mtdparts:
	 * add a subpartition inside the flash, which contains the cramdisk
	 * only data */
	if (cCONFIG_RG_MAINFS_IN_UBI)
	{
	    sprintf(cramfs_in_ubi, "%s:0x%lx@0x%p(%s)",
		CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION, len, cramfs_offset,
		CONFIG_RG_MAINFS_MTDPART_NAME);
	}
	else if (cCONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH)
	{
	    sprintf(phys_mapped_flash + strlen(phys_mapped_flash),
		",0x%lx@0x%p(%s)", len, cramfs_offset,
		CONFIG_RG_MAINFS_MTDPART_NAME);
	}
	else
	    panic("mainfs location was not specified!");
    }
    else
	panic("Can not create command line for rofs device");

#ifdef CONFIG_RG_OS_LINUX_24
#ifdef CONFIG_RG_INITFS_CRAMFS
    /* Linux 2.4 also uses initfs, which is always linked with the kernel,
     * and resides always in RAM, thus is accessed using slram */
    if (*slram)
	sprintf(slram + strlen(slram), ",");
    else
	sprintf(slram, "slram=");

    sprintf(slram + strlen(slram), "%s,0x%p,+%d", "initfs",
	(addr_t)virt_to_phys(&__crd_init_start),
	(&__crd_init_end - &__crd_init_start));
#endif
#endif

    DEBUGP("rofs_start_addr: %p len: %lx\n", rofs_start_addr, rofs_len);

    mtdpart_add(mtdparts, phys_mapped_flash);
    mtdpart_add(mtdparts, cCONFIG_RG_CMDLINE_MTDPARTS);
    mtdpart_add(mtdparts, cramfs_in_ubi);

    sprintf(cmdline+strlen(cmdline), " %s%s %s", *mtdparts ? "mtdparts=" : "",
	mtdparts, slram);
#ifdef CONFIG_CMDLINE
    sprintf(cmdline+strlen(cmdline), " %s", CONFIG_CMDLINE);
#endif
}
