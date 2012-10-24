/****************************************************************************
 *
 * rg/pkg/build/hw_config.c
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
#include <stdarg.h>
#include <string.h>
#include "config_opt.h"
#include "create_config.h"
#include <str.h>

#ifdef CONFIG_RG_DO_DEVICES
static int wireless_vaps_add(char *wl_name, dev_if_type_t wl_type,
    char *vap_name_fmt, int start_idx, wl_net_type_t *vap_order);
static void ralink_rt2xxx_add(dev_if_type_t type, char *wl_name, int start_idx,
    char *config);
static void wireless_phy_dev_add(dev_if_type_t type, char *wl_name,
    wl_net_type_t net_type, int enslaved);
static void wireless_vap_add(dev_if_type_t type, char *wl_name, char *vap_name,
    wl_net_type_t vap_net_type, int is_enabled, int enslaved);
#else
#define wireless_vaps_add(...)
#define ralink_rt2xxx_add(...)
#define wireless_phy_dev_add(...)
#define wireless_vap_add(...)
#define atheros_ar5xxx_add_ordered(...)
#endif

#ifdef CONFIG_RG_DO_DEVICES
typedef struct wl_net_type_properties_t {
    wl_net_type_t wl_net_type;
    char *vap_config;
    int is_scr;
    int is_web_auth;
    char *depend_on_config;
    int enabled_by_default;
    int is_enslaved;
} wl_net_type_properties_t;

static wl_net_type_properties_t wl_net_type_properties[] = {
    {WL_NET_TYPE_INSTALLTIME, "CONFIG_RG_VAP_INSTALLTIME", 1, 0, NULL, 1, 1},
    {WL_NET_TYPE_HOME, "CONFIG_RG_VAP_HOME", 0, 0, NULL, 0, 1},
    {WL_NET_TYPE_HOME_SECURED, "CONFIG_RG_VAP_SECURED", 0, 0, NULL, 0, 1},
    /* Helpline is created enabled in rg_conf. The actual state of the device
     * is not Senabled in rg_conf, but rather Svolatile_enabled in rg_conf_ram.
     * When VAP is created, Svolatile_enabled is set to 0 and then it is
     * set to 1 when JRMS status becomes GOOD. */
    {WL_NET_TYPE_HELPLINE, "CONFIG_RG_VAP_HELPLINE", 1, 0, "CONFIG_RG_HELPLINE",
	1, 0},
    {WL_NET_TYPE_PUBLIC, "CONFIG_RG_VAP_PUBLIC", 0, 0, NULL, 1, 0},
    {-1},
};

wl_net_type_t default_vap_order[] = {
    WL_NET_TYPE_INSTALLTIME,
    WL_NET_TYPE_HOME,
    WL_NET_TYPE_HOME_SECURED,
    WL_NET_TYPE_HELPLINE,
    -1
};

wl_net_type_t first_vap_secured_order[] = {
    WL_NET_TYPE_HOME_SECURED,
    WL_NET_TYPE_INSTALLTIME,
    WL_NET_TYPE_HOME,
    WL_NET_TYPE_HELPLINE,
    -1
};

wl_net_type_t fibertec_vap_order[] = {
    WL_NET_TYPE_HOME,
    WL_NET_TYPE_HOME_SECURED,
    WL_NET_TYPE_PUBLIC,
    -1
};

static wl_net_type_properties_t *wl_net_type_properties_get(
    wl_net_type_t wl_net_type)
{
    wl_net_type_properties_t *wl;

    for (wl=wl_net_type_properties; wl->wl_net_type !=-1 && 
	wl->wl_net_type!=wl_net_type; wl++);
    if (wl->wl_net_type==-1)
        conf_err("Unknown wl_net_type %d!", wl_net_type);
    return wl;
}

void wireless_features(void)
{
    wl_net_type_properties_t *wl;

    /* Add default vaps if none were specified */
    if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN") &&
	!token_get("CONFIG_RG_VAP_HOME") &&
	!token_get("CONFIG_RG_VAP_SECURED") &&
	!token_get("CONFIG_RG_VAP_HELPLINE") &&
	!token_get("CONFIG_RG_VAP_INSTALLTIME") &&
        !token_get("CONFIG_RG_VAP_PUBLIC"))
    {
	if (token_get("MODULE_RG_REDUCE_SUPPORT"))
	{
	    token_set_y("CONFIG_RG_VAP_INSTALLTIME");
	    token_set_y("CONFIG_RG_VAP_HELPLINE");
	}
	token_set_y("CONFIG_RG_VAP_HOME"); /* Web-auth */
	token_set_y("CONFIG_RG_VAP_SECURED"); /* WPA */
        token_set_y("CONFIG_RG_VAP_PUBLIC");
    }

    if (!token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
        return;

    for (wl=wl_net_type_properties; wl->wl_net_type !=-1; wl++)
    {
	if (!token_get(wl->vap_config) && !token_get("CONFIG_RG_JPKG"))
	    continue;

	if (wl->depend_on_config)
	    token_set_y(wl->depend_on_config);
	if (wl->is_web_auth)
	    token_set_y("CONFIG_RG_WEB_AUTH");
    }
}
#endif

static void mtd_physmap_add(long start, long len, int width)
{
    token_set("CONFIG_MTD_PHYSMAP_START", "0x%lx", start);
    token_set("CONFIG_MTD_PHYSMAP_LEN", "0x%lx", len);

    /* TODO: We need to fix this genericaly for NOR flash etc. */
    if (!token_get("CONFIG_MTD_COMCERTO_NOR"))
	token_set_y("CONFIG_MTD_PHYSMAP");
    token_set_y("CONFIG_MTD_CMDLINE_PARTS");
    token_set_y("CONFIG_MTD_SLRAM");

    if (token_get("CONFIG_MTD_PHYSMAP"))
    {
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set("CONFIG_MTD_PHYSMAP_BANKWIDTH", "%d", width);
	else
	    token_set("CONFIG_MTD_PHYSMAP_BUSWIDTH", "%d", width);
    }
}

#ifdef CONFIG_RG_DO_DEVICES
static dev_if_type_t config2type(code2str_t *list)
{
    for (; list->code != -1; list++)
    {
	if (token_get(list->str))
	    return list->code;
    }
    conf_err("Hardware (vap type) is not specified");

    /* not reached */
    return -1;
}

static dev_if_type_t get_vap_type(void)
{
    code2str_t vap_types[] = {
	{ DEV_IF_AR5212_VAP_SLAVE, "CONFIG_RG_ATHEROS_HW_AR5212" },
	{ DEV_IF_AR5416_VAP_SLAVE, "CONFIG_RG_ATHEROS_HW_AR5416" },
	{ DEV_IF_RT2561_VAP, "CONFIG_HW_80211G_RALINK_RT2561" },
	{ DEV_IF_RT2860_VAP, "CONFIG_HW_80211N_RALINK_RT2860" },
	{ DEV_IF_RT2860_VAP, "CONFIG_HW_80211N_RALINK_RT3062" },
	{ DEV_IF_RT2880_VAP, "CONFIG_HW_80211N_RALINK_RT2880" },
	{ DEV_IF_RT2880_VAP, "CONFIG_HW_80211N_RALINK_RT3052" },
	{ DEV_IF_BCM43XX_VAP, "CONFIG_HW_80211G_BCM43XX" },
	{ DEV_IF_UML_WLAN_VAP, "CONFIG_HW_80211G_UML_WLAN" },
	{ -1, NULL }
    };

    return config2type(vap_types);
}

static void wl_net_type_set(char *wl_name, wl_net_type_t wl_net_type)
{
    wl_net_type_properties_t *wl;

    /* Do not set type in conf if no SCR */
    if (token_get("MODULE_RG_REDUCE_SUPPORT"))
	dev_wl_net_type(wl_name, wl_net_type);

    wl = wl_net_type_properties_get(wl_net_type);
    if (wl->is_web_auth && !(wl_net_type == WL_NET_TYPE_INSTALLTIME &&
	token_get("CONFIG_RG_INSTALLTIME_WPA")))
    {
	dev_set_web_auth(wl_name);
    }
}

static void wireless_dev_add(dev_if_type_t type, char *wl_name,
    wl_net_type_t net_type, int is_enabled, int enslaved)
{
    if (token_get("CONFIG_RG_JPKG"))
    {
	token_set_dev_type(type);
	return;
    }

    if (is_enabled)
	dev_add(wl_name, type, DEV_IF_NET_INT);
    else
	dev_add_disabled(wl_name, type, DEV_IF_NET_INT);

    if (enslaved)
	dev_add_to_bridge("br0", wl_name);
    if (net_type != -1)
	wl_net_type_set(wl_name, net_type);
}

static void wireless_phy_dev_add(dev_if_type_t type, char *wl_name,
    wl_net_type_t net_type, int enslaved)
{
    wireless_dev_add(type, wl_name, net_type, 1, enslaved);
    dev_can_be_missing(wl_name);
}

static void wireless_vap_add(dev_if_type_t type, char *wl_name, char *vap_name,
    wl_net_type_t vap_net_type, int is_enabled, int enslaved)
{
    wireless_dev_add(type, vap_name, vap_net_type, is_enabled, enslaved);
    dev_set_dependency(vap_name, wl_name);
}

/* Add wireless devices and return the number of added vaps */
static int wireless_vaps_add(char *wl_name, dev_if_type_t wl_type,
    char *vap_name_fmt, int start_idx, wl_net_type_t *vap_order)
{
    char *name = NULL;
    int enslaved;
    wl_net_type_properties_t *wl;
    wl_net_type_t *v;
    int is_first = 1;
    int ret = 0;

    if (!vap_order)
        vap_order = default_vap_order;

    for (v=vap_order; *v!=-1; v++)
    {
        wl = wl_net_type_properties_get(*v);

	/* allow JPKG dists to enable dev types (CONFIG_HW_80211G_xxx) without
	 * specifying vap configs */
	if (!token_get(wl->vap_config) && !token_get("CONFIG_RG_JPKG"))
	    continue;

	if (wl->is_scr && !token_get("MODULE_RG_REDUCE_SUPPORT"))
	    continue;

	enslaved = token_get("CONFIG_DEF_BRIDGE_LANS") && wl->is_enslaved;

	if (is_first)
	{
	    /* First VAP */
	    wireless_phy_dev_add(wl_type, wl_name, *v, enslaved);
	    is_first = 0;
	}
	else
	{
	    if (!vap_name_fmt)
	        continue;

	    wireless_vap_add(get_vap_type(), wl_name,
		*str_printf(&name, vap_name_fmt, start_idx),
		*v, wl->enabled_by_default, enslaved);
	    start_idx++;
	}
	ret++;
    }

    str_free(&name);
    return ret;
}

static void ralink_rt2xxx_add(dev_if_type_t type, char *wl_name, int start_idx,
    char *config)
{
    token_set_m(config); 
    wireless_vaps_add(wl_name, type, "ra%d", start_idx, NULL);
}
#endif

void airgo_agn100_add(void)
{
    wireless_vaps_add("wlan0", DEV_IF_AGN100, NULL, 0, NULL);
}

static void ralink_rt2560_add(char *wl_name)
{
#ifdef CONFIG_RG_DO_DEVICES
    token_set_m("CONFIG_RALINK_RT2560");
#endif
    wireless_vaps_add(wl_name, DEV_IF_RT2560, NULL, 0, NULL);
}

static void ralink_rt2561_add(char *wl_name)
{
    ralink_rt2xxx_add(DEV_IF_RT2561, wl_name, 8, "CONFIG_RALINK_RT2561");
}

static void ralink_rt2860_add(char *wl_name)
{
    ralink_rt2xxx_add(DEV_IF_RT2860, wl_name, 1, "CONFIG_RALINK_RT2860");
}

static void ralink_rt2880_add(char *wl_name)
{
#ifdef CONFIG_RG_DO_DEVICES
    int i, vap_count;
    char *dev = NULL;

    token_set_m("CONFIG_RALINK_RT2880"); 
    vap_count = wireless_vaps_add(wl_name, DEV_IF_RT2880, "ra%d", 1, 
        first_vap_secured_order);
    /* Set Ralink devices separation state (by ap_vlan_id), excluding
     * the first device (ra0) - it must be already non-separated.
     */
    for (i=1; i<vap_count; i++)
    {
	str_printf(&dev, "ra%d", i);
	dev_set_ap_vlan_id(dev, i+1);
    }
    str_free(&dev);
#endif
}

static void bcm43xx_add(char *wl_name)
{
    wireless_vaps_add(wl_name, DEV_IF_BCM43XX, "wl0.%d", 1, NULL);
}

void isl_softmac_add(void)
{
    dev_add("eth0", DEV_IF_ISL_SOFTMAC, DEV_IF_NET_INT);
    dev_can_be_missing("eth0");
}

#ifdef CONFIG_RG_DO_DEVICES
static void atheros_ar5xxx_add_ordered(wl_net_type_t *vap_order)
{
    code2str_t ath_first_vap_types[] = {
	{ DEV_IF_AR5212_VAP, "CONFIG_RG_ATHEROS_HW_AR5212" },
	{ DEV_IF_AR5416_VAP, "CONFIG_RG_ATHEROS_HW_AR5416" },
	{ -1, NULL }
    };

    wireless_phy_dev_add(DEV_IF_WIFI, "wifi0", -1, 0); /* not an access point */
    wireless_vaps_add("ath0", config2type(ath_first_vap_types),
	"ath%d", 1, vap_order);
    dev_set_dependency("ath0", "wifi0");
}
#endif

static void atheros_ar5xxx_add(void)
{
    atheros_ar5xxx_add_ordered(NULL);
}

static void marvell_8xxx_add(int dev_idx, int num_vaps)
{
    int i;
    char *devname = NULL, *vapname = NULL;

    str_printf(&devname, "wdev%d", dev_idx);
    wireless_phy_dev_add(DEV_IF_WIFI, devname, -1, 0);

    /* Add Virtual Access Points */
    for (i = 0; i < num_vaps; i++)
    {
	str_printf(&vapname, "%sap%d", devname, i);
	wireless_vap_add(DEV_IF_MV8764_VAP, devname, vapname, -1, 1, 0);
    }
}

