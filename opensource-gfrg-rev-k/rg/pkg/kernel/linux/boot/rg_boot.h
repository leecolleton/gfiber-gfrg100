#ifndef _MISC_H_
#define _MISC_H_

#include "rg_cramfs_location.h"

typedef unsigned char* addr_t;

#define MASK_ADDR_T(addr, mask) ((addr_t)(((unsigned long)addr) & mask))

#define RG_BOOT_DEBUG 1

#ifdef RG_BOOT_DEBUG
#define DEBUGP printk
#else
#define DEBUGP(...)
#endif

void openrg_commandline_parse(char *cmdline, addr_t rofs_addr,
    unsigned long len, unsigned long is_in_flash, addr_t flash_start_addr, 
    unsigned long flash_len);

#ifdef CONFIG_RG_MAINFS_CRAMFS
#define ACTUAL_END_OFFSET 0
#else
#define ACTUAL_END_OFFSET \
    ((unsigned long)(rofs_len && \
    cramfs_location != CRAMFS_LOCATION_PHYS_MAPPED_FLASH) ? \
    ((rofs_start_addr + rofs_len) - __pa(&_end)) : 0)
#endif

#endif
