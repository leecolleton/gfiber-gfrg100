/****************************************************************************
 *
 * rg/pkg/build/config_target_os.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "config_opt.h"
#include "create_config.h"

void target_primary_os(void)
{
    if (token_get("CONFIG_RG_OS_LINUX_24"))
	token_set("CONFIG_RG_OS", "LINUX_24");
    else if (token_get("CONFIG_RG_OS_LINUX_26"))
	token_set("CONFIG_RG_OS", "LINUX_26");
}

void target_os_features(char *os)
{
    /* target OS */
    if (!strcmp(os, "LINUX_24"))
	token_set_y("CONFIG_RG_OS_LINUX_24");
    else if (!strcmp(os, "LINUX_26"))
	token_set_y("CONFIG_RG_OS_LINUX_26");

    /* Linux Kernel generic configs */
    if (token_get("CONFIG_RG_OS_LINUX_26"))
    {
	token_set_y("CONFIG_RG_OS_LINUX");

	/* Generic kernel */
	
	token_set("CONFIG_INIT_ENV_ARG_LIMIT", "32");
	token_set("CONFIG_LOG_BUF_SHIFT", "14");
	token_set_y("CONFIG_BUG");
	token_set("CONFIG_BASE_SMALL", "0");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_PROC_SYSCTL");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_CRC32");
	token_set_y("CONFIG_MMU");
	token_set_y("CONFIG_IOSCHED_NOOP");
        if (!token_get("CONFIG_RG_RGLOADER"))
        {
	    token_set_y("CONFIG_PRINTK");
            token_set_y("CONFIG_IOSCHED_AS");
            token_set_y("CONFIG_IOSCHED_DEADLINE");
            token_set_y("CONFIG_IOSCHED_CFQ");
            token_set("CONFIG_DEFAULT_IOSCHED", "\"anticipatory\"");
        }
        else
            token_set("CONFIG_DEFAULT_IOSCHED", "\"noop\"");

        if (!token_get("CONFIG_RG_RGLOADER"))
            token_set_y("CONFIG_SYSVIPC");
	token_set_y("CONFIG_SLAB");
	token_set("CONFIG_ZONE_DMA_FLAG", "1");
	token_set_y("CONFIG_BLOCK");

	/* Pseudo_filesystems */
	
	token_set_y("CONFIG_RAMFS");

	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    token_set_y("CONFIG_PROC_FS");
	    token_set_y("CONFIG_SYSFS");
	    token_set_m("CONFIG_RG_IPV4");
	}

	/* Modules support */
	
	token_set_y("CONFIG_MODULES");
	if (!token_get("CONFIG_ARCH_SOLOS"))
	    token_set_y("CONFIG_MODULE_UNLOAD");
	if (!token_get("CONFIG_RG_RGLOADER"))
        {
            token_set_y("CONFIG_OBSOLETE_MODPARM");
            token_set_y("CONFIG_KMOD");
        }

	/* Networking (recheck) */
	
	token_set_y("CONFIG_NET");
	token_set_y("CONFIG_NETDEVICES");
	token_set_y("CONFIG_PACKET");
	if (!token_get("CONFIG_RG_RGLOADER"))
            token_set_y("CONFIG_UNIX");
	token_set_y("CONFIG_INET");
	token_set_y("CONFIG_IP_MULTICAST");
	token_set_y("CONFIG_IP_FIB_HASH");
	token_set_y("CONFIG_IP_ADVANCED_ROUTER");
	token_set_y("CONFIG_NET_ETHERNET");
	token_set_y("CONFIG_IP_MULTIPLE_TABLES");
	token_set_y("CONFIG_FIB_RULES");
	token_set_y("CONFIG_XFRM");
	token_set_y("CONFIG_NET_SCH_CLK_JIFFIES");
	token_set("CONFIG_DEFAULT_TCP_CONG", "\"cubic\"");

	/* PTY */
	if (!token_get("CONFIG_RG_RGLOADER"))
        {
            token_set_y("CONFIG_LEGACY_PTYS");
            token_set("CONFIG_LEGACY_PTY_COUNT", "16");
        }

	/* Override only if LZMA is not fast enough */
        if (!token_get_str("CONFIG_RG_KERNEL_COMP_METHOD"))
            token_set("CONFIG_RG_KERNEL_COMP_METHOD", "lzma");
    }

    if (token_get("CONFIG_RG_OS_LINUX_24"))
    {
	token_set_y("CONFIG_UID16");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_IP_MULTICAST");
	token_set_y("CONFIG_IP_ADVANCED_ROUTER");
	token_set_y("CONFIG_UNIX");
	token_set_y("CONFIG_RG_OS_LINUX");
	token_set_y("CONFIG_INET");
	token_set_y("CONFIG_MODULES");
	token_set_y("CONFIG_NET");
	token_set_y("CONFIG_NETDEVICES");
	token_set_y("CONFIG_NETLINK_DEV");
	token_set_y("CONFIG_FILTER");
	token_set_y("CONFIG_NET_ETHERNET");
	token_set_y("CONFIG_PACKET");
	token_set_y("CONFIG_SYSVIPC");

	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    token_set_y("CONFIG_PROC_FS");
	    token_set_m("CONFIG_RG_IPV4");
	}
	
	/* Override only if LZMA is not fast enough */
        if (!token_get_str("CONFIG_RG_KERNEL_COMP_METHOD"))
            token_set("CONFIG_RG_KERNEL_COMP_METHOD", "lzma");
    }
	
    if (token_get("CONFIG_RG_OS_LINUX"))
	token_set_y("CONFIG_RG_TTYP");

    /* Set CONFIG_RG_TARGET_xxx */
    if (token_get("CONFIG_RG_OS_LINUX"))
	token_set_y("CONFIG_RG_TARGET_LINUX");
}

void target_os_enable_wireless(void)
{
    token_set_y("CONFIG_NET_RADIO");
    token_set_y("CONFIG_NET_WIRELESS");
    if (token_get("CONFIG_RG_OS_LINUX_26"))
	token_set_y("CONFIG_WIRELESS_EXT");
}