static void cameroon_ife6_hw_config(void)
{
    token_set_m("CONFIG_FUSIV_KERNEL_ACL");
    token_set_y("CONFIG_FUSIV_KERNEL_ACL_SPA");
    token_set_y("CONFIG_FUSIV_KERNEL_BME_DRIVER_VX180");
    token_set_m("CONFIG_FUSIV_KERNEL_VX180_AVDSL");
}

void hardware_features(void)
{
    option_t *hw_tok;

    if (!hw)
    {
	token_set("CONFIG_RG_HW", "NO_HW");
	token_set("CONFIG_RG_HW_DESC_STR", "No hardware - local targets only");
	token_set_y("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY");
	return;
    }

    hw_tok = option_token_get(openrg_hw_options, hw);

    if (!hw_tok->value)
	conf_err("No description available for HW=%s\n", hw);

    token_set("CONFIG_RG_HW", hw);
    token_set("CONFIG_RG_HW_DESC_STR", hw_tok->value);

    if (IS_HW("DANUBE") || IS_HW("TWINPASS") || IS_HW("IAD303"))
    {
	if (IS_HW("DANUBE"))
	{
	    token_set("BOARD", "Danube");
	    token_set_y("CONFIG_RG_HW_DANUBE");
	    token_set_y("CONFIG_RG_CPU_DANUBE");
	}
	else if (IS_HW("IAD303"))
	{
	    token_set("BOARD", "IAD303");
	    token_set_y("CONFIG_RG_HW_IAD303");
	    token_set_y("CONFIG_RG_CPU_DANUBE");
	}
	else
	{
	    token_set("BOARD", "Twinpass");
	    token_set_y("CONFIG_RG_HW_TWINPASS");
	    token_set_y("CONFIG_RG_CPU_TWINPASS");
	}
	token_set("FIRM", "Infineon");

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH",
		"vendor/infineon/danube/modules");
	}
	/* TODO: Add to CONFIG_CMDLINE mem=memsize (32?) */
	set_big_endian(1);
	token_set("LIBC_ARCH", "mips");
	token_set("ARCH", "mips");
	token_set_y("CONFIG_HAS_MMU");
	if (IS_HW("IAD303"))
	    token_set("CONFIG_BOOTLDR_UBOOT_COMP", "none");
	else
	    token_set("CONFIG_BOOTLDR_UBOOT_COMP", "lzma");
	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_RAMFS");
	token_set_y("CONFIG_CRAMFS_FS");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS1");
	token_set_y("CONFIG_BOOTLDR_UBOOT");

	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_NEW_IRQ");
	token_set_y("CONFIG_NONCOHERENT_IO");
	token_set_y("CONFIG_NEW_TIME_C");

	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_PCI_AUTO");
	token_set_y("CONFIG_DANUBE_PCI");
	token_set_y("CONFIG_DANUBE_PCI_HW_SWAP");

#if 0
	/* TODO: */
	token_set_y("CONFIG_SCSI");
	token_set_y("CONFIG_BLK_DEV_SD");
	token_set("CONFIG_SD_EXTRA_DEVS", "5");
#endif
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_KCORE_ELF");
	token_set_y("CONFIG_BINFMT_ELF");

	if (token_get("CONFIG_HW_ENCRYPTION"))
	{
	    token_set_y("CONFIG_CRYPTO_DANUBE");
	}

	/* MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CHAR");
	token_set_y("CONFIG_MTD_BLOCK");
	token_set_y("CONFIG_MTD_CFI");
	token_set_y("CONFIG_MTD_GEN_PROBE");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set_y("CONFIG_MTD_DANUBE");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_SLRAM");
	token_set("CONFIG_MTD_PHYSMAP_START", "0x10000000"); //"0xb0000000");
	if (IS_HW("IAD303"))
	{
	    token_set("CONFIG_SDRAM_SIZE", "64");
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "32");
	    token_set("CONFIG_MTD_PHYSMAP_LEN", "0x02000000");
	}
	else
	{
	    token_set("CONFIG_SDRAM_SIZE", "32");
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");
	    token_set("CONFIG_MTD_PHYSMAP_LEN", "0x00800000");
	}

	token_set_y("CONFIG_BLK_DEV_LOOP");

	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_IFX_ASC");
	token_set_y("CONFIG_SERIAL_IFX_ASC_CONSOLE");
	token_set("CONFIG_IFX_ASC_DEFAULT_BAUDRATE", "115200");
	token_set_y("CONFIG_IFX_ASC_CONSOLE_ASC1");

	token_set_y("CONFIG_DANUBE");
	token_set_y("CONFIG_DANUBE_MPS");
	token_set_y("CONFIG_DANUBE_MPS_PROC_DEBUG");
	token_set_y("CONFIG_DANUBE_MPS_EVENT_MBX");
	token_set("CONFIG_DANUBE_MPS_HISTORY", "128");
	token_set("CONFIG_DANUBE_FW_SIZE", "2");

	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}

	/* Networking */
	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_y("CONFIG_ATM_DANUBE");
	    token_set_y("CONFIG_IFX_ATM_OAM");
	    token_set_y("CONFIG_DANUBE_MEI");
	    token_set_y("CONFIG_DANUBE_MEI_MIB");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("atm0", DEV_IF_DANUBE_ATM, NULL);
	}

	if (token_get("CONFIG_HW_VDSL_WAN"))
	{
	    token_set_m("CONFIG_DANUBE_PPA");
	    token_set_y("CONFIG_RG_VINAX_VDSL");
	    token_set("CONFIG_RG_VINAX_VDSL_BASE_ADDR", "0x14000000");
	    token_set("CONFIG_RG_VINAX_VDSL_IRQ", "99");
	    dev_add("eth1", DEV_IF_VINAX_VDSL, DEV_IF_NET_EXT);

	    /* VDSL daemon application require threads support. */
	    token_set_y("CONFIG_RG_LIBC_CUSTOM_STREAMS");

	    token_set_y("CONFIG_HW_ETH_WAN");
	}

	/* Must be after VDSL because CONFIG_DANUBE_PPA conflicts with
	 * CONFIG_DANUBE_ETHERNET. */
	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set("CONFIG_RG_DANUBE_ETH0_DMA_OFFSET", "2");
	    if (!token_get("CONFIG_DANUBE_PPA") ||
		token_get("CONFIG_RG_JPKG"))
	    {
		token_set_y("CONFIG_DANUBE_ETHERNET");
	    }
	    if (token_get("CONFIG_HW_SWITCH_LAN"))
	    {
		if (IS_HW("DANUBE") || IS_HW("IAD303"))
		{
		    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
		    {
			dev_add("eth0", DEV_IF_DANUBE_ETH, DEV_IF_NET_INT);
			switch_dev_add("eth0_main", "eth0", DEV_IF_ADM6996_HW_SWITCH,
			    DEV_IF_NET_INT, 5);
			/* After a 14 bytes Ethernet header the IP header is not 
			 * aligned. Danube net driver adds 2 to skb->data to
			 * align the IP header. In case of port separation a 6
			 * byte S-Tag is added to the Ethernet header. As a
			 * result a 0 alignement offset is required */
			token_set("CONFIG_RG_DANUBE_ETH0_DMA_OFFSET", "0");
		    }
		    else
		    {
			switch_dev_add("eth0", NULL, DEV_IF_ADM6996_HW_SWITCH,
			    DEV_IF_NET_INT, 5);
		    }

		    if (IS_HW("IAD303"))
			token_set("CONFIG_HW_SWITCH_FIRST_PHY", "1");
		}
		if (IS_HW("TWINPASS"))
		{
		    switch_dev_add("eth0", NULL, DEV_IF_PSB6973_HW_SWITCH,
			DEV_IF_NET_INT, 6);
		}
	    }
	}

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "1");
	    atheros_ar5xxx_add();
	}

	if (token_get("CONFIG_HW_80211N_RALINK_RT2860"))
	    ralink_rt2860_add("ra0");

	if (token_get("CONFIG_HW_80211N_RALINK_RT3062"))
	{
	    ralink_rt2860_add("ra0");
	    token_set_y("CONFIG_RALINK_RT3062"); 
	}

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_VINETIC");
	    token_set("CONFIG_VINETIC_LINES_PER_CHIP", "2");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    if (token_get("CONFIG_HW_DECT"))
	    {
		token_set("CONFIG_HW_NUMBER_OF_DECT_LINES", "4");
		token_set("CONFIG_HW_MAX_DECT_HANDSETS", "6");
	    }
	    token_set_y("CONFIG_RG_DSP_THREAD");
	    if (token_get("CONFIG_HW_FXO"))
	    {
		token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
		/* Use for IAD303 with DUSLIC FXO */
		token_set_m("CONFIG_DUSLIC");
	    }
	}

	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_RG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_GENERIC_ISA_DMA");
	    token_set_y("CONFIG_USB_EHCI_HCD");
	    /* the host controller driver MUST be 'y' and not 'm' because on
	     * some DANUBE boards the insmode halts the board (B45740) */
	    token_set_m("CONFIG_USB_DWC3884_HCD");

	    if (IS_HW("IAD303"))
		token_set_y("CONFIG_HW_USB_OVERCURRENT_BY_VOLTAGE");
	}

	if (token_get("CONFIG_RG_HW_QOS") && IS_HW("TWINPASS"))
	    token_set("CONFIG_RG_QOS_PRIO_BANDS", "4");

	if (token_get("CONFIG_ATM"))
	    token_set_y("CONFIG_ATM_VBR_AS_NRT");
	
#if 0
	token_set_y("CONFIG_NET_HW_FLOWCONTROL"); /* Check... */
#endif
    }
    if (IS_HW("ARX188"))
    {
	token_set("BOARD", "ARX188");
	token_set("FIRM", "Lantiq");

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_IFX_OS");
	    token_set_m("CONFIG_IFX_TAPI");
	    token_set_m("CONFIG_IFX_VMMC");

	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set_m("CONFIG_RG_XWAY_JDSP");
	    token_set_y("CONFIG_XRX");
	    token_set_y("CONFIG_IFX_VOICE_CPE_TAPI_FAX");
	}

	token_set_y("CONFIG_AR9");
	token_set_y("CONFIG_AR9_REF_BOARD");

#if 0
	token_set_y("CONFIG_BOOTLDR_UBOOT");
	token_set("CONFIG_BOOTLDR_UBOOT_COMP", "lzma");
#endif

	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");
	token_set("CONFIG_SDRAM_SIZE", "32");

	token_set("CONFIG_MTD_PHYSMAP_START", "0x10000000");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x00800000");
	token_set("CONFIG_MTD_IFX_NOR_FLASH_SIZE", "8");
#if 0
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");
#endif
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS1");
	token_set("CONFIG_CMDLINE", "console=ttyS1,115200");

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set_y("CONFIG_IFX_PPA");
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH", 
		"vendor/lantiq/xway/rg/modules");
	}
	
	if (token_get("CONFIG_IFX_PPA"))
	{
	    token_set_y("CONFIG_IFX_PPA_A5");
	    token_set_y("CONFIG_IFX_PPA_IPv6_ENABLE");
	    token_set_y("CONFIG_IFX_PPA_DATAPATH");
	    /* AR9-A5 datapath driver demands ATM */
	    token_set_y("CONFIG_ATM");
	}
	else 
	{
	    if (token_get("CONFIG_HW_ETH_LAN") || 
		token_get("CONFIG_HW_ETH_WAN"))
	    {
		/* Note:
		 * CONFIG_IFX_3PORT_SWITCH is the driver for the AR9 internal 
		 * MAC. It MUST NOT be used concurrently with the AR9 datapath 
		 * driver (CONFIG_IFX_PPA_DATAPATH).
		 */
		token_set_y("CONFIG_IFX_ETH_FRAMEWORK");
		token_set_y("CONFIG_IFX_3PORT_SWITCH");
		if (token_get("CONFIG_HW_ETH_WAN"))
		    token_set_y("CONFIG_SW_ROUTING_MODE");
	    }
	    if (token_get("CONFIG_HW_DSL_WAN"))
	    {
		/* Note:
		 * CONFIG_IFX_ATM is the driver for the A1 ATM firmware.
		 * It MUST NOT be used concurrently with the AR9 datapath 
		 * driver (CONFIG_IFX_PPA_DATAPATH).
		 */
		token_set_y("CONFIG_IFX_ATM");
	    }
	}
	if (!token_get("CONFIG_LSP_DIST"))
	{
	    if (token_get("CONFIG_HW_ETH_LAN"))
            {
		dev_add("eth0", DEV_IF_PSB6973_HW_SWITCH, DEV_IF_NET_INT);
                token_set_y("CONFIG_PSB6973_HAVE_EXTERNAL_PHY");
            }
	    if (token_get("CONFIG_HW_ETH_WAN"))
		dev_add("eth1", DEV_IF_LANTIQ_AR9_ETH, DEV_IF_NET_EXT);
            if (token_get("CONFIG_HW_DSL_WAN"))
            {
		token_set_m("CONFIG_IFX_OS");
                token_set_y("CONFIG_IFX_ATM_OAM");
                token_set_y("CONFIG_RG_ATM_QOS");
		dev_add("dsl0", DEV_IF_LANTIQ_XWAY_DSL, DEV_IF_NET_EXT);
		dsl_atm_dev_add("atm0", DEV_IF_LANTIQ_XWAY_ATM, "dsl0");

		/* xDSL daemon application requires threads support. */
		token_set_y("CONFIG_RG_LIBC_CUSTOM_STREAMS");
            }

	    if (token_get("CONFIG_HW_80211N_LANTIQ_WAVE300"))
		wireless_phy_dev_add(DEV_IF_WAVE300, "wlan0", -1, 1);
	}
	
        if (token_get("CONFIG_RG_3G_WAN"))
            gsm_dev_add("gsm0", DEV_IF_3G_USB_SERIAL);

	if (token_get("CONFIG_HW_USB_STORAGE"))
	    token_set_y("CONFIG_USB");

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set_m("CONFIG_RG_ATHEROS");
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "1");
	    atheros_ar5xxx_add();

	    /* TODO: Check if we need it... */
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	}

	token_set_y("CONFIG_PCI");
	if (token_get("CONFIG_HW_ENCRYPTION"))
	{
	    token_set_y("CONFIG_CRYPTO_DEV_DEU");
	    token_set_y("CONFIG_CRYPTO_DEV_AR9");
	}
    }

    if (IS_HW("VRX288") || IS_HW("GRX288"))
    {
	token_set("BOARD", "VRX288");
	token_set("FIRM", "Lantiq");

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_IFX_OS");
	    token_set_m("CONFIG_IFX_TAPI");
	    token_set_m("CONFIG_IFX_VMMC");

	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set_m("CONFIG_RG_XWAY_JDSP");
	    token_set_y("CONFIG_XRX");
	    token_set_y("CONFIG_IFX_VOICE_CPE_TAPI_FAX");
	}

	token_set_y("CONFIG_VR9");
	token_set_y("CONFIG_VR9_REF_BOARD");

	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");
	token_set("CONFIG_SDRAM_SIZE", "64");

	token_set("CONFIG_MTD_PHYSMAP_START", "0x10000000");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x00800000");
	token_set("CONFIG_MTD_IFX_NOR_FLASH_SIZE", "8");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");
	token_set("CONFIG_CMDLINE", "console=ttyS0,115200");

	token_set_y("CONFIG_HW_FLASH_NAND");
	
	if (IS_HW("VRX288"))
	{
	    token_set("CONFIG_HW_NAND_PEB_SIZE", "0x20000");
	    token_set("CONFIG_HW_NAND_MIN_IO_SIZE", "2048");
	    token_set("CONFIG_HW_NAND_SUB_PAGE_SIZE", "512");
	}
	else if (IS_HW("GRX288"))
	{
	    /* We do not fully support the NAND variant on the GRX reference
	     * board (MLC technology) but these physical parameters are correct.
	     */
	    token_set("CONFIG_HW_NAND_PEB_SIZE", "0x40000");
	    token_set("CONFIG_HW_NAND_MIN_IO_SIZE", "2048");
	    token_set("CONFIG_HW_NAND_SUB_PAGE_SIZE", "2048");
	}
	
	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set_y("CONFIG_IFX_PPA");
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH", 
		"vendor/lantiq/xway/rg/modules");
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	    token_set_y("CONFIG_LANTIQ_VR9_PORT_SEP");

	if (token_get("CONFIG_HW_DSL_WAN") || token_get("CONFIG_HW_VDSL_WAN"))
	{
	    token_set_m("CONFIG_IFX_OS");
	    dev_add("dsl0", DEV_IF_LANTIQ_XWAY_DSL, DEV_IF_NET_EXT);

	    /* xDSL daemon application requires threads support. */
	    token_set_y("CONFIG_RG_LIBC_CUSTOM_STREAMS");
	}

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_IFX_ATM_OAM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("atm0", DEV_IF_LANTIQ_XWAY_ATM, "dsl0");
	    if (!token_get("CONFIG_IFX_PPA"))
		token_set_m("CONFIG_IFX_ATM");
	}

	if (token_get("CONFIG_HW_VDSL_WAN"))
	{
	    dsl_ptm_dev_add("ptm0", DEV_IF_LANTIQ_XWAY_PTM, "dsl0");
	    if (!token_get("CONFIG_IFX_PPA"))
		token_set_m("CONFIG_IFX_PTM");
	}

	if (token_get("CONFIG_HW_ETH_LAN") || token_get("CONFIG_HW_ETH_WAN"))
	{
	    token_set_y("CONFIG_GE_MODE");
	    token_set_y("CONFIG_GPHY_DRIVER");
	    token_set_y("CONFIG_IFX_ETH_FRAMEWORK");
	    token_set_y("CONFIG_IFX_ETHSW_API");

	    if (token_get("CONFIG_IFX_PPA"))
	    {
		int drv_count = 0;

		if (token_get("CONFIG_HW_VDSL_WAN"))
		{
		    token_set_m("CONFIG_IFX_PPA_E5");
		    drv_count++;
		}
		if (token_get("CONFIG_HW_DSL_WAN"))
		{
		    token_set_m("CONFIG_IFX_PPA_A5");
		    drv_count++;
		}

		/* In case A5 & E5 are loaded, we decide which driver to load 
		 * at runtime.
		 */
		if (drv_count > 1)
		    token_set_y("CONFIG_RG_LANTIQ_XWAY_DYN_DATAPATH");
		else if (!drv_count)
		    token_set_m("CONFIG_IFX_PPA_D5");


		token_set_y("CONFIG_IFX_PPA_DATAPATH");
		token_set_y("CONFIG_IFX_PPA_API_PROC");
	    }
	    else 
	    {
		/* Note:
		 * CONFIG_IFX_7PORT_SWITCH is the driver for the VR9 internal 
		 * MAC. It MUST NOT be used if CONFIG_IFX_PPA (fastpath) is set.
		 */
		token_set_y("CONFIG_IFX_7PORT_SWITCH");

		if (token_get("CONFIG_HW_ETH_WAN"))
		    token_set_y("CONFIG_SW_ROUTING_MODE");
	    }

	    if (token_get("CONFIG_LANTIQ_VR9_PORT_SEP"))
	    {
		token_set("CONFIG_LANTIQ_VR9_PORT_SEP_NUM", "5");
		token_set("CONFIG_LANTIQ_VR9_VLAN_CPU", "4001");
		token_set("CONFIG_LANTIQ_VR9_VLAN_PORT_SEP", "4002");
	    }
	}

	if (!token_get("CONFIG_LSP_DIST"))
	{
	    if (token_get("CONFIG_HW_ETH_LAN"))
	    {
		switch_dev_add("eth0", NULL, DEV_IF_LANTIQ_VR9_HW_SWITCH, 
		    DEV_IF_NET_INT, 6);
		token_set("CONFIG_HW_SWITCH_FIRST_PHY", "1");
	    }
	    if (token_get("CONFIG_HW_ETH_WAN"))
		dev_add("eth1", DEV_IF_LANTIQ_VR9_ETH, DEV_IF_NET_EXT);
	}

	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_PCIEPORTBUS");

	if (IS_HW("VRX288"))
	    token_set_y("CONFIG_IFX_PCIE_PHY_36MHZ_MODE");
	else if (IS_HW("GRX288"))
	    token_set_y("CONFIG_IFX_PCIE_PHY_25MHZ_MODE");

	if (token_get("CONFIG_HW_80211N_LANTIQ_WAVE300"))
	    wireless_phy_dev_add(DEV_IF_WAVE300, "wlan0", -1, 1);

	if (token_get("CONFIG_HW_USB_STORAGE"))
	    token_set_y("CONFIG_USB");

	if (token_get("CONFIG_HW_ENCRYPTION"))
	{
	    token_set_y("CONFIG_CRYPTO_DEV_DEU");
	    token_set_y("CONFIG_CRYPTO_DEV_VR9");
	}
    }

    if (IS_HW("WADB100G"))
    {
	token_set_y("CONFIG_BCM963XX_COMMON");
	token_set_y("CONFIG_BCM96348");
	
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	    token_set_m("CONFIG_BCM963XX_BOARD");

	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_SERIAL_CORE");
	    token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	    if (token_get("CONFIG_HW_ETH_WAN"))
	    {
		token_set_m("CONFIG_BCM963XX_ETH"); 
		dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_EXT); 
	    }
	}
	
	token_set_y("CONFIG_BCM963XX_SERIAL");

        token_set_y("CONFIG_MTD_CFI_AMDSTD");
	
	token_set("CONFIG_BCM963XX_BOARD_ID", "96348GW-10");
	token_set("CONFIG_BCM963XX_CHIP", "6348");
	token_set("CONFIG_BCM963XX_SDRAM_SIZE", "16");
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "4");
	/* this value is taken from 
	 * vendor/broadcom/bcm963xx/linux-2.6/bcmdrivers/opensource/include/bcm963xx/board.h*/
	token_set("CONFIG_MTD_PHYSMAP_START", "0xbfc00000");
        mtd_physmap_add(0x1FC00000,
	     token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024, 2);

	token_set_y("CONFIG_PCI");

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM963XX_ADSL");
	    token_set_m("CONFIG_BCM963XX_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM963XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);

	    if (!token_get("CONFIG_HW_SWITCH_LAN"))
	    {
		token_set_m("CONFIG_BCM963XX_ETH");
		switch_dev_add("bcm1", NULL, DEV_IF_BCM963XX_ETH,
		    DEV_IF_NET_INT, -1);
	    }
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    switch_dev_add("bcm1", NULL, DEV_IF_BCM5325A_HW_SWITCH,
		DEV_IF_NET_INT, 5);
	}
	
	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	{
	    if (token_get("CONFIG_RG_OS_LINUX_24"))
		token_set_y("CONFIG_BCM963XX_WLAN");
	    if (token_get("CONFIG_RG_OS_LINUX_26"))
                token_set_y("CONFIG_BCM4318");

	    bcm43xx_add("wl0");
	}

	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    token_set_m("CONFIG_BCM963XX_USB");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	token_set("BOARD", "WADB100G");
	token_set("FIRM", "Broadcom");
    }

    if (IS_HW("ASUS6020VI"))
    {
	token_set_y("CONFIG_BCM963XX_COMMON");
	token_set_y("CONFIG_BCM96348");
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	    token_set_m("CONFIG_BCM963XX_BOARD");

	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_SERIAL_CORE");
	    token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	    if (token_get("CONFIG_HW_ETH_WAN"))
	    {
		token_set_m("CONFIG_BCM963XX_ETH"); 
		dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_EXT); 
	    }
	}
	token_set_y("CONFIG_BCM963XX_SERIAL");

        token_set_y("CONFIG_MTD_CFI_AMDSTD");
	
	token_set("CONFIG_BCM963XX_BOARD_ID", "AAM6020VI");
	token_set("CONFIG_BCM963XX_CHIP", "6348");
	token_set("CONFIG_BCM963XX_SDRAM_SIZE", "16");
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "4");

	/* taken from vendor/broadcom/bcm963xx/kernel/bcm963xx/board.h */
        mtd_physmap_add(0x1FC00000,
	     token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024, 2);

	/* this value is taken from 
	 * vendor/broadcom/bcm963xx/linux-2.6/bcmdrivers/opensource/include/bcm963xx/board.h*/
	token_set("CONFIG_MTD_PHYSMAP_START", "0x1fc00000");

	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL"); /* CramFS on flash */

	token_set_y("CONFIG_PCI");

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM963XX_ADSL");
	    token_set_m("CONFIG_BCM963XX_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM963XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    switch_dev_add("bcm1", NULL, DEV_IF_BCM5325E_HW_SWITCH,
		DEV_IF_NET_INT, 5);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_BCM963XX_ETH");
	    dev_add("bcm1", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	{
	    if (token_get("CONFIG_RG_OS_LINUX_24"))
	    {
		token_set_y("CONFIG_BCM963XX_WLAN");
		token_set_y("CONFIG_BCM963XX_WLAN_4318");
	    }
	    if (token_get("CONFIG_RG_OS_LINUX_26"))
		token_set_y("CONFIG_BCM4318");

	    bcm43xx_add("wl0");
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	token_set("BOARD", "ASUS6020VI");
	token_set("FIRM", "Asus");
    }

    if (IS_HW("WADB102GB"))
    {
	token_set_y("CONFIG_BCM963XX_COMMON");
	token_set_y("CONFIG_BCM96348");
	token_set_y("CONFIG_BCM963XX_SERIAL");
	token_set_m("CONFIG_BCM963XX_BOARD");
	token_set("CONFIG_BCM963XX_BOARD_ID", "WADB102GB");
	token_set("CONFIG_BCM963XX_CHIP", "6348");
	token_set("CONFIG_BCM963XX_SDRAM_SIZE", "16");

	token_set_y("CONFIG_RG_MTD_DEFAULT_PARTITION");

	/* for use with pkg/kernel/linux/mtd */
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x00400000");
	token_set("CONFIG_MTD_PHYSMAP_START", "0x1FC00000");

	token_set_y("CONFIG_PCI");

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM963XX_ADSL");
	    token_set_m("CONFIG_BCM963XX_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM963XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_BCM963XX_ETH");
	    dev_add("bcm1", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    switch_dev_add("bcm1", NULL, DEV_IF_BCM5325E_HW_SWITCH,
		DEV_IF_NET_INT, 5);
	}

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	{
	    token_set_y("CONFIG_BCM963XX_WLAN");
	    token_set_y("CONFIG_BCM963XX_WLAN_4318");

	    bcm43xx_add("wl0");
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	token_set("BOARD", "WADB102GB");
	token_set("FIRM", "Belkin");
    }
    
    if (IS_HW("MPC8272ADS"))
    {
	token_set_y("CONFIG_MPC8272ADS");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0" );

	token_set("FIRM", "freescale");
	token_set("BOARD", "MPC8272ADS");

	/* Ethernet drivers */

	if (token_get("CONFIG_HW_ETH_FEC"))
	{
	    /* CPM2 Options */

	    token_set_y("CONFIG_FEC_ENET");
	    token_set_y("CONFIG_USE_MDIO");
            token_set_y("CONFIG_FCC_DM9161");
	    token_set_y("CONFIG_FCC1_ENET");
	    token_set_y("CONFIG_FCC2_ENET");

	    dev_add("eth0", DEV_IF_MPC82XX_ETH, DEV_IF_NET_EXT);
	    dev_add("eth1", DEV_IF_MPC82XX_ETH, DEV_IF_NET_INT);
	}
	else if (token_get("CONFIG_HW_ETH_EEPRO1000"))
	{
	    token_set_y("CONFIG_E1000");
	    
	    dev_add("eth0", DEV_IF_EEPRO1000, DEV_IF_NET_EXT);
	    dev_add("eth1", DEV_IF_EEPRO1000, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_USB_STORAGE"))
	{		
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_M82XX_HCD");
	    token_set_y("CONFIG_USB_DEVICEFS");
	}

	if (token_get("MODULE_RG_BLUETOOTH"))
	    token_set_y("CONFIG_BT_HCIUSB");

	if (token_get("CONFIG_HW_80211G_ISL38XX"))
	{
	    token_set_m("CONFIG_ISL38XX");
	    dev_add("eth2", DEV_IF_ISL38XX, DEV_IF_NET_INT);
	    dev_can_be_missing("eth2");
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	{
	    ralink_rt2560_add("ra0");
	    token_set("CONFIG_RALINK_RT2560_TIMECSR", "0x40");
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}

	if (token_get("CONFIG_HW_ENCRYPTION"))
	    token_set_y("CONFIG_IPSEC_USE_MPC_CRYPTO");
    }

    if (IS_HW("EP8248"))
    {
	token_set_y("CONFIG_EP8248");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0" );

	token_set("FIRM", "Embedded Planet");
	token_set("BOARD", "EP8248");

	/* Ethernet drivers */

	if (token_get("CONFIG_HW_ETH_FEC"))
	{
	    /* CPM2 Options */

	    token_set_y("CONFIG_FEC_ENET");
	    token_set_y("CONFIG_USE_MDIO");
            token_set_y("CONFIG_FCC_LXT971");
	    token_set_y("CONFIG_FCC1_ENET");
	    token_set_y("CONFIG_FCC2_ENET");

	    dev_add("eth0", DEV_IF_MPC82XX_ETH, DEV_IF_NET_INT);
	    dev_add("eth1", DEV_IF_MPC82XX_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_USB_M82XX_HCD");
	}
	
	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}
    }
    
    if (IS_HW("MPC8349ITX"))
    {
	token_set_y("CONFIG_MPC8349_ITX");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");

	token_set("FIRM", "Freescale");
	token_set("BOARD", "mpc8349-itx");

	/* Ethernet drivers */
	if (token_get("CONFIG_HW_ETH_WAN"))
	{
	    token_set_y("CONFIG_GIANFAR");
	    token_set_y("CONFIG_CICADA_PHY");
	    token_set_y("CONFIG_PHYLIB");
	    
	    dev_add("eth0", DEV_IF_MPC82XX_ETH, DEV_IF_NET_EXT);
	}
	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_y("CONFIG_GIANFAR");
	    token_set_y("CONFIG_CICADA_PHY");
	    token_set_y("CONFIG_PHYLIB");

	    dev_add("eth1", DEV_IF_MPC82XX_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_USB_ETH"))
	{
	    token_set_y("CONFIG_USB_GADGET");
	    token_set_y("CONFIG_USB_ETH");
	    token_set_y("CONFIG_USB_MPC");
	    token_set_y("CONFIG_USB_GADGET_MPC");
	    token_set_y("CONFIG_USB_GADGET_DUALSPEED");
	    token_set_y("CONFIG_USB_ETH_RNDIS");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}
	
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_EHCI_HCD");
	    token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_FSL_USB20");
	    token_set_y("CONFIG_MPH_USB_SUPPORT");
	    token_set_y("CONFIG_MPH0_USB_ENABLE");
	    token_set_y("CONFIG_MPH0_ULPI");
	    if (!token_get("CONFIG_USB_MPC"))
	    {
		token_set_y("CONFIG_MPH1_USB_ENABLE");
		token_set_y("CONFIG_MPH1_ULPI");
	    }
	}

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
	    atheros_ar5xxx_add();
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	{
	    ralink_rt2560_add("ra0");
	    token_set("CONFIG_RALINK_RT2560_TIMECSR", "0x40");
	}
	
	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_80211N_RALINK_RT2860"))
	    ralink_rt2860_add("ra0");

	if (token_get("CONFIG_HW_IDE"))
	{
	    token_set_y("CONFIG_SCSI");
	    token_set_y("CONFIG_SCSI_SATA");
	    token_set_y("CONFIG_SCSI_SATA_SIL");
	}
	if (token_get("CONFIG_HW_SATA"))
	{
	    token_set_y("CONFIG_ATA");
	    token_set_y("CONFIG_SCSI");
	    token_set_y("CONFIG_SCSI_PROC_FS");
	    token_set_y("CONFIG_BLK_DEV_SD");
	    token_set_y("CONFIG_CHR_DEV_SG");
	    /* token_set_y("CONFIG_BLK_DEV_SR"); CDROMs and DVDs */
	    token_set_y("CONFIG_SCSI_MULTI_LUN");
	}

	if (token_get("CONFIG_HW_ENCRYPTION"))
	    token_set_m("CONFIG_MPC8349E_SEC2x");

	/* TODO: Do we need some CONFIG_HW_xxx to enable flash support? */
	/* MTD */
	token_set_y("CONFIG_RG_MTD");
	token_set_y("CONFIG_RG_MTD_DEFAULT_PARTITION");
	token_set_y("CONFIG_MTD");
	token_set("CONFIG_MTD_PHYSMAP_BANKWIDTH", "2");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x800000");
	token_set("CONFIG_MTD_PHYSMAP_START", "0xFE800000");
        token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_SLRAM");
#if 0
	/* TODO: write custom mpc8349-itx MTD driver, as it has 2 8Mb chips.
	 * Currently generic physmap driver is used, only for the second chip */
	/* XXX: It seems that it was already done but not in the generic way.
	 * Need to use the generic function */
	mtd_physmap_add(0xFE800000, 0x800000, 2);
#endif	
	token_set_y("CONFIG_RG_PHY_POLL");
    }

    if (IS_HW("CX82100"))
    {
	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN"))
	    token_set_m("CONFIG_CNXT_EMAC");

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("cnx0", DEV_IF_CX821XX_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("cnx1", DEV_IF_CX821XX_ETH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("cnx1", DEV_IF_CX821XX_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_USB_RNDIS"))
	    token_set_y("CONFIG_USB_RNDIS");

	token_set_y("CONFIG_ARMNOMMU");
	token_set_y("CONFIG_CX821XX_COMMON");
	token_set_y("CONFIG_BD_GOLDENGATE");
	token_set_y("CONFIG_CHIP_CX82100");
	token_set_y("CONFIG_PHY_KS8737");

	token_set("FIRM", "Conexant");
	token_set("BOARD", "CX82100");
    }

    if (IS_HW("BCM91125E") || IS_HW("COLORADO"))
    {
	token_set_y("CONFIG_SIBYTE_SB1250");

	/* Used to be CONFIG_SIBYTE_SWARM */
	if (IS_HW("BCM91125E"))
	    token_set_y("CONFIG_SIBYTE_SENTOSA");
	else if (IS_HW("COLORADO"))
	    token_set_y("CONFIG_SIBYTE_COLORADO");

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("eth1", DEV_IF_BRCM91125E_ETH, DEV_IF_NET_INT);
	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("eth0", DEV_IF_BRCM91125E_ETH, DEV_IF_NET_EXT);
	else if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("eth0", DEV_IF_BRCM91125E_ETH, DEV_IF_NET_INT);

    	/* Flash/MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");

	/* New MTD configs (Linux-2.6) */
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	    token_set_y("CONFIG_MTD_CFI_I1");
	    token_set_y("CONFIG_MTD_CFI_UTIL");
	}

	/* taken from vendor/broadcom/bcm91125e/modules/mtd/bcm91125e_flash_26.c */
        mtd_physmap_add(0x1FC00000, 0x01000000, 1);

    }

    if (IS_HW("CN3XXX"))
    {
	token_set_y("CONFIG_CAVIUM_OCTEON");

	/* Ethernet */
	token_set_m("CONFIG_CAVIUM_ETHERNET");
	token_set_y("CONFIG_MII");
	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("eth1", DEV_IF_CN3XXX_ETH, DEV_IF_NET_INT);
	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("eth0", DEV_IF_CN3XXX_ETH, DEV_IF_NET_EXT);
	else if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("eth0", DEV_IF_CN3XXX_ETH, DEV_IF_NET_INT);

	/* USB */
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_m("CONFIG_USB_DWC_OTG");
	    token_set_y("CONFIG_USB_DEVICEFS");
	}

	/* IPSec */
	if (token_get("CONFIG_HW_ENCRYPTION"))
	{
	    token_set_y("CONFIG_IPSEC_USE_OCTEON_CRYPTO");
	    token_set_m("CONFIG_IPSEC_ALG_JOINT");
	}

    	/* Flash/MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_ADV_OPTIONS");
	token_set_y("CONFIG_MTD_CFI_NOSWAP");
	token_set_y("CONFIG_MTD_CFI_GEOMETRY");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set("CONFIG_MTD_CFI_AMDSTD_RETRY", "0");
	token_set_y("CONFIG_MTD_CFI_UTIL");

	mtd_physmap_add(0x1f400000, 0x800000, 1);
	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL"); /* CramFS on flash */

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set_y("CONFIG_CAVIUM_FASTPATH");
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH", 
		"vendor/cavium/octeon/fastpath");
	}
    }    

    if (IS_HW("RTA770W") || IS_HW("GTWX5803"))
    {
	token_set_y("CONFIG_BCM963XX_COMMON");
	token_set_y("CONFIG_BCM96345");
	token_set_y("CONFIG_BCM963XX_SERIAL");
	token_set_m("CONFIG_BCM963XX_BOARD");
	token_set_y("CONFIG_BCM963XX_RGL_FLASH_LAYOUT");
	token_set("CONFIG_BCM963XX_CHIP", "6345");
	token_set("CONFIG_BCM963XX_SDRAM_SIZE", "16");

	if (token_get("CONFIG_RG_MTD"))
	{
	    token_set_y("CONFIG_RG_MTD_DEFAULT_PARTITION");

	    /* for use with pkg/kernel/linux/mtd */
	    token_set("CONFIG_MTD_PHYSMAP_LEN", "0x00400000");
	    token_set("CONFIG_MTD_PHYSMAP_START", "0x1FC00000");
	}
	else
	{
	    /* 
	    token_set_m("CONFIG_BCM963XX_MTD");
	    token_set_y("CONFIG_MTD_CFI_AMDSTD");
	    */
	    conf_err("TODO: CONFIG_BCM963XX_MTD is no longer used, "
		"use mtd_physmap_add() instead");
	}

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM963XX_ADSL");
	    token_set_m("CONFIG_BCM963XX_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM963XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_BCM963XX_ETH");
	    dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    switch_dev_add("bcm0", NULL, DEV_IF_BCM5325A_HW_SWITCH,
		DEV_IF_NET_INT, 5);
	}

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	{
	    token_set_y("CONFIG_BCM963XX_WLAN");
	    bcm43xx_add("wl0");
	}

	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    token_set_m("CONFIG_BCM963XX_USB");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	if (IS_HW("RTA770W"))
	{
	    token_set("CONFIG_BCM963XX_BOARD_ID", "RTA770W");
	    token_set("BOARD", "RTA770W");
	    token_set("FIRM", "Belkin");
	}
	else
	{
	    token_set("CONFIG_BCM963XX_BOARD_ID", "GTWX5803");
	    token_set("BOARD", "GTWX5803");
	    token_set("FIRM", "Gemtek");
	}
    }

    if (IS_HW("BCM94702"))
    {
	token_set_y("CONFIG_BCM947_COMMON");
	token_set_y("CONFIG_BCM4702");

	/* In order to make an root cramfs based dist use the following 
	 * instead of SIMPLE_RAMDISK
	 *  token_set_y("CONFIG_CRAMFS");
	 *  token_set_y("CONFIG_SIMPLE_CRAMDISK");
	 *  token_set("CONFIG_CMDLINE", 
	 *      "\"root=/dev/mtdblock2 noinitrd console=ttyS0,115200\"");
	 */

	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_ET");
	    token_set_y("CONFIG_ET_47XX");
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("bcm1", DEV_IF_BCM4710_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("bcm0", DEV_IF_BCM4710_ETH, DEV_IF_NET_INT);

	token_set("BOARD", "BCM94702");
	token_set("FIRM", "Broadcom");
    }

    if (IS_HW("BCM94704"))
    {
	token_set_y("CONFIG_BCM947_COMMON");
	token_set_y("CONFIG_BCM4704");

	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_ET");
	    token_set_y("CONFIG_ET_47XX");
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("bcm1", DEV_IF_BCM4710_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("bcm0", DEV_IF_BCM4710_ETH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	    bcm43xx_add("eth0");

	token_set("BOARD", "BCM94704");
	token_set("FIRM", "Broadcom");
    }

    if (IS_HW("BCM94712"))
    {
	/* This means (among others) copy CRAMFS to RAM, which is much
	 * safer, but PMON/CFE currently has a limit of ~3MB when uncompressing.
	 * If your image is larger than that, either reduce image size or
	 * remove CONFIG_COPY_CRAMFS_TO_RAM for this platform. */
	/* TODO: CONFIG_COPY_CRAMFS_TO_RAM does not exist any longer... Check
	 * what should be done here... */
	token_set_y("CONFIG_BCM947_COMMON");
	token_set_y("CONFIG_BCM4710");

	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_ET");
	    token_set_y("CONFIG_ET_47XX");
	    token_set_y("CONFIG_RG_VLAN_8021Q");
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("bcm0.1", DEV_IF_BCM4710_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("bcm0.0", DEV_IF_BCM4710_ETH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	    bcm43xx_add("eth0");

	token_set("BOARD", "BCM94712");
	token_set("FIRM", "Broadcom");
    }

    if (IS_HW("WRT54G"))
    {
	token_set_y("CONFIG_BCM947_COMMON");
	token_set_y("CONFIG_BCM947_HW_BUG_HACK");

	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_m("CONFIG_ET");
	    token_set_y("CONFIG_ET_47XX");
	    token_set_y("CONFIG_RG_VLAN_8021Q");
	    token_set_y("CONFIG_VLAN_8021Q_FAST");
	    dev_add("bcm0", DEV_IF_BCM4710_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("bcm0.1", DEV_IF_VLAN, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("bcm0.2", DEV_IF_VLAN, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	    bcm43xx_add("eth0");

	token_set("BOARD", "Cybertan");
	token_set("FIRM", "Cybertan");
    }
    
    if (IS_HW("CENTROID"))
    {
	set_big_endian(0);

	/* Do not change the order of the devices definition.
	 * Storlink has a bug in their ethernet driver which compells us to `up`
	 * eth0 before eth1
	 */
	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("sl0", DEV_IF_SL2312_ETH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("sl1", DEV_IF_SL2312_ETH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("sl1", DEV_IF_SL2312_ETH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_ETH_LAN") || token_get("CONFIG_HW_ETH_WAN"))
	    token_set_m("CONFIG_SL2312_ETH");

	if (token_get("CONFIG_HW_ETH_LAN2") && token_get("CONFIG_HW_ETH_WAN"))
	    conf_err("Can't define both CONFIG_HW_ETH2 and CONFIG_HW_ETH_WAN");

	if (token_get("CONFIG_HW_80211G_ISL38XX"))
	{
	    token_set_m("CONFIG_ISL38XX");
	    dev_add("eth0", DEV_IF_ISL38XX, DEV_IF_NET_INT);
	    dev_can_be_missing("eth0");
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0");

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_RG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_m("CONFIG_USB_OHCI_SL2312");
	}
	
	token_set_y("CONFIG_ARCH_SL2312"); 
 	token_set_y("CONFIG_SL2312_ASIC"); 
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttySI0");
	token_set("CONFIG_SDRAM_SIZE", "64");

	token_set_m("CONFIG_SL2312_FLASH");
	
	if (!token_get("CONFIG_RG_MTD"))
	    token_set_y("CONFIG_MTD_CFI_AMDSTD");
	
	token_set("FIRM", "Storlink");
	token_set("BOARD", "SL2312");
    }

    if (IS_HW("AD6834"))
    {
	int is_linux26 = !strcmp(os, "LINUX_26");

	token_set_y("CONFIG_MACH_ADI_FUSIV");

	token_set_y("CONFIG_CPU_LX4189");
	token_set_y("CONFIG_ADI_6843");
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_NEW_PCI");
	token_set_y("CONFIG_PCI_AUTO");

	token_set("CONFIG_SDRAM_SIZE", "32");
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");

	token_set_y("CONFIG_ADI_6843_RG1");
	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    if (!is_linux26)
		token_set_y("CONFIG_ETH_NETPRO_SIERRA");
	    else
		token_set_y("CONFIG_ADI_FUSIV_ETHERNET");
	    
	    dev_add("eth0", DEV_IF_AD6834_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_ETH_WAN"))
	{
	    token_set_y("CONFIG_ETH_NETPRO_SIERRA");
	    dev_add("eth1", DEV_IF_AD6834_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    if (!is_linux26) /* linux-2.6 BSP doesn't use zipped ADSL fw */
		token_set_y("CONFIG_ZLIB_INFLATE");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("dsl0", DEV_IF_AD68XX_ADSL, NULL);
	}
	
	token_set("BOARD", "AD6834");
	token_set("FIRM", "Analog Devices");

	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    token_set_m("CONFIG_ADI_6843_USB");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	{
	    ralink_rt2560_add("ra0");
	    token_set("CONFIG_RALINK_RT2560_TIMECSR", "0x40");
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_DSPDRIVER_218X");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	}

	if (!is_linux26)
	    token_set_m("CONFIG_ADI_6843_MTD");
    }
    if (IS_HW("ALASKA") || IS_HW("HIMALAYA2") || IS_HW("CAMEROON") ||
	IS_HW("CAMEROON-IFE6")) 
    {
	token_set("BOARD", hw);
	token_set("FIRM", "Ikanos Communications");
	
	/* General setup */
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");

	/* Machine selection */
	if (IS_HW("ALASKA") || IS_HW("HIMALAYA2"))
	{
	    token_set_y("CONFIG_MACH_ADI_FUSIV");
	    token_set_y("CONFIG_FUSIV_VX160");
	    token_set("CONFIG_SDRAM_SIZE", "32");	
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");
	}
	else if (IS_HW("CAMEROON") || IS_HW("CAMEROON-IFE6"))
	{
	    token_set_y("CONFIG_MACH_IKAN_MIPS");
	    token_set_y("CONFIG_FUSIV_VX180");
            token_set_y("CONFIG_FUSIV_VX180_WRITE_BACK");
	    token_set("CONFIG_SDRAM_SIZE", "128");	
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "32");
	}

	if (IS_HW("ALASKA"))
	    token_set_y("CONFIG_FUSIV_KERNEL_BME_MODULE");

	mtd_physmap_add(
	    0x20000000 - token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024,
	    token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024, 2);
	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");

	/* Ethernet */
	if (token_get("CONFIG_HW_ETH_LAN") || token_get("CONFIG_HW_ETH_LAN2") ||
	    token_get("CONFIG_HW_ETH_WAN"))
	{
	    /* Fusiv drivers */
	    token_set("CONFIG_FUSIV_LIBRARY", "m");
	    token_set("CONFIG_FUSIV_BMDRIVER", "m");
	    token_set("CONFIG_FUSIV_KERNEL_ETHERNET", "m");
	    token_set("CONFIG_FUSIV_TIMERS", "m");
	    
	    if (token_get("CONFIG_HW_ETH_LAN"))
		dev_add("eth0", DEV_IF_AD6834_ETH, DEV_IF_NET_INT);

	    if (token_get("CONFIG_HW_ETH_LAN2"))
	    {
		dev_add("eth1", DEV_IF_AD6834_ETH, DEV_IF_NET_INT);
		dev_add_to_bridge("br0", "eth1");
	    }
	    else if (token_get("CONFIG_HW_ETH_WAN"))
		dev_add("eth1", DEV_IF_AD6834_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("eth1", DEV_IF_AD6834_ETH, DEV_IF_NET_INT);
	
	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH", 
		"vendor/ikanos/fusiv/modules");
	}

	if (token_get("CONFIG_HW_VDSL_WAN"))
	{
	    token_set_y("CONFIG_RG_IKANOS_VDSL");
	    token_set_y("CONFIG_HW_ETH_WAN");

	    if (IS_HW("ALASKA"))
		dev_add("eth1", DEV_IF_IKANOS_VDSL, DEV_IF_NET_EXT);

	    if (IS_HW("CAMEROON"))
	    {
		dev_add("eth2", DEV_IF_IKANOS_VDSL, DEV_IF_NET_EXT);
		token_set_y("CONFIG_FUSIV_VX180_VDSL"); 
		token_set_m("CONFIG_FUSIV_KERNEL_ACL");
		token_set_y("CONFIG_FUSIV_KERNEL_ACL_SPA");
		token_set_y("CONFIG_FUSIV_KERNEL_BME_DRIVER_VX180");
	    }

	    if (IS_HW("CAMEROON-IFE6"))
	    {
		dev_add("eth2", DEV_IF_IKANOS_VDSL, DEV_IF_NET_EXT);
		cameroon_ife6_hw_config();
	    }
	}
	else if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_m("CONFIG_FUSIV_KERNEL_ATM");
	    token_set_y("CONFIG_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    if (IS_HW("CAMEROON-IFE6"))
	    {
		cameroon_ife6_hw_config();
		token_set_y("CONFIG_RG_IKANOS_ADSL");
		dsl_atm_dev_add("dsl0", DEV_IF_IKANOS_ADSL, NULL);
	    }
	    else
		dsl_atm_dev_add("dsl0", DEV_IF_AD68XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	{
	    token_set("CONFIG_FUSIV_PCI_OUTB_ENDIAN_CONVERSION_ENABLE", "0");
	    ralink_rt2560_add("ra0");
	}
	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	{
	    token_set("CONFIG_FUSIV_PCI_OUTB_ENDIAN_CONVERSION_ENABLE", "0");
	    ralink_rt2561_add("ra0");
	}
	if (token_get("CONFIG_HW_80211N_RALINK_RT2860"))
	{
	    token_set("CONFIG_FUSIV_PCI_OUTB_ENDIAN_CONVERSION_ENABLE", "0");
	    ralink_rt2860_add("ra0");
	}

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    if (token_get("CONFIG_RG_PSE_HW"))
		token_set_y("CONFIG_FUSIV_KERNEL_PERI_AP");
	    token_set("CONFIG_FUSIV_PCI_OUTB_ENDIAN_CONVERSION_ENABLE", "1");
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "1");
	    if (token_get("CONFIG_RG_ATHEROS_HW_AR5212"))
		token_set("CONFIG_FUSIV_KERNEL_WLAN_ATH_11G", "1");
	    else
		token_set("CONFIG_FUSIV_KERNEL_WLAN_ATH_11N", "1");
	    token_set_y("CONFIG_ATHEROS_IKANOS_SHADOW_REGISTERS");

	    atheros_ar5xxx_add();
	}

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_FUSIV_KERNEL_DSP");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set_y("CONFIG_RG_DSP_THREAD");
	}
	
	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}

	/* XXX: USB GPIOs and PCI_IDSEL numbers are not verified yet */
	token_set("ADSL_GPIO_RESET", "15");
	token_set("USB_PHY_CONTROL_GPIO", "8");
	token_set("USB_CABLE_SENSE_GPIO", "9");
	token_set("ADSL_LINK_STATE_LED_GPIO", "29");
	token_set("PCI_IDSEL", "20");
    }

    if (IS_HW("BCM93380"))
    {
	token_set("BOARD", "BCM93380");
	token_set("FIRM", "Broadcom");

	token_set_y("CONFIG_BROADCOM_9338X");
	token_set_y("CONFIG_BCM93380");

	token_set("BRCM_CHIP", "3380");
	token_set("BRCM_BOARD_ID", "93380LRG");

	token_set("BRCM_BOARD", "bcm963xx");
	token_set("BRCM_FLASHBLK_SIZE", "64");
	token_set("BRCM_NUM_MAC_ADDRESSES", "11");
	token_set("BRCM_BASE_MAC_ADDRESS", "02:10:18:01:00:01");
	token_set("BRCM_MAIN_TP_NUM", "0");
	token_set("BRCM_PSI_SIZE", "24");

	token_set("CONFIG_SDRAM_SIZE", "64");
    }
    if (IS_HW("BCM93383"))
    {
	token_set("BOARD", "BCM93383");
	token_set("FIRM", "Broadcom");

	token_set_y("CONFIG_BROADCOM_9338X");
	token_set_y("CONFIG_BCM93383");

	token_set("BRCM_CHIP", "3383");
	token_set("BRCM_BOARD_ID", "93383LxG");

	token_set("BRCM_BOARD", "bcm963xx");
	token_set("BRCM_FLASHBLK_SIZE", "64");
	token_set("BRCM_NUM_MAC_ADDRESSES", "11");
	token_set("BRCM_BASE_MAC_ADDRESS", "02:10:18:01:00:01");
	token_set("BRCM_MAIN_TP_NUM", "0");
	token_set("BRCM_PSI_SIZE", "24");

	token_set("CONFIG_SDRAM_SIZE", "64");

	/* These modules are always required for the remote flash to work */
	token_set_m("CONFIG_BCM_IOP");
	token_set_m("CONFIG_BCM_FPM");
	token_set_m("CONFIG_BCM_VENET");

	/* SNMP parameters for communicating with eCos */
	token_set("CONFIG_BCM_SNMP_AGENT", "192.168.1.1");
	token_set("CONFIG_BCM_SNMP_COMMUNITY", "private");

	/* VNIHAL is in use by IP stack, WLAN & LAN virtual ports. */
	token_set_m("CONFIG_BCM_VNIHAL");
	dev_add("cmlan_ipstack0", DEV_IF_BCM338X_VIPSTK, DEV_IF_NET_EXT);
	/* XXX: Temporary until cmlan_ipstack device will be a DHCP client */
	dev_set_static_ip("cmlan_ipstack0", "192.168.1.2", "255.255.255.0");

	if (token_get("CONFIG_HW_ETH_WAN") || token_get("CONFIG_HW_ETH_LAN") ||
	    token_get("CONFIG_HW_80211N_BCM9338X"))
	{
	    if (token_get("CONFIG_HW_ETH_WAN"))
	    {
		dev_add("eth0", DEV_IF_BCM338X_VETH, DEV_IF_NET_EXT);
		dev_set_id("eth0", 2);
	    }
	    if (token_get("CONFIG_HW_ETH_LAN"))
	    {
		dev_add("ethsw0", DEV_IF_BCM338X_VETHSW, DEV_IF_NET_INT);
		dev_set_static_ip("ethsw0", "192.168.0.1", "255.255.255.0");
	    }
	    if (token_get("CONFIG_HW_80211N_BCM338X"))
	    {
		dev_add("wlan0", DEV_IF_BCM338X_VWLAN_PHY, 0);
		dev_add("wl0", DEV_IF_BCM338X_VWLAN, DEV_IF_NET_INT);
		dev_set_dependency("wl0", "cmlan_ipstack0");
		dev_set_id("wl0", 12);
		dev_add("wl1", DEV_IF_BCM338X_VWLAN, DEV_IF_NET_INT);
		dev_set_dependency("wl1", "cmlan_ipstack0");
		dev_set_id("wl1", 22);
	    }
	}
    }
    if (IS_HW("BCM96362"))
    {
        token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");

	if (IS_HW("BCM96362"))
	{
	    token_set("BOARD", "BCM96362");
	    token_set_y("CONFIG_BCM96362");
	    token_set("BRCM_CHIP", "6362");
	    token_set("BRCM_BOARD_ID", "96362ADVNgr");
	    token_set("BRCM_VOICE_BOARD_ID", "SI3217X");
	}
	token_set("FIRM", "Broadcom");

	token_set_y("CONFIG_BROADCOM_9636X");

	token_set("BRCM_BOARD", "bcm963xx");
	token_set("BRCM_FLASHBLK_SIZE", "64");
	token_set("BRCM_NUM_MAC_ADDRESSES", "11");
	token_set("BRCM_BASE_MAC_ADDRESS", "02:10:18:01:00:01");
	token_set("BRCM_MAIN_TP_NUM", "0");
	token_set("BRCM_PSI_SIZE", "24");

	if (IS_HW("BCM96362"))
	{
	    token_set_y("CONFIG_HW_BCM96362");
	    if (!token_get("CONFIG_SDRAM_SIZE"))
		token_set("CONFIG_SDRAM_SIZE", "256");
	    token_set("CONFIG_HW_FLASH_NOR", "n");
	    token_set_y("CONFIG_HW_FLASH_NAND");
	}

	if (!token_get("CONFIG_HW_FLASH_NOR") && 
	    token_get("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH"))
	{
	    rg_error(LEXIT, "CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH is not"
	        " possible on this board.\n");
	}

	if (!token_get("CONFIG_LSP_DIST"))
	{
	    if (token_get("CONFIG_HW_ETH_LAN"))
		dev_add("bcmsw", DEV_IF_BCM9636X_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_ETH_LAN") || token_get("CONFIG_HW_ETH_WAN"))
	    token_set_m("CONFIG_BCM_ENET");

        if (token_get("CONFIG_RG_3G_WAN"))
            gsm_dev_add("gsm0", DEV_IF_3G_USB_SERIAL);

	if (token_get("CONFIG_HW_FLASH_NAND"))
	{
	    /* Tightly coupled with CONFIG_SDRAM_SIZE.
	     * RGloader needs private mem to place the openrg image prior 
	     * booting; it is located right after the memory seen by the 
	     * rgloader kernel. Must also defined at the openrg dist (the 
	     * openrg image opener executabe should be linked to this address).
	     */
	    token_set("CONFIG_RGLOADER_RESERVED_RAM_START", "0x02000000");
	}

	if (token_get("CONFIG_HW_USB_STORAGE"))
        {
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
        }

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_BCM_BCMDSP");
	    token_set_m("CONFIG_BCM_ENDPOINT");
	    if (IS_HW("BCM96362"))
	    {
		token_set_y("BRCM_6362_UNI");
		token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    }
	}

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM9636X_ADSL, NULL);
	}

	if (token_get("CONFIG_RG_HW_WATCHDOG"))
	{
	    token_set_m("CONFIG_RG_BCM9636X_WATCHDOG");
	    token_set_y("CONFIG_RG_HW_WATCHDOG_ARCH_LINUX_GENERIC");
	}
    }

    if (IS_HW("BCM96358") || IS_HW("DWV_BCM96358") || IS_HW("HH1620") ||
	IS_HW("CT6382D"))
    {
	long start;
	token_set_y("CONFIG_BCM963XX_COMMON");
	token_set_y("CONFIG_BCM96358");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_BCM963XX_SERIAL");
	token_set_y("CONFIG_BCM96358_BOARD");
	token_set_y("BOOTLOADER_ADDR_SPACE_IS_VIRTUAL");

	if (IS_HW("BCM96358"))
	    token_set("CONFIG_BCM963XX_BOARD_ID", "96358GW");
	else if (IS_HW("DWV_BCM96358"))
	{
	    token_set_y("CONFIG_HW_DWV_96358");
	    token_set("CONFIG_BCM963XX_BOARD_ID", "DWV-S0");
	}
	else if (IS_HW("HH1620"))
	{
	    token_set_y("CONFIG_HW_HH1620");
	    token_set("CONFIG_BCM963XX_BOARD_ID", "96358VW");
	}
	else if (IS_HW("CT6382D"))
	{
	    token_set_y("CONFIG_HW_CT6382D");
	    token_set("CONFIG_BCM963XX_BOARD_ID", "CT6382D");
	}

	token_set("CONFIG_BCM963XX_SDRAM_SIZE", "32");
	if (IS_HW("DWV_BCM96358") || IS_HW("HH1620") || IS_HW("CT6382D"))
	{
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    /* this value is taken from 
	     * vendor/broadcom/bcm963xx/linux-2.6/bcmdrivers/opensource/include/bcm963xx/board.h*/
	    start = 0x1e000000; /* =CPHYSADDR(CONFIG_MTD_PHYSMAP_START) */
	}
	else
	{
	    /* this value is taken from 
	     * vendor/broadcom/bcm963xx/linux-2.6/bcmdrivers/opensource/include/bcm963xx/board.h*/
	    token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "4");
	    start = 0x1fc00000; /* =CPHYSADDR(CONFIG_MTD_PHYSMAP_START) */
	}

	mtd_physmap_add(start, 
	    token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024, 2);

	token_set("CONFIG_BCM963XX_CHIP", "6358");

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM963XX_ADSL");
	    token_set_m("CONFIG_BCM963XX_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bcm_atm0", DEV_IF_BCM963XX_ADSL, NULL);
	}

	if (token_get("CONFIG_HW_ETH_WAN") &&
	    !token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	{
	    if (token_get("CONFIG_HW_ETH_LAN"))
		conf_err("Ethernet device 'bcm0' cannot be WAN and LAN device");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    dev_add("bcm0", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);

	    if (!token_get("CONFIG_HW_SWITCH_LAN"))
	    {
		token_set_m("CONFIG_BCM963XX_ETH");
		switch_dev_add("bcm1", NULL, DEV_IF_BCM963XX_ETH,
		    DEV_IF_NET_INT, -1);
	    }
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	    {
		dev_add("bcm1", DEV_IF_BCM963XX_ETH, DEV_IF_NET_INT);
		switch_dev_add("bcm1_main", "bcm1", DEV_IF_BCM5325E_HW_SWITCH,
		    DEV_IF_NET_INT, 5);
	    }
	    else
	    {
		switch_dev_add("bcm1", NULL, DEV_IF_BCM5325E_HW_SWITCH,
		    DEV_IF_NET_INT, 5);
	    }
	}

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	{
	    token_set_y("CONFIG_BCM4318");
	    bcm43xx_add("wl0");
	}

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
	    atheros_ar5xxx_add();
	}

        if (token_get("CONFIG_RG_3G_WAN"))
            gsm_dev_add("gsm0", DEV_IF_3G_USB_SERIAL);

	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    token_set_m("CONFIG_BCM963XX_USB");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_BCM963XX_DSP");
	    token_set_m("CONFIG_BCM_ENDPOINT");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set("CONFIG_BCM963XX_SLICTYPE", IS_HW("CT6382D") ? "LE88221" :
		"SI3215");
	}
	if (token_get("CONFIG_HW_FXO"))
	{
	    token_set_y("CONFIG_BCM963XX_FXO");
	    token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
	}

	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_NEW_PCI");
	token_set_y("CONFIG_PCI_AUTO");
	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	if (IS_HW("HH1620"))
	{
	    token_set("BOARD", "HH1620");
	    token_set("FIRM", "Motorola/Netopia");
	}
	else
	{
	    token_set("BOARD", "BCM96358");
	    token_set("FIRM", "Broadcom");
	}

    }
	
    if (IS_HW("CX8620XR") || IS_HW("CX8620XD"))
    {
	if (IS_HW("CX8620XR"))
	{
	    token_set("CONFIG_CX8620X_SDRAM_SIZE", "8");
	    token_set_y("CONFIG_RG_BOOTSTRAP");
	    
	    /* Flash chip */
	    token_set_m("CONFIG_CX8620X_FLASH_TE28F160C3");
	    token_set("BOARD", "CX8620XR");
	}
	else
	{
	    token_set("CONFIG_CX8620X_SDRAM_SIZE", "64");
	
	    if (token_get("CONFIG_LSP_DIST"))
		token_set_y("CONFIG_RG_BOOTSTRAP");

	    /* Flash chip */
	    if (!token_get("CONFIG_CX8620X_FLASH_TE28F320C3"))
		token_set_m("CONFIG_CX8620X_FLASH_TE28F640C3");
	    token_set("BOARD", "CX8620XD");
	}
	    
	token_set_m("CONFIG_CX8620X_SWITCH");

	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("cnx1", DEV_IF_CX8620X_SWITCH, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("cnx0", DEV_IF_CX8620X_SWITCH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_ETH_LAN2"))
	    dev_add("cnx1", DEV_IF_CX8620X_SWITCH, DEV_IF_NET_INT);

	if (token_get("CONFIG_HW_USB_HOST_EHCI"))
	    token_set_y("CONFIG_CX8620X_EHCI");
	
	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}

	if (token_get("CONFIG_HW_80211G_ISL_SOFTMAC"))
	    isl_softmac_add();
	
	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0");

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	token_set("FIRM", "Conexant");
    }
    if (IS_HW("CX9451X"))
    {
	set_big_endian(0);
	token_set("ARCH", "arm");
	token_set("CONFIG_ARCH_MACHINE", "solos");
	token_set_y("CONFIG_HAS_MMU");
	token_set("CONFIG_CMDLINE", "console=ttyS0,38400");
	token_set_y("CONFIG_DEBUG_USER");
	token_set_y("CONFIG_KALLSYMS");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_SOLOS_CONSOLE");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set_y("CONFIG_RG_ARCHINIT");

#if 0
	/* Conexant does not use this ethernet driver. If we try to set it, it
	 * block the kernel
	 */
	token_set_y("CONFIG_MII");
	token_set_y("CONFIG_ARM_SOLOS_ETHER");
#endif
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set("CONFIG_ZBOOT_ROM_TEXT", "0");
	token_set_y("CONFIG_CONEXANT_COMMON");
	token_set_y("CONFIG_CX9451X_COMMON");
	token_set_y("CONFIG_CPU_32");
	/* XXX Remove CONFIG_LOCK_KERNEL after resolving B36439 */
	token_set_y("CONFIG_LOCK_KERNEL");
	token_set_y("CONFIG_FPE_NWFPE");
	token_set_y("CONFIG_ALIGNMENT_TRAP");
	token_set_y("CONFIG_PREVENT_FIRMWARE_BUILD");
	token_set_y("CONFIG_ARCH_SOLOS_GALLUS");
	token_set_y("CONFIG_ARCH_SOLOS_376PIN");
	token_set_y("CONFIG_MACH_SOLOS_GALLUSBU");
	token_set_y("CONFIG_CPU_ARM1026");
	token_set_y("CONFIG_CPU_32v5");
	token_set_y("CONFIG_CPU_ABRT_EV5T");
	token_set_y("CONFIG_CPU_CACHE_VIVT");
	token_set_y("CONFIG_CPU_COPY_V4WB");
	token_set_y("CONFIG_CPU_TLB_V4WBI");
	token_set_y("CONFIG_FW_LOADER");
	token_set_y("CONFIG_STANDALONE");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_UID16");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_SOLOS");
	token_set_y("CONFIG_SERIAL_SOLOS_CONSOLE");
	token_set("CONFIG_SERIAL_SOLOS_CONSOLE_BAUD", "38400");
	token_set("LIBC_ARCH", "arm");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");

	/* MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_CHAR");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_MTD_CFI_UTIL");

	mtd_physmap_add(0x38000000, 0x1000000, 1);

	token_set_y("CONFIG_OBSOLETE_INTERMODULE");
	
	if (!token_get("CONFIG_LSP_DIST"))
	{
	    dev_add("lan0", DEV_IF_SOLOS_LAN, DEV_IF_NET_INT);
	    if (token_get("CONFIG_HW_ETH_WAN"))
		dev_add("dmz0", DEV_IF_SOLOS_DMZ, DEV_IF_NET_EXT);
	}
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_USB_EHCI_HCD");
	    token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
	    token_set_y("CONFIG_USB_SOLOS_HCD");
	    token_set_y("CONFIG_SOLOS_USB_HOST0");
	    token_set_y("CONFIG_SOLOS_USB_HOST1");
	}

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_CX9451X_DSP");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	}

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    dsl_atm_dev_add("bunatm", DEV_IF_SOLOS_DSL, NULL);
	}
    }

    if (IS_HW("COMCERTO"))
    {
	if (token_get("CONFIG_COMCERTO_COMMON"))
	{
	    /* Network interfaces */
	    if (token_get("CONFIG_HW_ETH_LAN"))
		dev_add("eth0", DEV_IF_COMCERTO_ETH, DEV_IF_NET_INT);
	    if (token_get("CONFIG_HW_ETH_WAN"))
		dev_add("eth2", DEV_IF_COMCERTO_ETH, DEV_IF_NET_EXT);

	    /* VED */
	    token_set_y("CONFIG_COMCERTO_VED");
	    token_set_y("CONFIG_MII");
	    dev_add("eth1", DEV_IF_COMCERTO_ETH_VED, DEV_IF_NET_INT);

	    token_set_y("CONFIG_RG_ARCHINIT");
	}

	/* CONFIG_MTD_PHYSMAP_START taken from NORFLASH_MEMORY_PHY1 in
	 * vendor/mindspeed/comcerto/kernel/arch-comcerto/platform.h
	 */
	mtd_physmap_add(0x80000000, 8 * 1024 * 1024, 2);

	/* Ralink WiFi card */
	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0");
	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	token_set_y("CONFIG_NETFILTER");

	token_set_y("CONFIG_COMCERTO_MATISSE");

	if (token_get("CONFIG_COMCERTO_MALINDI"))
	{
	    token_set_y("CONFIG_EVM_MALINDI");
	    token_set("CONFIG_LOCALVERSION","malindi");
	    token_set("BOARD", "Malindi");
	    if (token_get("CONFIG_HW_DSP"))
	    {
		token_set_y("CONFIG_COMCERTO_CAL");
		token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "4");
	    }
	}
	else if (token_get("CONFIG_COMCERTO_NAIROBI"))
	{
	    token_set_y("CONFIG_EVM_SUPERMOMBASA");
	    token_set("CONFIG_LOCALVERSION","nairobi");
	    token_set("BOARD", "Nairobi");
	    if (token_get("CONFIG_HW_DSP"))
	    {
		token_set_y("CONFIG_COMCERTO_CAL");
		token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    }
	}

	token_set("FIRM", "Mindspeed");
    }
    if (IS_HW("BB-ROUTER") || IS_HW("FE-ROUTER") || 
	IS_HW("PACKET-IAD"))
    {

	if (token_get("CONFIG_HW_ETH_LAN")) 
	    dev_add("eth2", DEV_IF_COMCERTO_ETH, DEV_IF_NET_INT); 
	if (token_get("CONFIG_HW_ETH_WAN")) 
	    dev_add("eth0", DEV_IF_COMCERTO_ETH, DEV_IF_NET_EXT); 
	if (token_get("CONFIG_COMCERTO_VED_M821"))
	    dev_add("eth1", DEV_IF_COMCERTO_ETH_VED, DEV_IF_NET_INT);

	token_set_y("CONFIG_RG_ARCHINIT");
	token_set_y("CONFIG_NETFILTER");
	token_set("FIRM", "Mindspeed");

	token_set_y("CONFIG_CPU_CP15");
	token_set_y("CONFIG_CPU_CP15_MMU");

	if (IS_HW("PACKET-IAD"))
	{
	    token_set_y("CONFIG_COMCERTO_PACKET_IAD");
	    token_set_y("CONFIG_EVM_PACKET_IAD");
	    token_set("CONFIG_LOCALVERSION","Packet-IAD");
	    token_set("BOARD", "Packet-iad");
	    token_set("CONFIG_SDRAM_SIZE", "120");
	    if (token_get("CONFIG_HW_DSP"))
	    {
		if (token_get("CONFIG_RG_PSE_HW"))
		    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
		else
		{
		    token_set_y("CONFIG_HW_FXO"); 
		    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "4");
		    token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
		}
	    }
	}
	else if (IS_HW("BB-ROUTER"))
	{
	    token_set_y("CONFIG_COMCERTO_BB_ROUTER");
	    token_set_y("CONFIG_EVM_ROUTER");
	    token_set("CONFIG_LOCALVERSION", "Giga-Router");
	    token_set("BOARD", "Giga-Router"); 
	    token_set("CONFIG_SDRAM_SIZE", "56");
	    if (token_get("CONFIG_HW_DSP"))
	    {
		if (token_get("CONFIG_RG_PSE_HW"))
		    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
		else
		{
		    token_set_y("CONFIG_HW_FXO"); 
		    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "4");
		    token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
		}
	    }
	    /* CONFIG_MTD_PHYSMAP_START taken from NORFLASH_MEMORY_PHY1 in
	     * vendor/mindspeed/comcerto-100/kernel/arch-comcerto/boards/
	     * board-ferouter.h
	     */
	    mtd_physmap_add(0x20000000, 16 * 1024 * 1024, 2);
	}
	else if (IS_HW("FE-ROUTER"))
	{
	    token_set_y("CONFIG_COMCERTO_FE_ROUTER");
	    token_set_y("CONFIG_EVM_FEROUTER");
	    token_set("CONFIG_LOCALVERSION", "FE-Router");
	    token_set("BOARD", "FE-Router");
	    token_set("CONFIG_SDRAM_SIZE", "56");
	    if (token_get("CONFIG_RG_PSE_HW"))
		token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    else
	    {
		token_set_y("CONFIG_HW_FXO"); 
		token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
		token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");   
	    }

	    /* CONFIG_MTD_PHYSMAP_START taken from NORFLASH_MEMORY_PHY1 in
	     * vendor/mindspeed/comcerto-100/kernel/arch-comcerto/boards/
	     * board-ferouter.h
	     */
	    mtd_physmap_add(0x20000000, 16 * 1024 * 1024, 2);
	}

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH",
		"vendor/mindspeed/comcerto-100/modules/fastpath");

	    token_set_m("CONFIG_COMCERTO_FPP");
	    token_set_y("CONFIG_COMCERTO_FPP_LOADER");
	    token_set("FPP_CODE_LOCATION", "/lib/modules/fpp.axf");
	    token_set("FPP_NODE_LOCATION", "/dev/fpp");
	    token_set_y("CONFIG_COMCERTO_FPP_TX_ZERO_COPY");
	    token_set_y("CONFIG_COMCERTO_FPP_RX_ZERO_COPY");
	}
	if (token_get("CONFIG_HW_ENCRYPTION"))
 	{
	    token_set_y("CONFIG_ELLIPTIC_IPSEC");
	    token_set_m("CONFIG_IPSEC_ALG_JOINT");
	    token_set_y("CONFIG_IPSEC_ALG_JOINT_ASYNC");
 	}

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_SPI_LEGERITY");
	    token_set_y("CONFIG_COMCERTO_MATISSE");
	    token_set_y("CONFIG_COMCERTO_CAL");
	}

	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}

	/* Atheros WiFi card */ 
	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
	    atheros_ar5xxx_add();
	}

	/* Ralink WiFi card */ 
 	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0"); 
 
	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))	   	
	    ralink_rt2561_add("ra0"); 

	if (token_get("CONFIG_HW_80211N_RALINK_RT2860"))	   	
	    ralink_rt2860_add("ra0"); 
    }

    if (IS_HW("C1KMFCNEVM"))
    {
	set_big_endian(0);
    	token_set_y("CONFIG_RG_HW_C1KMFCNEVM");
	token_set_y("CONFIG_EVM_C1KMFCN_EVM");
	token_set_y("CONFIG_BOOTLDR_UBOOT");
	/* For arm platforms we use kernel (gzip) compression */
	token_set("CONFIG_BOOTLDR_UBOOT_COMP", "none");

	token_set("FIRM", "Mindspeed");
	token_set("BOARD", "C1KMFCNEVM");
	token_set("CONFIG_SDRAM_SIZE", "256");
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "32"); 
	token_set_y("CONFIG_COMCERTO_COMMON_83XXX");
	token_set_y("CONFIG_COMCERTO_UART0_SUPPORT");

	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_COMCERTO_NOR");
	mtd_physmap_add(0x20000000,
	    token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024, 0);

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_COMCERTO_CAL");
	    token_set_y("CONFIG_COMCERTO_TDM_CLOCK");
	    token_set_y("CONFIG_SPI_SI3050");
	    token_set_y("CONFIG_SPI_SI32267");
	    token_set_y("CONFIG_COMCERTO_MATISSE");
	    token_set_y("CONFIG_COMCERTO_VED");
	    token_set_y("CONFIG_RG_ARCHINIT");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set_y("CONFIG_RG_DSP_THREAD");
	}

	if (token_get("CONFIG_HW_ETH_LAN") || token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_COMCERTO_SPI");
	    token_set_y("CONFIG_COMCERTO_SPI_SUPPORT");
	    token_set_y("CONFIG_SPI_MSPD");
	}

	if (token_get("CONFIG_HW_ETH_WAN") ||
	    token_get("CONFIG_HW_ETH_LAN") ||
	    token_get("CONFIG_HW_SWITCH_LAN") ||
	    token_get("CONFIG_COMCERTO_VED"))
	{
	    token_set_y("CONFIG_COMCERTO_ETH");
	    token_set_y("CONFIG_PHYLIB");
	    token_set_y("CONFIG_FPE_NWFPE");
	    token_set_y("CONFIG_MII");
	}

	/*USB*/
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_COMCERTO_USB0_SUPPORT");
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_USB_EHCI_HCD");
	    token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
	    token_set_y("CONFIG_USB_EHCI_SPLIT_ISO");
	    token_set_y("CONFIG_USB_EHCI_TT_NEWSCHED");
	}

	if (token_get("CONFIG_HW_ETH_WAN")) 
	    dev_add("eth0", DEV_IF_C1000_ETH, DEV_IF_NET_EXT);
	if (token_get("CONFIG_HW_ETH_LAN"))
	    dev_add("eth2", DEV_IF_BCM53115M_ETH, DEV_IF_NET_INT);
	if (token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
	    atheros_ar5xxx_add();
	}

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set_m("CONFIG_COMCERTO_FPP");
	    token_set_y("CONFIG_FW_LOADER");
	    token_set_y("CONFIG_SYSFS_DEPRECATED");

	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH",
		"vendor/mindspeed/comcerto-1000/modules/fastpath");
	}
    }

    if (IS_HW("CENTAUR"))
    {
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "4"); 
	token_set("CONFIG_SDRAM_SIZE", "32");	
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");

	if (token_get("CONFIG_HW_ETH_WAN"))
	{
	    token_set_y("CONFIG_KS8695");
	    dev_add("eth0", DEV_IF_KS8695_ETH, DEV_IF_NET_EXT);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_y("CONFIG_KS8695");
	    dev_add("eth1", DEV_IF_KS8695_ETH, DEV_IF_NET_INT);
	}
	
	if (token_get("CONFIG_HW_80211G_ISL38XX"))
	{
	    token_set_m("CONFIG_ISL38XX");
	    dev_add("eth2", DEV_IF_ISL38XX, DEV_IF_NET_INT);
	    dev_can_be_missing("eth2");
	}

	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0");

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_ZSP400");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "4");
	}

	token_set("FIRM", "Micrel");
	token_set("BOARD", "CENTAUR");

	/* Flash chip XXX Should be module */
	token_set_y("CONFIG_KS8695_FLASH_AM29LV033C");

	mtd_physmap_add(0x2800000, 0x400000, 1);
    }

    if (IS_HW("UML"))
    {
	token_set("ARCH", "um");

	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "64"); 
	if (token_get("CONFIG_HW_ETH_WAN"))
	    dev_add("eth0", DEV_IF_UML, DEV_IF_NET_EXT);
	if (token_get("CONFIG_HW_ETH_WAN2"))
	    dev_add("eth3", DEV_IF_UML, DEV_IF_NET_EXT);

	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM_NULL");
	    dsl_atm_dev_add("atmnull0", DEV_IF_ATM_NULL, NULL);
	}

	if (token_get("CONFIG_HW_HSS_WAN"))
	    token_set_y("CONFIG_RG_HSS");

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    dev_add_uml_hw_switch("hw_switch0", DEV_IF_NET_INT, -1, "eth4",
		"eth5", "eth6", "eth7", NULL);
	    /* Yep, a bridge under the bridge... */
	    dev_add_to_bridge("br0", "hw_switch0");
	}

	if (token_get("CONFIG_HW_ETH_LAN2"))
	{
	    dev_add("eth2", DEV_IF_UML, DEV_IF_NET_INT);
	    if (token_get("CONFIG_DEF_BRIDGE_LANS"))
		dev_add_to_bridge("br0", "eth2");
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    dev_add("eth1", DEV_IF_UML, DEV_IF_NET_INT);
	    if (token_get("CONFIG_DEF_BRIDGE_LANS"))
		dev_add_to_bridge("br0", "eth1");
	}

	if (token_get("CONFIG_HW_80211G_UML_WLAN"))
	{
	    wireless_vaps_add("uml_wlan0", DEV_IF_UML_WLAN, "uml_wlan%d", 8,
		NULL);
	}

	if (token_get("CONFIG_RG_TUTORIAL_ADVANCED"))
	{
	    dev_add("mydev0", DEV_IF_MYDEV, DEV_IF_NET_INT);
	    dev_can_be_missing("mydev0");
	    dev_add_to_bridge("br0", "mydev0");
	}

	if (token_get("CONFIG_UML_ROUTE_GROUPS"))
	{
	    dev_add("eth8", DEV_IF_UML, DEV_IF_NET_EXT); 
	    dev_add("eth9", DEV_IF_UML, DEV_IF_NET_EXT); 
	    dev_add("eth10", DEV_IF_UML, DEV_IF_NET_INT);
	    dev_add("eth11", DEV_IF_UML, DEV_IF_NET_INT);
	    dev_add("eth12", DEV_IF_UML, DEV_IF_NET_INT);
	    dev_add("eth13", DEV_IF_UML, DEV_IF_NET_INT);
	    dev_add("eth14", DEV_IF_UML, DEV_IF_NET_INT);
	    dev_add("eth15", DEV_IF_UML, DEV_IF_NET_EXT);
	    dev_add("eth16", DEV_IF_UML, DEV_IF_NET_EXT);
	    dev_add("eth17", DEV_IF_UML, DEV_IF_NET_EXT);
	    dev_add("eth18", DEV_IF_UML, DEV_IF_NET_EXT);

	    /* Setting three routing groups non-forwarding, demo1, demo2 */
	    dev_set_route_group("eth8", DEV_ROUTE_GROUP_NON_FORWARDING);
	    dev_set_route_group("eth9", DEV_ROUTE_GROUP_NON_FORWARDING);
	    dev_set_route_group("eth12", DEV_ROUTE_GROUP_DEMO1);
	    dev_set_route_group("eth13", DEV_ROUTE_GROUP_DEMO1);
	    dev_set_route_group("eth17", DEV_ROUTE_GROUP_DEMO1);
	    dev_set_route_group("eth14", DEV_ROUTE_GROUP_DEMO2);
	    dev_set_route_group("eth18", DEV_ROUTE_GROUP_DEMO2);
	}

	token_set_y("CONFIG_RG_UML");

	token_set_y("CONFIG_DEBUGSYM"); /* UML is always for debug ;-) */
	
	token_set("CONFIG_RG_CONSOLE_DEVICE", "console");

	token_set_y("CONFIG_RAMFS");
	token_set("CONFIG_KERNEL_STACK_ORDER", "2");
	token_set_y("CONFIG_MODE_SKAS");
	token_set("CONFIG_NEST_LEVEL", "0");
	token_set("CONFIG_CON_ZERO_CHAN", "fd:0,fd:1");
	token_set("CONFIG_CON_CHAN", "xterm");
	token_set("CONFIG_SSL_CHAN", "pty");
	token_set_y("CONFIG_PT_PROXY");
	token_set_y("CONFIG_STDIO_CONSOLE");
	token_set_y("CONFIG_SSL");
	token_set_y("CONFIG_FD_CHAN");
	token_set_y("CONFIG_NULL_CHAN");
	token_set_y("CONFIG_PORT_CHAN");
	token_set_y("CONFIG_PTY_CHAN");
	token_set_y("CONFIG_TTY_CHAN");
	token_set_y("CONFIG_XTERM_CHAN");
	token_set_y("CONFIG_BLK_DEV_NBD");
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_RG_INITFS_RAMFS");
	    token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	    token_set_y("CONFIG_RG_MODFS_CRAMFS");
	    token_set_y("CONFIG_UML_X86");

	    token_set_y("CONFIG_FLATMEM");
	}
	else
	{
	    token_set("CONFIG_KERNEL_HALF_GIGS", "1");
	    token_set_y("CONFIG_BLK_DEV_UBD");
	    token_set_y("CONFIG_SIMPLE_RAMDISK");
	    token_set("CONFIG_BLK_DEV_RAM_SIZE", "8192");
	    token_set_y("CONFIG_RG_INITFS_RAMDISK");
	}
	token_set_y("CONFIG_UML_NET");
	token_set_y("CONFIG_UML_NET_ETHERTAP");
	token_set_y("CONFIG_UML_NET_TUNTAP");
	token_set_y("CONFIG_UML_NET_SLIP");
	token_set_y("CONFIG_UML_NET_SLIRP");
	token_set_y("CONFIG_UML_NET_DAEMON");
	token_set_y("CONFIG_UML_NET_MCAST");
	token_set_y("CONFIG_DUMMY");
	token_set_y("CONFIG_TUN");
	token_set_y("CONFIG_KALLSYMS");

	token_set("CONFIG_UML_RAM_SIZE",
	    token_get("CONFIG_VALGRIND") ? "192M" : "64M");

	token_set_y("CONFIG_USERMODE");
	token_set_y("CONFIG_UID16");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_BSD_PROCESS_ACCT");
	token_set_y("CONFIG_HOSTFS");
	token_set_y("CONFIG_HPPFS");
	token_set_y("CONFIG_MCONSOLE");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	if (!token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set_y("CONFIG_PROC_MM");

	token_set_y("CONFIG_PACKET_MMAP");
	token_set_y("CONFIG_QUOTA");
	token_set_y("CONFIG_AUTOFS_FS");
	token_set_y("CONFIG_AUTOFS4_FS");
	token_set_y("CONFIG_REISERFS_FS");

	token_set_y("CONFIG_MTD_BLKMTD");
	token_set_y("CONFIG_ZLIB_INFLATE");
	token_set_y("CONFIG_ZLIB_DEFLATE");

	token_set_y("CONFIG_PT_PROXY");
	token_set("CONFIG_RG_KERNEL_COMP_METHOD", "gzip");
	token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "lzma");
	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_y("CONFIG_HW_FXO");
	    token_set_y("CONFIG_RG_VOIP_HW_EMULATION");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "8");
	    token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
	}
    }
    
    if (IS_HW("JPKG"))
    {
	if (token_get("CONFIG_HW_80211G_RALINK_RT2560"))
	    ralink_rt2560_add("ra0");

	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	if (token_get("CONFIG_HW_80211N_RALINK_RT2860"))
	    ralink_rt2860_add("ra0");

	if (token_get("CONFIG_HW_80211G_BCM43XX"))
	    bcm43xx_add(NULL);

	if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	    token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    atheros_ar5xxx_add();
	}

	if (token_get("CONFIG_HW_80211N_AIRGO_AGN100"))
	    airgo_agn100_add();
    }
    
    if (IS_HW("DB88F6560BP") || IS_HW("MI424WR-FEROCEON"))
    {
	token_set("ARCH", "arm");
	token_set("LIBC_ARCH", "arm");

	token_set_y("CONFIG_ARCH_FEROCEON_KW2");
	token_set_y("CONFIG_FEROCEON_COMMON");

	token_set_y("CONFIG_MV_INCLUDE_PEX");
	token_set_y("CONFIG_MV_INCLUDE_TDM");
	token_set_y("CONFIG_MV_INCLUDE_SPI");
	token_set("CONFIG_MV_GPP_MAX_PINS", "70");
	token_set("CONFIG_MV_DCACHE_SIZE", "0x4000");
	token_set("CONFIG_MV_ICACHE_SIZE", "0x4000");
	token_set_y("CONFIG_L2_CACHE_ENABLE");
	token_set_y("CONFIG_MV_SP_I_FTCH_DB_INV");
	token_set_y("CONFIG_MII");

	/* Ethernet drivers */
	if (token_get("CONFIG_HW_ETH_WAN"))
        {
            dev_add("eth1", DEV_IF_MV88F65XX_ETH, DEV_IF_NET_EXT);
            token_set_y("CONFIG_MV_ETHERNET");
            token_set_y("CONFIG_MV_ETH_NETA");
            token_set("CONFIG_MV_ETH_PORTS_NUM", "3");
            token_set_y("CONFIG_MV_INCLUDE_GIG_ETH");
        }

        if (token_get("CONFIG_HW_ETH_LAN"))
        {
	    switch_dev_add("eth0", NULL, DEV_IF_FEROCEON_HW_SWITCH,
		DEV_IF_NET_INT, 4);
#if 0
            token_set_y("CONFIG_MV_ETH_SWITCH");
            token_set("CONFIG_MV_ETH_SWITCH_NETDEV_NUM", "1");
            token_set("CONFIG_MV_ETH_SWITCH_NETCONFIG",
                "\"1,(00:22:77:22:77:22,0:1:2:3),mtu=1500\"");
            token_set_y("CONFIG_MV_ETH_SWITCH_LINK");
            token_set_y("CONFIG_MV_INCLUDE_SWITCH");
#endif
        }

	if (token_get("CONFIG_HW_PON_WAN"))
	{
	    dev_add("pon0", DEV_IF_MV88F65XX_PON, DEV_IF_NET_EXT);
            token_set_y("CONFIG_PON");
	    token_set_y("CONFIG_MV_INCLUDE_PON");
	    token_set_y("CONFIG_MV_INCLUDE_CUST");
            token_set_y("CONFIG_MV_INCLUDE_TPM");
	}

	token_set_y("CONFIG_BOOTLDR_UBOOT");
	token_set("CONFIG_BOOTLDR_UBOOT_COMP", "none");

	token_set("CONFIG_HW_FLASH_NOR", "n");
	token_set_y("CONFIG_HW_FLASH_NAND");
	token_set("CONFIG_HW_NAND_PEB_SIZE", "0x20000");
	token_set("CONFIG_HW_NAND_MIN_IO_SIZE", "2048");
	token_set("CONFIG_HW_NAND_SUB_PAGE_SIZE", "512");
   
	/* Temporary, relevant only for FIBERTEC distribution */
	if (token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	{
	    token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
	    atheros_ar5xxx_add_ordered(token_get("CONFIG_RG_FIBERTEC") ?
		fibertec_vap_order : NULL);
	}

	if (token_get("CONFIG_MARVELL_MMP"))
	{
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set_y("CONFIG_PHONE");
	    token_set_y("CONFIG_MV_PHONE");
	    token_set_y("CONFIG_MV_TDM_SUPPORT");
	    token_set_y("CONFIG_ZARLINK_SLIC_SUPPORT");
	    token_set_y("CONFIG_ZARLINK_SLIC_VE880");
	    token_set_y("CONFIG_MV_TDM_PCM_CLK_8MHZ");
	    token_set_y("CONFIG_MV_PHONE_USE_SLIC_LIBS");
	}

	if (token_get("CONFIG_HW_ENCRYPTION"))
	{
	    token_set_m("CONFIG_IPSEC_ALG_JOINT");
	    token_set_y("CONFIG_IPSEC_ALG_JOINT_ASYNC");
	    token_set_y("CONFIG_IPSEC_USE_FEROCEON_CRYPTO");
	}

	if (token_get("CONFIG_MV_WIFI_8764"))
	{
	    marvell_8xxx_add(0, token_get("CONFIG_RG_FIBERTEC") ? 3 : 1);
	    marvell_8xxx_add(1, token_get("CONFIG_RG_FIBERTEC") ? 3 : 1);
	}

	if (token_get("CONFIG_HW_LEDS"))
	{
	    token_set_y("CONFIG_RG_UIEVENTS");
	    token_set_m("CONFIG_RG_KLEDS");
	}
	if (token_get("CONFIG_QUANTENNA_QHS_71X"))
            dev_add("host0", DEV_IF_QHS71X_HOST, DEV_IF_NET_INT);
    }
    
    if (token_get("CONFIG_RG_IPV6_OVER_IPV4_TUN"))
	dev_add("sit0", DEV_IF_IPV6_OVER_IPV4_TUN, DEV_IF_NET_INT);

    if (IS_HW("OLYMPIA_P40X"))
    {
	/* Linux/Build generic */
	set_big_endian(0);
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");

	token_set_y("CONFIG_CENTILLIUM_P400");
	token_set_y("CONFIG_P400_REF");
	token_set_m("CONFIG_MTD_CTLM_SF");

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");

	token_set_y("CONFIG_SERIAL_EXTERNAL_8250");
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "4");

	token_set("CONFIG_BLK_DEV_RAM_SIZE", "8192");

	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8");
	token_set("CONFIG_MTD_PHYSMAP_START", "0xbe000000");

	/* next step, interfaces. Not there yet */
	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_y("CONFIG_RG_ATM_QOS");
	    token_set_m("CONFIG_P400_ATM");
	    dsl_atm_dev_add("atm0", DEV_IF_P400_DSL, NULL);
	}

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_y("CONFIG_MII");
	    dev_add("eth0", DEV_IF_KS8995MA_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_y("CONFIG_MII");
#if 0 /* ETH Switch*/
	    token_set_m("CONFIG_HW_SWITCH_BCM53XX");
	    token_set_m("CONFIG_BCM963XX_ETH");
	    switch_dev_add("bcm1", NULL, DEV_IF_BCM5325E_HW_SWITCH,
		DEV_IF_NET_INT, 5);
#endif
	}

	/* WLAN */
	token_set_y("CONFIG_PCI");
	if (token_get("CONFIG_HW_80211G_RALINK_RT2561"))
	    ralink_rt2561_add("ra0");

	/* USB */ 
	if (token_get("CONFIG_HW_USB_RNDIS"))
	{
	    token_set_y("CONFIG_USB_RNDIS");
	    token_set_y("CONFIG_USB_CENTILLIUM");
	    dev_add("usb0", DEV_IF_USB_RNDIS, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	if (token_get("CONFIG_HW_ENCRYPTION"))
	    token_set_y("CONFIG_IPSEC_USE_CENTILLIUM_CRYPTO");

	/* VOICE  */
	if (token_get("CONFIG_HW_DSP"))
	{
	    token_set_m("CONFIG_P400_DSP");
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
	    token_set("CONFIG_HW_NUMBER_OF_FXO_PORTS", "1");
	    token_set_y("CONFIG_HW_FXO");
	}

	token_set("BOARD", "OLYMPIA_P40X");
	token_set("FIRM", "Centillium");
    }

    if (IS_HW("TNETC550W") || IS_HW("BVW3653") || IS_HW("MVG3420N"))
    {
	token_set_y("CONFIG_BOOTLDR_UBOOT");
	token_set("CONFIG_BOOTLDR_UBOOT_COMP", "gzip");
	token_set_y("CONFIG_AVALANCHE_U_BOOT");	

	token_set_y("CONFIG_AVALANCHE_COMMON");
	token_set_y("CONFIG_MACH_PUMA5");
	if (IS_HW("TNETC550W"))
	    token_set_y("CONFIG_MACH_PUMA5EVM");
	else if(IS_HW("BVW3653"))
	    token_set_y("CONFIG_MACH_PUMA5_BVW3653");
	else if(IS_HW("MVG3420N"))
	    token_set_y("CONFIG_MACH_PUMA5_MVG3420N");

	if (token_get("CONFIG_HW_ETH_LAN"))
	{
	    token_set_y("CONFIG_MII");
	    token_set_y("CONFIG_ARM_AVALANCHE_CPGMAC_F");
	    token_set("CONFIG_ARM_CPMAC_PORTS", "1");
	    token_set_y("CONFIG_AVALANCHE_LOW_CPMAC");
	    token_set_y("CONFIG_ARM_CPMAC_DS_TRAFFIC_PRIORITY");
	    
	    dev_add("eth0", DEV_IF_AVALANCHE_CPGMAC_F_ETH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_SWITCH_LAN"))
	{
	    token_set_y("CONFIG_MII");
	    token_set_y("CONFIG_ARM_AVALANCHE_CPGMAC_F");
	    token_set("CONFIG_ARM_CPMAC_PORTS", "1");
	    token_set_y("CONFIG_AVALANCHE_LOW_CPMAC");
	    if(IS_HW("BVW3653"))
	    {
		token_set("CONFIG_AR8316_CPU_PORT", "0");
		token_set("CONFIG_AR8316_WIRELESS_PORT", "5");
	    }
	    else if(IS_HW("MVG3420N"))
	    {
		token_set("CONFIG_AR8316_CPU_PORT", "5");
		token_set("CONFIG_AR8316_WIRELESS_PORT", "0");
	    }
	    token_set_y("CONFIG_AVALANCHE_SWITCH_PROMISCOUS");
	    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	    {
		dev_add("eth0", DEV_IF_TI550_ETH, DEV_IF_NET_INT);
		switch_dev_add("eth0_main", "eth0", DEV_IF_AR8316_HW_SWITCH,
		    DEV_IF_NET_INT, token_get("CONFIG_AR8316_CPU_PORT"));
	    }
	    else
		dev_add("eth0", DEV_IF_AR8316_HW_SWITCH, DEV_IF_NET_INT);
	}

	if (token_get("CONFIG_HW_80211N_RALINK_RT2880") ||
	    token_get("CONFIG_HW_80211N_RALINK_RT3052"))
	{
	    ralink_rt2880_add("ra0");
	    if (token_get("CONFIG_HW_80211N_RALINK_RT3052"))
		token_set_y("CONFIG_RALINK_RT3052"); 
	}
	if (token_get("CONFIG_RALINK_RT2880"))
	{
	    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	    {
		dev_set_dependency("ra0", "eth0_main");
		token_set("CONFIG_RALINK_RT2880_DEF_RX_DEV", "\"eth0_main\"");
	    }
	    else
	    {
		dev_set_dependency("ra0", "eth0");
		token_set("CONFIG_RALINK_RT2880_DEF_RX_DEV", "\"eth0\"");
	    }
	}

	if (token_get("CONFIG_HW_CABLE_WAN"))
	{
	    dev_add("lbr0", DEV_IF_TI550_ETH_DOCSIS3, DEV_IF_NET_EXT);
	    dev_add("cni0", DEV_IF_TI550_CNI_DOCSIS3, DEV_IF_NET_EXT);
	    dev_set_dependency("lbr0", "cni0");
	}

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	    {
		conf_err("Fastpath can't be enabled when switch port "
		    "separation is used (see B85161)\n");
	    }
	    token_set_y("CONFIG_TI_PACKET_PROCESSOR");
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH",
		"vendor/ti/avalanche/rg/modules");
	}
	
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_m("CONFIG_USB_MUSB_HDRC");
	    token_set_y("CONFIG_USB_MUSB_SOC");
	    token_set_y("CONFIG_USB_MUSB_HOST");
	    token_set_y("CONFIG_USB_MUSB_HDRC_HCD");
	    token_set_y("CONFIG_USB_INVENTRA_FIFO");

	    /* debug flag */
	    token_set("CONFIG_USB_INVENTRA_HCD_LOGGING", "0");
	}

	if (token_get("CONFIG_HW_LEDS"))
	    token_set_y("CONFIG_RG_UIEVENTS");

	token_set("CONFIG_SDRAM_SIZE", "64");

	if (IS_HW("TNETC550W"))
	{
	    token_set("BOARD", "TNETC550W");
	    token_set("FIRM", "Texas Instruments");
	}
	else if(IS_HW("BVW3653"))
	{
	    token_set("BOARD", "BVW3653");
	    token_set("FIRM", "Hitron");
	    token_set_y("CONFIG_RG_UIEVENTS");
	}
	else if(IS_HW("MVG3420N"))
	{
	    token_set("BOARD", "MVG3420N");
	    token_set("FIRM", "Mototech");
	}
    }
    
    if (!token_get_str("CONFIG_RG_MAINFS_MTDPART_NAME"))
	token_set("CONFIG_RG_MAINFS_MTDPART_NAME", "\"mainfs\"");
}

