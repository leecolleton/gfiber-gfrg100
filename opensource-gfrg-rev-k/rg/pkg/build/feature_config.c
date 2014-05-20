/****************************************************************************
 *
 * rg/pkg/build/feature_config.c
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
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>

#include "config_opt.h"
#include "create_config.h"

static void assert_dep(char *dependent, ...)
{
    va_list ap;
    char *token;

    if (!token_get(dependent))
	return;

    va_start(ap, dependent);
    while ((token = va_arg(ap, char *)))
    {
	if (!token_get(token))
	{
	    fprintf(stderr, "Failed dependency check: "
		"%s is dependent on %s.\n", dependent, token);
	    exit(-1);
	}
    }
    va_end(ap);
}

static int is_gui_selected(void)
{
    option_t *p;
    
    for (p = openrg_config_options; p->token; p++)
    {
	/* Return 1 if found any selected theme */
	if (p->value && p->type&OPT_THEME)
	    return 1;
    }
    return 0;
}

static void select_default_themes(void)
{
    token_set_y("CONFIG_GUI_RG2");
    token_set("CONFIG_RG_DEF_THEME", "openrg2");
    if (token_get("CONFIG_RG_SMB"))
    {
	token_set_y("CONFIG_GUI_SMB2");
	token_set("CONFIG_RG_DEF_THEME", "opensmb2");
    }
}

static int is_dsp_configurable(void)
{
    return token_get("CONFIG_VINETIC") ||
	token_get("CONFIG_COMCERTO_COMMON") || token_get("CONFIG_CX9451X_DSP")
	|| token_get("CONFIG_BCM963XX_DSP") || token_get("CONFIG_P400_DSP") ||
	token_get("CONFIG_COMCERTO_COMMON_821XX") ||
	token_get("CONFIG_BCM_BCMDSP") || token_get("CONFIG_XRX");
}

void _set_big_endian(char *file, int line, int is_big)
{
    if (is_big)
	token_set_y("CONFIG_CPU_BIG_ENDIAN");
    else
	token_set_y("CONFIG_CPU_LITTLE_ENDIAN");
    _token_set(file, line, SET_PRIO_TOKEN_SET, "TARGET_ENDIANESS",
	is_big ? "BIG" : "LITTLE");
}

void hw_wireless_features(void)
{
    token_set("RG_SSID_STR", "Home Network");

    if (!token_get_str("CONFIG_RG_SSID_NAME"))
	token_set("CONFIG_RG_SSID_NAME", token_get_str("RG_PROD_STR"));

    if (token_get("CONFIG_HW_80211G_UML_WLAN"))
    {
        target_os_enable_wireless();
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    if (!token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_8021X_WBM");
	    token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	}
    }

    if (token_get("CONFIG_HW_80211G_ISL_SOFTMAC"))
    {
	token_set_m("CONFIG_ISL_SOFTMAC");
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    token_set_y("CONFIG_RG_8021X_MD5");
	    token_set_y("CONFIG_RG_8021X_TLS");
	    token_set_y("CONFIG_RG_8021X_TTLS");
	    token_set_y("CONFIG_RG_8021X_RADIUS");
	}
    }

    if (token_get("CONFIG_HW_80211N_AIRGO_AGN100"))
    {
        target_os_enable_wireless();
	token_set_y("CONFIG_RG_WIRELESS");
	token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	token_set_y("CONFIG_RG_WPA_WBM");
	token_set_y("CONFIG_RG_8021X_WBM");
	token_set_m("CONFIG_AGN100");
	token_set_y("CONFIG_NETFILTER");
    }

    if (token_get("CONFIG_HW_80211G_BCM43XX"))
    {
        target_os_enable_wireless();
	token_set("CONFIG_BCM43XX_MODE", "AP");
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    token_set_y("CONFIG_RG_8021X_MD5");
	    token_set_y("CONFIG_RG_8021X_TLS");
	    token_set_y("CONFIG_RG_8021X_TTLS");
	    if (!token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_WPA_BCM");
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	    token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	}
	if (token_get("MODULE_RG_REDUCE_SUPPORT") && 
	    /* not allowed to export this driver */
	    !token_get("CONFIG_RG_JPKG")) 
	{
	    token_set_y("CONFIG_BCM43XX_MULTIPLE_BSSID");
	}
    }

    if (token_get("CONFIG_RALINK_RT2560") ||
        token_get("CONFIG_RALINK_RT2561") ||
        token_get("CONFIG_RALINK_RT2860") ||
	token_get("CONFIG_RALINK_RT2880"))
    {
        target_os_enable_wireless();
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    if (!token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_8021X_WBM");
	    if (token_get("CONFIG_RALINK_RT2561") ||
		token_get("CONFIG_RALINK_RT2860") ||
		token_get("CONFIG_RALINK_RT2880"))
	    {
		token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    }
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	}
    }

    if (token_get("CONFIG_RG_ATHEROS_HW_AR5212") ||
	token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
    {
	token_set_m("CONFIG_RG_ATHEROS");
        target_os_enable_wireless();
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    if (!token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_8021X_WBM");
	    token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	}
    }

    if (token_get("CONFIG_HW_80211N_LANTIQ_WAVE300"))
    {
        target_os_enable_wireless();
	token_set_y("CONFIG_CRYPTO");
        token_set_y("CONFIG_CRYPTO_HASH");
        token_set_y("CONFIG_CRYPTO_ALGAPI");
	token_set_y("CONFIG_CRYPTO_HMAC");
	token_set_y("CONFIG_CRYPTO_MD5");
	token_set_y("CONFIG_CRYPTO_AES");
	token_set_y("CONFIG_DRIVER_MTLK");

	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
            token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    token_set_y("CONFIG_RG_8021X_WBM");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
            if (!token_get("CONFIG_RG_INTEGRAL"))
                token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_HOSTAPD");
	}
    }

    if (token_get("CONFIG_HW_80211N_BCM338X"))
    {
        target_os_enable_wireless();
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    token_set_y("CONFIG_RG_8021X_WBM");
            if (!token_get("CONFIG_RG_INTEGRAL"))
                token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	}
    }

    if (token_get("CONFIG_MV_WIFI_8764") || token_get("CONFIG_MV_WIFI_8864"))
    {
        target_os_enable_wireless();
	if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
	{
	    token_set_y("CONFIG_RG_WIRELESS");
	    token_set_y("CONFIG_RG_VENDOR_WLAN_SEC");
	    if (!token_get("CONFIG_RG_FIBERTEC") && !token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
	    token_set_y("CONFIG_RG_WPA_WBM");
	    token_set_y("CONFIG_RG_8021X_WBM");
	    token_set_y("CONFIG_RG_WLAN_AUTO_CHANNEL_SELECT");
	    token_set_y("CONFIG_RG_WSEC_DAEMON");
	}
    }

#ifdef CONFIG_RG_DO_DEVICES
    if (!token_get("CONFIG_RG_DEFAULT_80211N_MODE"))
	token_set("CONFIG_RG_DEFAULT_80211N_MODE", "%d", DOT11_MODE_BGN);
#endif
}

void general_features(void)
{
    token_set_default("CONFIG_HW_FLASH_NOR", "y");

    /* unless stated otherwise, the default is standard size */
    if (!token_get_str("CONFIG_RG_STANDARD_SIZE_CONF_SECTION") || 
	token_get("CONFIG_RG_JPKG"))
    {
	token_set_y("CONFIG_RG_STANDARD_SIZE_CONF_SECTION");
    }
    if (!token_get_str("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION") ||
	token_get("CONFIG_RG_JPKG"))
    {
	token_set_y("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION");
    }

    /* enable full service list to platforms with no memory limitations */
    if (token_get("CONFIG_RG_STANDARD_SIZE_CONF_SECTION"))
        token_set("CONFIG_RG_ADV_SERVICES_LEVEL", "2");
    else
	token_set("CONFIG_RG_ADV_SERVICES_LEVEL", "1");

    if (token_get("CONFIG_RG_MTD_DEFAULT_PARTITION") &&
	!token_get("CONFIG_RG_MTD"))
    {
	token_set_y("CONFIG_RG_MTD");
    }

    if (token_get("CONFIG_RG_PERM_STORAGE_UBI"))
    {
	token_set("CONFIG_RG_UBI_PARTITION", "UBI");
	token_set_y("CONFIG_MTD_UBI");
    }
    if (!token_get_str("CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION"))
    {
	token_set("CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION", "OpenRG");
    }

    if (token_get("MODULE_RG_SNMP"))
    {
	token_set_y("CONFIG_RG_SNMP");
	token_set_y("CONFIG_RG_UCD_SNMP");
	token_set_y("CONFIG_RG_UCD_SNMP_V3");
    }

    if (token_get("MODULE_RG_SNMP_CLIENT"))
    {
	token_set_y("CONFIG_RG_SNMP_CLIENT");
	token_set_y("CONFIG_RG_NET_SNMP");
	token_set_y("CONFIG_RG_NET_SNMP_GET");
	token_set_y("CONFIG_RG_NET_SNMP_WALK");
	token_set_y("CONFIG_RG_NET_SNMP_SET");
    }

    if (token_get("CONFIG_RG_UCD_SNMP_STATISTICS"))
	token_set_y("CONFIG_RG_KNET_MIB2_IF_COUNTERS");

    if (token_get("MODULE_RG_UPNP"))
	token_set_y("CONFIG_RG_IGD");

    if (token_get("MODULE_RG_UPNP_AV"))
    {
	token_set_y("CONFIG_RG_UPNP_AV");
	token_set_y("CONFIG_RG_DISK_MNG");
    }
    
    if (token_get("MODULE_RG_ACCESS_DLNA"))
	token_set_y("CONFIG_RG_ACCESS_DLNA");

    if (token_get("MODULE_RG_MEDIA_SERVER"))
	token_set_y("CONFIG_RG_MEDIA_SERVER");

    if (token_get("MODULE_RG_VIDEO_SURVEILLANCE"))
	token_set_y("CONFIG_RG_UPNP_SNIFFER");

    if (token_get("MODULE_RG_OSAP_AGENT"))
    {
	token_set_y("CONFIG_RG_OSAP_AGENT");
	token_set_y("CONFIG_RG_OSAP_STORAGE");
	token_set_y("CONFIG_RG_OSAP_OCC");
    }

    if (token_get("MODULE_RG_MODULAR_UPGRADE"))
    {
	token_set_y("CONFIG_RG_DLM");
	token_set_y("CONFIG_RG_DPKG");
	token_set_y("CONFIG_RG_PLUGINS");
	token_set_y("CONFIG_BLK_DEV_LOOP");
    }

    if (token_get("MODULE_RG_REDUCE_SUPPORT"))
    {
	token_set_y("CONFIG_RG_REDUCE_SUPPORT");
	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    /* Configure ethoa0 and ppp0 devices */
	    token_set_y("CONFIG_RG_PPP_PRECONF");
	    token_set("CONFIG_RG_PPPOA_PRECONF_DEV_NAME",
		token_get_str("CONFIG_ATM_DEV_NAME"));

	}
	if ((token_get("CONFIG_HW_DSL_WAN") || 
	    token_get("CONFIG_HW_CABLE_WAN")) &&
	    token_get("CONFIG_RG_REDUCE_SUPPORT"))
	{
	    token_set_y("CONFIG_RG_NOISY_LINE");
	}
	if (!token_get("CONFIG_RG_INTEGRAL"))
	    token_set_y("CONFIG_ZERO_CONFIGURATION");
	if (token_get("CONFIG_HW_DSL_WAN"))
	    token_set_y("CONFIG_RG_WALLED_GARDEN");
	token_set_y("CONFIG_RG_HELPLINE");
	token_set_y("CONFIG_RG_HELPLINE_UNIQUE");
	if (token_get("CONFIG_RG_INTEGRAL"))
	{
	    token_set_y("CONFIG_RG_SECURED_NETWORK_AS_HOME");
	    token_set_y("CONFIG_RG_SECURED_NETWORK_ALLOWS_NONE");
	}
	/* DNS caching is useful to prevent usage of 1.1.1.1 false IP address
	 * in web auth and HTTP interception. It also wastes a lot of CPU cycles
	 * on every DNS query and causes many flash commits. Do not use it in
	 * production unless you really must. */
	token_set_y("CONFIG_RG_DNS_CACHE");
	if (token_get("CONFIG_RG_WIRELESS"))
	{
	    if (!token_get("CONFIG_RG_INTEGRAL"))
		token_set_y("CONFIG_RG_SCR_GLOBAL_WLAN_PW");
	    token_set_y("CONFIG_RG_FIRST_TIME_INSTALL_RG_SCR");
	}
	token_set_y("CONFIG_RG_SYSLOG");
	token_set_y("CONFIG_RG_LOG_STATISTICS");
	token_set("CONFIG_RG_SYSLOG_STATS_SIZE", "4096"); /* 4 KB */
	token_set_y("CONFIG_RG_REDUCE_SUPPORT_DNS_ALIASING");
    }

    if (token_get("MODULE_RG_IPV6"))
    {
	token_set_y("CONFIG_RG_IPV6");
	token_set_y("CONFIG_RG_DHCPV6C");
	token_set_y("CONFIG_RG_DHCPV6S");
    }

    if (token_get("MODULE_RG_IPV6_OVER_IPV4_TUN"))
	token_set_y("CONFIG_RG_IPV6_OVER_IPV4_TUN");

    if (token_get("MODULE_RG_ADVANCED_ROUTING"))
    {
	token_set_y("CONFIG_RG_RIP");
	if (token_get("CONFIG_RG_JPKG") ||
 	    token_get("CONFIG_RG_ADV_SERVICES_LEVEL") > 1)
	{
	    token_set_y("CONFIG_RG_BGP");
	    token_set_y("CONFIG_RG_OSPF");
	}
	token_set_y("CONFIG_RG_DNS_CONCURRENT");
	token_set_y("CONFIG_RG_DNS_DOMAIN_ROUTING");
#if 0
	/* Does not compile until B125678 is fixed */
	token_set_y("CONFIG_RG_TUNNELS");
#endif
    }

    if (token_get("CONFIG_RG_BGP") || token_get("CONFIG_RG_OSPF"))
	token_set_y("CONFIG_RG_QUAGGA");

    if (token_get("CONFIG_RG_TUNNELS"))
    {
	token_set_y("CONFIG_INET_TUNNEL");
	token_set_y("CONFIG_NET_IPIP");
	token_set_y("CONFIG_NET_IPGRE");
    }

    if (token_get("CONFIG_RG_DNS_DOMAIN_ROUTING"))
	token_set_y("CONFIG_RG_DNS_ROUTE");

    if (token_get("MODULE_RG_FIREWALL_AND_SECURITY"))
    {
	token_set_y("CONFIG_RG_RNAT");
	token_set_y("CONFIG_RG_FIREWALL");

	/* Time of Day Client, SNTP Client */
	token_set_y("CONFIG_RG_TODC");
    }

    if (token_get("MODULE_RG_WLAN_AND_ADVANCED_WLAN"))
    {
	/* tokens which are set if a certain config is NOT enabled, should be
	 * always enabled in a jpkg dist, if this feature can be disabled later
	 * by the makeflags/license
	 */
	if (!token_get("CONFIG_RG_VENDOR_WLAN_SEC") || 
	    token_get("CONFIG_RG_JPKG"))
	{
	    token_set_y("CONFIG_RG_8021X_MD5");
	    token_set_y("CONFIG_RG_8021X_TLS");
	    token_set_y("CONFIG_RG_8021X_TTLS");
	}
    }

    if (token_get("MODULE_RG_CABLEHOME"))
    {
	token_set_y("CONFIG_RG_CABLEHOME_11");
	token_set_y("CONFIG_RG_CABLEHOME");
	token_set_y("CONFIG_RG_CABLEHOME_EMBEDDED");
    }

    if (token_get("MODULE_RG_BLUETOOTH"))
	token_set_y("CONFIG_RG_BLUETOOTH");

    token_set_y("CONFIG_RG_LICENSE");

    /* Enable debug symbols in linux (for footprint_kernel report) */
    if (!token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_DEBUG_INFO");
    
    /* Enable threads for all platforms. See explanation in
     * sys_get_ticks_from_boot() in pkg/util/sys.c.
     */
    if (!token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_RG_THREADS");

    if (token_get("MODULE_RG_FOUNDATION") || token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_RG_IPROUTE2");
	
    if (token_get("MODULE_RG_FOUNDATION"))
    {
	token_set_y("CONFIG_RG_FOUNDATION_CORE");

	token_set_y("CONFIG_RG_CHECK_BAD_REBOOTS");

    	/* DHCP Server */
	token_set_y("CONFIG_RG_DHCPS");
	token_set_y("CONFIG_AUTO_LEARN_DNS");

	token_set_y("CONFIG_RG_LAN_HOST_LINK_MONITOR");

    	/* DHCP Relay */
	if (!token_get("CONFIG_RG_INTEGRAL"))
    	    token_set_y("CONFIG_RG_DHCPR");

	/* Telnet Server */
	token_set_y("CONFIG_RG_TELNETS");

	/* Bridging */
	token_set_m("CONFIG_RG_BRIDGE");
	token_set_y("CONFIG_RG_HYBRID_BRIDGE");
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set_m("CONFIG_RG_USFS");

	/* STP */
	if (!token_get("CONFIG_HW_SWITCH_PORT_SEPARATION") && !token_get("CONFIG_FEROCEON_COMMON"))
	    token_set_m("CONFIG_RG_STP");

	/* NAT/NAPT */	
	token_set_y("CONFIG_RG_NAT");

	/* ALG support */
	token_set_y("CONFIG_RG_ALG");
	token_set_y("CONFIG_RG_ALG_SIP");
	token_set_y("CONFIG_RG_ALG_MGCP");
	token_set_y("CONFIG_RG_ALG_H323");
	token_set_y("CONFIG_RG_ALG_AIM");
	token_set_y("CONFIG_RG_ALG_MSNMS");
	token_set_y("CONFIG_RG_ALG_PPTP");
	token_set_y("CONFIG_RG_ALG_IPSEC");
	token_set_y("CONFIG_RG_ALG_L2TP");
	token_set_y("CONFIG_RG_ALG_ICMP");
	token_set_y("CONFIG_RG_ALG_PORT_TRIGGER");
	token_set_y("CONFIG_RG_ALG_FTP");
	token_set_y("CONFIG_RG_PROXY_RTSP");

	if (!token_get("CONFIG_RG_INTEGRAL"))
	    token_set_m("CONFIG_RG_PPPOE_RELAY");

	/* Firewall */
	token_set_m("CONFIG_RG_MSS");
	
	/* Remote management */
	token_set_y("CONFIG_RG_RMT_MNG");
	token_set_y("CONFIG_RG_OSS_RMT");

	/* various */ 
	token_set_y("CONFIG_RG_STATIC_ROUTE");
	if (!token_get("CONFIG_RG_CABLEHOME_11"))
	    token_set_m("CONFIG_RG_IGMP_PROXY");
	
	token_set_y("CONFIG_RG_PING");
	token_set_y("CONFIG_RG_TRACEROUTE");
	token_set_y("CONFIG_RG_TCP_DETECT");
	token_set_y("CONFIG_RG_ARP_PING");

	/* Set DNS+DHCP to no-cablehome distributions (B11715) */
	if (!token_get("CONFIG_RG_CABLEHOME_11"))
	{
	    token_set_y("CONFIG_RG_ALG_DNS");
	    token_set_y("CONFIG_RG_ALG_DHCP");
	}

	/* WBM */
	token_set_y("CONFIG_RG_WBM");
	token_set_y("CONFIG_RG_TZ_COMMON");
	if (!token_get("CONFIG_RG_REDUCE_SUPPORT") && !token_get_str("CONFIG_RG_WBM_CONN_TROUBLESHOOT"))
	    token_set_y("CONFIG_RG_WBM_CONN_TROUBLESHOOT");
	if (token_get("CONFIG_RG_INTEGRAL"))
	    token_set("CONFIG_RG_INSTALLATION_WIZARD", "n");

	if (token_get("CONFIG_OPENRG"))
	{
	    /* FIXME:Uncomment this once OO WBM is operational */
	    /* token_set_y("CONFIG_RG_WEB"); */

	    token_set_y("CONFIG_RG_WEB_LIB_UTIL");
	    token_set_y("CONFIG_RG_WEB_LIB_GRAPHICS");
	    token_set_y("CONFIG_RG_WEB_LIB_NETWORK");
	    token_set_y("CONFIG_RG_WEB_LIB_RG");
	}

	/* Auto IP Configuration */
	token_set_y("CONFIG_ZC_IP_AUTO_DETECTION");

	/* Preconfiguration */
	token_set_y("CONFIG_RG_PRECONF");

	/* WAN connection type auto detection */
	token_set_y("CONFIG_RG_ETH_WAN_DETECT");

	/* Netlink wrapper library */
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set_y("CONFIG_RG_NETLINK_LIBNL");
    }

    if (token_get("CONFIG_RG_REDUCE_SUPPORT"))
    {
	/* SCRs */
	if (token_get("CONFIG_RG_JNET_CLIENT"))
	{
	    token_set_y("CONFIG_RG_JNET_SCR");
	    token_set_y("CONFIG_RG_JNET_SCR_CONNECTING");
	    if (token_get("CONFIG_RG_INTEGRAL"))
		token_set("CONFIG_RG_JNET_SCR_CONNECTING_TIMEOUT", "25");
	    else
		token_set("CONFIG_RG_JNET_SCR_CONNECTING_TIMEOUT", "90");
	}
    }

    if (token_get("CONFIG_RG_FOUNDATION_CORE"))
    {
	token_set_y("CONFIG_OPENRG");
        if (!token_get("CONFIG_RG_UML"))
	    token_set_y("CONFIG_RG_RAND_POOL_RAM_INIT");

	/* Set general flag for languages support */
	token_set_y("CONFIG_RG_LANG");

	token_set_m("CONFIG_RG_KOS");
	token_set_y("CONFIG_RG_KOS_KNET");
	
	/* Command Line Interface (CLI) */
	token_set_y("CONFIG_RG_CLI");
	token_set_y("CONFIG_RG_RGLOADER_CLI_CMD");
	token_set_y("CONFIG_RG_LOGIN");
	token_set_y("CONFIG_RGLOADER_CLI");
	token_set_y("CONFIG_RG_CONFIG_STRINGS");

	/* DNS Server */ 
	token_set_y("CONFIG_RG_DNS");
	token_set_y("CONFIG_RG_ERESOLV");

	/* Time of Day Client, SNTP Client */
	token_set_y("CONFIG_RG_TODC");

	/* Dynamic DNS */
	token_set_y("CONFIG_RG_DDNS");

	/* DHCP Client */
    	token_set_y("CONFIG_RG_DHCPC");

	if (!token_get("CONFIG_HW_CABLE_WAN"))
	{
	    /* In Cable modems upgrade is done via DOCSIS */
	    if (token_get("CONFIG_RG_WBM"))
		token_set_y("CONFIG_RG_LAN_UPGRADE");
	    token_set_y("CONFIG_RG_WAN_UPGRADE");
	    token_set_y("CONFIG_RG_RMT_UPDATE");
	}

	token_set_y("CONFIG_RG_SYSINIT");
	
	token_set_y("CONFIG_RG_PERM_STORAGE");
	
	token_set_y("CONFIG_RG_CHRDEV");

	/* various */ 
	token_set_y("CONFIG_RG_BUSYBOX");

	/* MAC cloning */
	if (!token_get("CONFIG_RG_INTEGRAL"))
	    token_set_y("CONFIG_RG_MAC_CLONING");

	/* IP V4 */
	token_set_m("CONFIG_RG_IPV4");
	
	/* The following are implicit in the system and are not mentioned
	 * here explicitly:
	 * Spanning tree protocol
	 * TFTP
	 * Serial access
	 * Autolearning of MAC addresses
	 * Multiple user support
	 * Multilingual support
	 * Connection wizard setup
	 * Connection diagnostics
	 */
    }

    if (token_get("MODULE_RG_ADVANCED_MANAGEMENT"))
    {
	/* OpenRG Advanced Management */

	/* Secured transmission Secured Login; HTTP-S; Telnet-S */
	token_set_y("CONFIG_RG_SSL");

	/* Email notification */
	token_set_y("CONFIG_RG_ENTFY");

	/* Event Logging */
    	token_set_y("CONFIG_RG_EVENT_LOGGING");

	/* SSH */
	if (!token_get("CONFIG_RG_FIBERTEC") || 
	     token_get("CONFIG_FIBERTEC_DOGFOOD"))
	    token_set_y("CONFIG_RG_SSH");

	/* SNTP Server */
	token_set_y("CONFIG_RG_SNTPS");
    }

    if (token_get("MODULE_RG_PPTP"))
    {
	token_set_y("CONFIG_RG_PPTPC");
	token_set_y("CONFIG_RG_PPTPS");
	token_set_y("CONFIG_RG_PPTP_VPN");
	token_set_y("CONFIG_RG_PPP_MPPE");
	token_set_m("CONFIG_RG_PPP_COMMON");
    }

    if (token_get("MODULE_RG_PPP"))
    {
	token_set_m("CONFIG_RG_PPP_COMMON");
	token_set_y("CONFIG_RG_PPP");
	token_set_y("CONFIG_RG_PPPOE");
	if (!token_get("CONFIG_RG_INTEGRAL"))
	{
	    token_set_y("CONFIG_RG_PPTPC");
	    token_set_y("CONFIG_RG_L2TPC");
	    token_set_y("CONFIG_RG_PPP_AUTHENTICATION");
	}

	token_set_y("CONFIG_RG_PPP_DEFLATE");
	token_set_y("CONFIG_RG_PPP_BSDCOMP");

	if (token_get("CONFIG_ATM") && !token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set_m("CONFIG_RG_PPPOA");

	if (!token_get_str("CONFIG_RG_PPP_ON_DEMAND_DEFAULT_MAX_IDLE_TIME"))
	{
	    token_set("CONFIG_RG_PPP_ON_DEMAND_DEFAULT_MAX_IDLE_TIME",
		"1200");
	}
	if (!token_get_str("CONFIG_RG_PPP_DEFAULT_BSD_COMPRESSION"))
	{
	    /* XXX Originally the value here was '0' (PPP_COMP_REJECT),
	     * but the generated rg_config.h file printed it as '1'
	     * (PPP_COMP_ALLOW), due to a bug in create_config.c.
	     */
	    /* from include/enums.h PPP_COMP_ALLOW is 1 */
	    token_set("CONFIG_RG_PPP_DEFAULT_BSD_COMPRESSION", "1");
	}
	if (!token_get_str("CONFIG_RG_PPP_DEFAULT_DEFLATE_COMPRESSION"))
	{
	    /* XXX Originally the value here was '0' (PPP_COMP_REJECT),
	     * but the generated rg_config.h file printed it as '1'
	     * (PPP_COMP_ALLOW), due to a bug in create_config.c.
	     */
	    /* from include/enums.h PPP_COMP_ALLOW is 1 */
	    token_set("CONFIG_RG_PPP_DEFAULT_DEFLATE_COMPRESSION", "1");
	}
    }

    if (token_get("MODULE_RG_DSL"))
    {
	token_set_y("CONFIG_ATM");
	token_set_y("CONFIG_ATM_PVC_SCAN");

	/* XXX temporarily disable oam ping for bcm compilation */
	if (!token_get("CONFIG_ARCH_SOLOS"))
	    token_set_y("CONFIG_RG_OAM_PING");

	token_set_y("CONFIG_ATM_SKB");
	token_set_y("CONFIG_ATM_BR2684");
	if (token_get("CONFIG_RG_PPP_COMMON"))
	    token_set_m("CONFIG_RG_PPPOA");
	if (token_get("CONFIG_RG_OS_LINUX_24") &&
	    !token_get("CONFIG_RG_INTEGRAL"))
	{
	    token_set_y("CONFIG_ATM_CLIP");
	}
    }

    if (token_get("MODULE_RG_VLAN"))
    {
	if (token_get("CONFIG_RG_BRIDGE"))
	    token_set_y("CONFIG_RG_VLAN_BRIDGE");

	token_set_y("CONFIG_RG_8021Q_IF");
    }
    
    if (token_get("CONFIG_RG_8021Q_IF"))
	token_set_y("CONFIG_RG_VLAN_8021Q");
        
    if (token_get("CONFIG_RG_VLAN_8021Q"))
	token_set_y("CONFIG_VLAN_8021Q");

    if (token_get("MODULE_RG_FULL_PBX"))
    {
	token_set_y("CONFIG_RG_FULL_PBX");
	token_set_y("CONFIG_RG_PBX");
	token_set_y("CONFIG_RG_DISK_MNG");
	token_set_y("CONFIG_RG_VOIP_AUTO_ATTENDANT");
	token_set_y("CONFIG_RG_VOIP_MOH");
	token_set_y("CONFIG_RG_VOIP_VOICEMAIL");
	token_set_y("CONFIG_RG_VOIP_HUNT_GROUP");
	token_set_y("CONFIG_RG_VOIP_DYNAMIC_TRUNK_GROUP");
	token_set_y("CONFIG_RG_VOIP_DYNAMIC_EXTENSIONS");
    }

    if (token_get("MODULE_RG_HOME_PBX"))
    {
	token_set_y("CONFIG_RG_HOME_PBX");
	token_set_y("CONFIG_RG_PBX");
	if (!token_get("CONFIG_RG_REDUCE_SUPPORT"))
	    token_set_y("CONFIG_RG_VOIP_DYNAMIC_EXTENSIONS");
    }

    if (token_get("MODULE_RG_PRINTSERVER"))
    {
	token_set_y("CONFIG_RG_PRINT_SERVER");
	token_set_y("CONFIG_RG_IPP");
	enable_module("CONFIG_RG_LPD");
    }

    if (token_get("MODULE_RG_MAIL_SERVER"))
    {
	token_set_y("CONFIG_RG_MAIL_SERVER");
	token_set_y("CONFIG_RG_DISK_MNG");
    }

    if (token_get("MODULE_RG_WEB_SERVER"))
    {
	token_set_y("CONFIG_RG_WEB_SERVER");
	token_set_y("CONFIG_RG_DISK_MNG");
    }

    if (token_get("MODULE_RG_FTP_SERVER"))
    {
	token_set_y("CONFIG_RG_FTP_SERVER");
	token_set_y("CONFIG_RG_DISK_MNG");
    }

    if (token_get("MODULE_RG_FILESERVER"))
    {
	token_set_y("CONFIG_RG_DISK_MNG");
	token_set_y("CONFIG_RG_FILESERVER");
	token_set_y("CONFIG_RG_SAMBA");
	token_set_y("CONFIG_RG_FILESERVER_ACLS");

	if (!token_get("CONFIG_RG_INTEGRAL"))
	{
	    token_set_y("CONFIG_RG_FS_BACKUP");
	    token_set_y("CONFIG_RG_RAID");
	    token_set_y("CONFIG_RG_WINS_SERVER");
	    token_set_y("CONFIG_RG_WINS_CLIENT");
	}

	if (token_get("MODULE_RG_PRINTSERVER"))
	    token_set_y("CONFIG_RG_SAMBA_PRINTING");

	/* ACL kernel support */
	if (token_get("CONFIG_RG_FILESERVER_ACLS"))
	{
	    token_set_y("CONFIG_FS_POSIX_ACL");

	    /* ACLs for EXT2 */
	    token_set_y("CONFIG_EXT2_FS_POSIX_ACL");
	    token_set_y("CONFIG_EXT2_FS_XATTR");
	    token_set_y("CONFIG_EXT2_FS_XATTR_SHARING");

	    /* ACLs for EXT3 */
	    token_set_y("CONFIG_EXT3_FS_POSIX_ACL");
	    token_set_y("CONFIG_EXT3_FS_XATTR");
	    token_set_y("CONFIG_EXT3_FS_XATTR_SHARING");

	    /* Filesystem Meta Information Block Cache */
	    token_set_y("CONFIG_FS_MBCACHE");
	}
    }

    if (token_get("CONFIG_RG_BITTORRENT"))
    {
	token_set_y("CONFIG_RG_OPENSSL");
	token_set_y("CONFIG_RG_DISK_MNG");
    }

    if (token_get("CONFIG_RG_DISK_MNG"))
    {
	if (!token_get("CONFIG_RG_INTEGRAL"))
	    token_set_y("CONFIG_RG_E2FSPROGS"); /* mke2fs, e2fsck */
	token_set_y("CONFIG_RG_DOSFSTOOLS"); /* mkdosfs, dosfsck */
	token_set_y("CONFIG_RG_UTIL_LINUX"); /* sfdisk */
	token_set_y("CONFIG_RG_DISKTYPE");   /* filesystem detection */
	/* ntfs-3g does not work on ARM based boards (XXX: Check Mindspeed) */
	token_set_y("CONFIG_RG_NTFS_3G"); /* ntfs-3g, libntfs-3g.so */

	token_set_y("CONFIG_HOTPLUG");
	token_set_y("CONFIG_MSDOS_PARTITION");
	token_set("CONFIG_FAT_DEFAULT_CODEPAGE", "437");
	if (!token_get_str("CONFIG_RG_CODEPAGE"))
	    token_set("CONFIG_RG_CODEPAGE", "\"437\"");

	token_set_y("CONFIG_FAT_FS");
	token_set_y("CONFIG_MSDOS_FS");
	token_set_y("CONFIG_VFAT_FS");
	token_set_y("CONFIG_EXT2_FS");
	token_set_y("CONFIG_EXT3_FS");
	token_set_y("CONFIG_JBD");
	token_set_y("CONFIG_SWAP");
	token_set("CONFIG_FAT_DEFAULT_IOCHARSET", "\"iso8859-1\"");

	if (token_get("CONFIG_RG_NTFS_3G"))
	{
	    token_set_y("CONFIG_RG_NTFSPROGS"); /* mkntfs */
	    token_set_y("CONFIG_FUSE_FS"); /* fusermount, libfuse.so */
	}
	else
	    token_set_y("CONFIG_NTFS_FS");

	if (token_get("CONFIG_RG_RAID"))
	{
	    token_set_y("CONFIG_MD");
	    token_set_y("CONFIG_BLK_DEV_MD");
	    token_set_y("CONFIG_MD_RAID0");
	    token_set_y("CONFIG_MD_RAID1");
	    token_set_y("CONFIG_MD_RAID5");
	}
        if (token_get("MODULE_RG_MAIL_SERVER") ||
	    token_get("MODULE_RG_WEB_SERVER") ||
	    token_get("MODULE_RG_FTP_SERVER"))
	{
	    token_set_y("CONFIG_RG_DISK_MNG_HOME_DIR");
	}
    }

    if (token_get("MODULE_RG_IPSEC"))
	token_set_y("CONFIG_FREESWAN");

    if (token_get("MODULE_RG_L2TP"))
    {
	token_set_y("CONFIG_RG_L2TPC");
	token_set_y("CONFIG_RG_L2TPS");
	token_set_y("CONFIG_RG_L2TP_VPN");
	token_set_y("CONFIG_RG_PPP_MPPE");
	token_set_m("CONFIG_RG_PPP_COMMON");
    }

    if (token_get("MODULE_RG_ATA"))
	token_set_y("CONFIG_RG_ATA");

    if (token_get("MODULE_RG_VOIP_RV_SIP"))
	token_set_y("CONFIG_RG_VOIP_RV_SIP");

    if (token_get("MODULE_RG_VOIP_RV_H323"))
	token_set_y("CONFIG_RG_VOIP_RV_H323");

    if (token_get("MODULE_RG_VOIP_RV_MGCP"))
	token_set_y("CONFIG_RG_VOIP_RV_MGCP");

    if (token_get("MODULE_RG_VOIP_ASTERISK_SIP"))
	token_set_y("CONFIG_RG_VOIP_ASTERISK_SIP");

    if (token_get("MODULE_RG_VOIP_ASTERISK_H323"))
	token_set_y("CONFIG_RG_VOIP_ASTERISK_H323");

    if (token_get("MODULE_RG_VOIP_ASTERISK_MGCP_GW"))
	token_set_y("CONFIG_RG_VOIP_ASTERISK_MGCP_GW");

    if (token_get("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT"))
	token_set_y("CONFIG_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");

    if (token_get("MODULE_RG_VOIP_OSIP"))
	token_set_y("CONFIG_RG_VOIP_OSIP");
    
    if (token_get("MODULE_RG_URL_FILTERING"))
	token_set_y("CONFIG_RG_URL_KEYWORD_FILTER");

    if (token_get("MODULE_RG_CHILDREN_NETWORK"))
	token_set_y("CONFIG_RG_CHILDREN_NETWORK");
    
    if (token_get("MODULE_RG_QOS"))
        token_set_y("CONFIG_RG_QOS");

    if (token_get("CONFIG_RG_QOS") && !token_get("CONFIG_RG_QOS_PRIO_BANDS"))
        token_set("CONFIG_RG_QOS_PRIO_BANDS", "4");

    if (token_get("MODULE_RG_ROUTE_MULTIWAN"))
	token_set_y("CONFIG_RG_ROUTE_MULTIWAN");
    
    if (token_get("MODULE_RG_DSLHOME"))
    {
	token_set_y("CONFIG_RG_DSLHOME");
	token_set_y("CONFIG_RG_TR_157");

	/* Enable vouchers for JPKG only, not enabled by default for flash size
	 * (footprint) reasons
	 */
	if (token_get("CONFIG_OPENRG") && token_get("CONFIG_RG_JPKG"))
	{
	    token_set_y("CONFIG_RG_DSLHOME_VOUCHERS");
	}
	
	if (token_get("CONFIG_RG_IPV6"))
	{
	    token_set_y("CONFIG_RG_DSLHOME_DATAMODEL_TR_181");
	    token_set_y("CONFIG_RG_DSLHOME_DATAMODEL_TR_181_ENCAPSULATED");
	}
    }

    if (token_get("CONFIG_RG_DSLHOME") && 
	(token_get("CONFIG_RG_QOS") || token_get("CONFIG_RG_JPKG")))
    {
	token_set_y("CONFIG_RG_TR_098_QOS");
    }

    if (token_get("CONFIG_RG_DSLHOME") && 
	(token_get("CONFIG_RG_ATA") || token_get("CONFIG_RG_HOME_PBX") ||
	(token_get("CONFIG_RG_JPKG") && token_get("CONFIG_RG_VOIP"))))
    {
	token_set_y("CONFIG_RG_TR_104");
    }

    if (token_get("CONFIG_RG_DSLHOME") && 
	(token_get("CONFIG_RG_DISK_MNG") || token_get("CONFIG_RG_JPKG")))
    {
	token_set_y("CONFIG_RG_TR_140");
    }
    
    if (token_get("CONFIG_RG_DSLHOME_VOUCHERS"))
    {
	assert_dep("CONFIG_RG_DSLHOME_VOUCHERS", "CONFIG_RG_DSLHOME", NULL);
	token_set_y("CONFIG_RG_OPTION_MANAGER");
    }

    if (token_get("CONFIG_RG_OPTION_MANAGER"))
    {
	token_set_y("CONFIG_RG_XMLSEC");
	token_set_y("CONFIG_RG_XML2");
    }

    if (token_get("MODULE_RG_MAIL_FILTER"))
        token_set_y("CONFIG_RG_MAIL_FILTER");

    if (token_get("MODULE_RG_ANTIVIRUS_LAN_PROXY"))
    {
	token_set_y("CONFIG_RG_ANTIVIRUS");
	token_set_y("CONFIG_RG_ANTIVIRUS_LAN_PROXY");
	if (!token_get("CONFIG_RG_ANTIVIRUS_VENDOR_ID"))
	{
	    token_set("CONFIG_RG_ANTIVIRUS_VENDOR_ID", "8000");
	    token_set("CONFIG_RG_ANTIVIRUS_VENDOR_NAME", "Jungo");
	}
    }
    if (token_get("MODULE_RG_ANTIVIRUS_NAC"))
    {
	token_set_y("CONFIG_RG_ANTIVIRUS");
	token_set_y("CONFIG_RG_ANTIVIRUS_NAC");
	token_set_y("CONFIG_RG_XML");
    }

    if (token_get("MODULE_RG_ZERO_CONFIGURATION_NETWORKING"))
	token_set_y("CONFIG_ZERO_CONFIGURATION");
    
    if (token_get("MODULE_RG_TR_064"))
        token_set_y("CONFIG_RG_TR_064");

    if (token_get("MODULE_RG_OSGI"))
    {
        token_set_y("CONFIG_RG_OSGI");
        token_set_y("CONFIG_RG_PROSYST_MBS");
    }

    if (token_get("MODULE_RG_JVM") || token_get("MODULE_RG_OSGI"))
    {
        token_set_y("CONFIG_RG_LIBTOOL");
        token_set_y("CONFIG_RG_ZZIPLIB");
        token_set_y("CONFIG_RG_CLASSPATH");
        token_set_y("CONFIG_RG_KAFFE");
    }

    if (token_get("MODULE_RG_VAS_CLIENT"))
        token_set_y("CONFIG_RG_VAS_CLIENT");

    if (token_get("CONFIG_RG_VAS_CLIENT"))
    {
	token_set_y("CONFIG_RG_VAS_WEB_SERVICE");
	token_set_y("CONFIG_RG_WEB_CIFS");
	enable_module("MODULE_RG_SSL_VPN");
	token_set_y("CONFIG_RG_LAN_HOST_LINK_MONITOR");
    }

    if (token_get("MODULE_RG_SSL_VPN"))
    {
        token_set_y("CONFIG_RG_SSL_VPN");
        token_set_y("CONFIG_RG_SSL_VPN_TELNET");
        token_set_y("CONFIG_RG_SSL_VPN_RDP");
        token_set_y("CONFIG_RG_SSL_VPN_FTP");
        token_set_y("CONFIG_RG_SSL_VPN_CIFS");
        token_set_y("CONFIG_RG_SSL_VPN_VNC");
	token_set_y("CONFIG_RG_JNLP");
	/* Web-CIFS adds samba to the distribution, which takes a lot of space
	 * on the flash. Therefore, in distributions with a small flash size, we
	 * avoid adding this feature */
	if (token_get("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION"))
	    token_set_y("CONFIG_RG_WEB_CIFS");
    }

    if (token_get("MODULE_RG_PSE"))
    {
	token_set_m("CONFIG_RG_PSE");
	/* Whomever wants to avoid SOFTPATH needs to manually diable it. */
	token_set_y("CONFIG_RG_PSE_SW");
	token_set_y("CONFIG_RG_PSE_SW_BY_HOOKS");

	if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	{
	    if (token_get("CONFIG_RG_PSE_HW"))
	    {
		conf_err("ERROR: software fastpath suitable for port "
		    "separation will only work when no hardware fastpath "
		    "(CONFIG_RG_PSE_HW=n)\n");
	    }
	    if (token_get("CONFIG_RG_PSE_SW"))
	    {
		/* In case of software PSE, fastpath operates on the switch 
		 * port devices (the virtual devices) and not the physical 
		 * Ethernet device.
		 */
		token_set_y("CONFIG_RG_PSE_SW_SWITCH_PORT");
	    }
        }
    }
    
    /* This is the temporary location for new features which are yet not
     * categorized into a marketing priced module - features should NOT 
     * live here forever!! 
     */
    if (token_get("CONFIG_NEW_NONE_PRICED_FEATURES"))
    {
	token_set_y("CONFIG_RG_PROXY_ARP");
	token_set_y("CONFIG_RG_TFTP_UPGRADE");
	token_set_y("CONFIG_RG_SSI_PAGES");
    }

    if (token_get("CONFIG_RG_IPROUTE2"))
    {
	/* OpenRG is an advanced router by default, just making sure */
	token_set_y("CONFIG_IP_ADVANCED_ROUTER");
	/* Equal cost multipath */
	token_set_y("CONFIG_IP_ROUTE_MULTIPATH");
	/* Verbose route monitoring */
	token_set_y("CONFIG_IP_ROUTE_VERBOSE");
	/* Policy routing */
	token_set_y("CONFIG_IP_MULTIPLE_TABLES");
	token_set_y("CONFIG_FIB_RULES");
	/* Policy routing for IPv6 */
	if (token_get("CONFIG_RG_IPV6"))
	{
	    token_set_y("CONFIG_IPV6_MULTIPLE_TABLES");
	    token_set_y("CONFIG_IPV6_SUBTREES");
	}
	/* Use TOS value as routing key */
	token_set_y("CONFIG_IP_ROUTE_TOS");

	if (token_get("CONFIG_RG_ROUTE_MULTIWAN") || 
	    token_get("CONFIG_RG_JPKG"))
	{
	    /* Routing according DSCP field in IP header */
	    token_set_y("CONFIG_RG_DSCP_POLICY_ROUTING");
	    /* Load balancing */
	    token_set_y("CONFIG_RG_LOAD_BALANCING");
	    /* Fail over */
	    token_set_y("CONFIG_RG_FAILOVER");
	}

	token_set_y("CONFIG_ULIBC_DO_C99_MATH");
    }

    if (token_get("CONFIG_RG_IPROUTE2") && token_get("CONFIG_RG_QOS"))
    {
	token_set_y("CONFIG_TOS_DSCP_RFC2474");
	/* Network packet filtering */
	token_set_y("CONFIG_NETFILTER");
	/* QoS and/or fair queueing */
	token_set_y("CONFIG_NET_SCHED");

	/* HTB packet scheduler */
	token_set_y("CONFIG_NET_SCH_HTB");
	token_set_y("CONFIG_NET_SCH_WRR");
	/* Ingress queueing discipline */
	token_set_y("CONFIG_NET_SCH_INGRESS");
	/* The simplest PRIO pseudoscheduler */
	token_set_y("CONFIG_NET_SCH_PRIO");
	/* The simplest RED pseudoscheduler */
	token_set_y("CONFIG_NET_SCH_RED");
	/* The simplest SFQ pseudoscheduler */
	token_set_y("CONFIG_NET_SCH_SFQ");
	/* Diffserv field marker */
	token_set_y("CONFIG_NET_SCH_DSMARK");
	/* The simplest FIFO queue */
	token_set_y("CONFIG_NET_SCH_FIFO");
	/* QoS support */
	token_set_y("CONFIG_NET_QOS");
	/* Rate estimator */
	token_set_y("CONFIG_NET_ESTIMATOR");
	/* Packet classifier API */
	token_set_y("CONFIG_NET_CLS");
	/* Traffic policing */
	token_set_y("CONFIG_NET_CLS_POLICE");
	/* TC index classifier */
	token_set_y("CONFIG_NET_CLS_TCINDEX");
	/* Firewall based classifier */
	token_set_y("CONFIG_NET_CLS_FW");
	/* U32 classifier */
	token_set_y("CONFIG_NET_CLS_U32");
	/* Special RSVP classifier */
	token_set_y("CONFIG_NET_CLS_RSVP");
	/* Special RSVP classifier for IPv6 */
	token_set_y("CONFIG_NET_CLS_RSVP6");

	/* ATM packet pseudoscheduler */
	if (token_get("CONFIG_ATM"))
	    token_set_y("CONFIG_NET_SCH_ATM");

	/* Use netfilter MARK value as routing key */
	token_set_y("CONFIG_IP_ROUTE_FWMARK");

	/* ingress qdisc support */
	token_set_m("CONFIG_RG_QOS_INGRESS");

	if (token_get("CONFIG_RG_VLAN_BRIDGE") ||
	    token_get("CONFIG_RG_VLAN_8021Q"))
	{
	    token_set_y("CONFIG_RG_VLAN_DSCP");
	}
    }

    /* XXX: B83867 - Need to create jrms_features(). This function and friends
     * should not be called at all for jrms.
     */
    if (token_get("CONFIG_JRMS_DEBUG"))
	token_set_y("CONFIG_JRMS_VALGRIND");
    
    /* ============= END of Module definition section ============= */

    if (token_get("CONFIG_RG_WEB_CIFS") || token_get("CONFIG_SMB_FS") ||
	token_get("CONFIG_CIFS"))
    {
	token_set_y("CONFIG_RG_SAMBA");
    }

    if (token_get("CONFIG_LSP_DIST"))
	token_set_y("CONFIG_RG_BUSYBOX");

    if (token_get("CONFIG_IPTABLES"))
    {
	token_set_y("CONFIG_NETFILTER");
	token_set_y("CONFIG_IP_NF_FILTER");
	token_set_y("CONFIG_IP_NF_IPTABLES");
	token_set_y("CONFIG_IP_NF_CONNTRACK");
	token_set_y("CONFIG_IP_NF_NAT_NEEDED");
	token_set_y("CONFIG_IP_NF_NAT");
	token_set_y("CONFIG_IP_NF_NAT_LOCAL");
	token_set_y("CONFIG_IP_NF_TARGET_MASQUERADE");
	token_set_y("CONFIG_IP_NF_CT_ACCT");
	token_set_y("CONFIG_IP_NF_CONNTRACK_MARK");
	token_set_y("CONFIG_NETFILTER_XTABLES");
	token_set_y("CONFIG_NETFILTER_XT_TARGET_CLASSIFY");
	token_set_y("CONFIG_NETFILTER_XT_TARGET_CONNMARK");
	token_set_y("CONFIG_NETFILTER_XT_TARGET_MARK");
	token_set_y("CONFIG_NETFILTER_XT_TARGET_NFQUEUE");
	token_set_y("CONFIG_NETFILTER_XT_TARGET_NOTRACK");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_COMMENT");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_CONNBYTES");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_CONNMARK");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_CONNTRACK");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_DCCP");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_HELPER");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_LENGTH");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_LIMIT");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_MAC");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_MARK");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_PKTTYPE");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_REALM");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_SCTP");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_STATE");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_STRING");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_TCPMSS");
	token_set_y("CONFIG_NETFILTER_XT_MATCH_PHYSDEV");
	token_set_y("CONFIG_NET_CLS_ROUTE");
	token_set_y("CONFIG_BRIDGE_NETFILTER");
	token_set_y("CONFIG_TEXTSEARCH");
    }
    
    if (!token_get_str("CONFIG_RG_CONSOLE_DEVICE"))
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");
    
    if (token_get("CONFIG_RG_RADIUS_LOGIN_AUTH") ||
	token_get("CONFIG_RG_8021X_RADIUS"))
    {
	token_set_y("CONFIG_RG_RADIUS_CLIENT");
    }

    if (token_get("MODULE_RG_RADIUS_SERVER"))
    {
	token_set_y("CONFIG_RG_RADIUS_SERVER");
	token_set_y("CONFIG_RG_OPENSSL");
    }

    if (token_get("CONFIG_RG_PRINT_SERVER"))
    {
	token_set_y("CONFIG_HOTPLUG");

	if (token_get("CONFIG_RG_UML"))
	    token_set_y("CONFIG_RG_PRINT_SERVER_PRINTER_NULL");
	else
	    token_set_y("CONFIG_USB_PRINTER");

	if (!token_get("CONFIG_RG_PRINT_SERVER_SPOOL"))
	    token_set("CONFIG_RG_PRINT_SERVER_SPOOL", "131072"); /* 128k */
    }

    if (!token_get("CONFIG_RG_DEV") && token_get("CONFIG_RG_KOS_KNET"))
	token_set_m("CONFIG_RG_ONE_MODULE");
    /* debug. kernel larger and slower */
    if (token_get("CONFIG_RG_DEV"))
    {
	if (!token_get("CONFIG_ARMNOMMU"))
	    token_set_y("CONFIG_FRAME_POINTER");
	token_set_y("CONFIG_KALLSYMS");
	token_set_y("CONFIG_RG_DEBUG_KMALLOC");
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_DEBUG_SPINLOCK_SLEEP");
	    token_set_y("CONFIG_DEBUG_INFO");
	}
	token_set_y("CONFIG_RG_IPROUTE2_UTILS");
	token_set_y("CONFIG_RG_WIRELESS_TOOLS"); 		
	token_set_y("CONFIG_RG_KGDB");
    }

    if (token_get("CONFIG_RG_BRIDGE") || token_get("CONFIG_HW_SWITCH_LAN") ||
	token_get("CONFIG_HW_SWITCH_WAN"))
    {
	token_set_m("CONFIG_RG_SWITCH");
    }

    if (IS_DIST("JPKG_I386"))
    {
	token_set("ARCH", "i386");
	token_set("CONFIG_X86_L1_CACHE_SHIFT", "4");
	token_set("LIBC_ARCH", "i386");
	token_set_y("CONFIG_M386");
	token_set_y("CONFIG_FLASH_IN_FILE");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_DYN_LINK"); /* XXX we need also to support static */
	set_big_endian(0);
	token_set_y("CONFIG_HW_CLOCK");
	token_set_y("CONFIG_PROC_FS");
	token_set_y("CONFIG_EXT2_FS");
	token_set_y("CONFIG_BINFMT_ELF");
    }

    if (IS_DIST("JPKG_UML"))
    {
	token_set("ARCH", "um");
	token_set("CONFIG_X86_L1_CACHE_SHIFT", "4");
	token_set_y("CONFIG_RG_GDBSERVER");
	token_set_m("CONFIG_RG_KRGLDR");
	token_set("LIBC_ARCH", "i386");
	token_set_y("CONFIG_M386");
	token_set_y("CONFIG_FLASH_IN_FILE");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_DYN_LINK"); /* XXX we need also to support static */
	set_big_endian(0);
	token_set_y("CONFIG_HW_CLOCK");
	token_set_y("CONFIG_PROC_FS");
	token_set_y("CONFIG_EXT2_FS");
	token_set_y("CONFIG_BINFMT_ELF");
   }

    /* Platforms */
    if (token_get("CONFIG_RG_UML"))
    {
	token_set("BOARD", "UML");
	token_set_m("CONFIG_RG_KRGLDR");
	token_set_y("CONFIG_RG_GDBSERVER");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");

	if (token_get("CONFIG_VALGRIND"))
	    token_set_y("CONFIG_UML_BIG_FLASH");

	if ((os && !strcmp(os, "LINUX_26")))
	{
	    token_set_y("CONFIG_UID16");
	    token_set_y("CONFIG_SYSVIPC");
	    token_set_y("CONFIG_CLEAN_COMPILE");
	    token_set_y("CONFIG_BROKEN_ON_SMP");
	    token_set_y("CONFIG_BASE_FULL");
	    token_set_y("CONFIG_FUTEX");
	    token_set_y("CONFIG_RT_MUTEXES");
	    token_set_y("CONFIG_PLIST");
	    token_set_y("CONFIG_EPOLL");
	    token_set_y("CONFIG_X86_XADD");
	    token_set_y("CONFIG_BINFMT_MISC");
	    token_set_y("CONFIG_STANDALONE");
	    token_set_y("CONFIG_PREVENT_FIRMWARE_BUILD");
	    token_set("CONFIG_BLK_DEV_RAM_COUNT", "16");
	    token_set("CONFIG_NET_IPGRE", "m");
	    token_set_y("CONFIG_NET_IPGRE_BROADCAST");
	    token_set_y("CONFIG_ARCH_HAS_SC_SIGNALS");
	    token_set("CONFIG_TOP_ADDR", "0xc0000000");
	    token_set_y("CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA");
	    token_set_y("CONFIG_LD_SCRIPT_STATIC");
	    token_set_y("CONFIG_UML_REAL_TIME_CLOCK");
	    token_set_y("CONFIG_NOCONFIG_CHAN");
	    token_set_y("CONFIG_BLK_DEV_COW_COMMON");
	    token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	    token_set_y("CONFIG_QUOTA");
	    token_set_y("CONFIG_QUOTACTL");
	    token_set_y("CONFIG_RG_TTYP");
            token_set("CONFIG_X86_L1_CACHE_SHIFT", "4");

	    /* linux-2.6.16.14 */
	    token_set_y("CONFIG_SWAP");
	    token_set_y("CONFIG_IRQ_RELEASE_METHOD");
	    token_set("UML_CONFIG_STUB_DATA", "0xbffff000");
	    token_set("CONFIG_STUB_DATA", "0xbffff000");
	    token_set("CONFIG_STUB_START", "0xbfffe000");
	    token_set("CONFIG_STUB_CODE", "0xbfffe000");

	    token_set_y("CONFIG_M386");
	    token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	    token_set_y("CONFIG_X86_PPRO_FENCE");
	    token_set_y("CONFIG_X86_F00F_BUG");
	    token_set_y("CONFIG_SEMAPHORE_SLEEPERS");
	    token_set_y("CONFIG_SELECT_MEMORY_MODEL");
	    token_set_y("CONFIG_FLATMEM_MANUAL");
	    token_set_y("CONFIG_FLATMEM");
	    token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	    token_set_y("CONFIG_FORCED_INLINING");
	    token_set_y("CONFIG_UID16");
	    token_set_y("CONFIG_CC_OPTIMIZE_FOR_SIZE");
	    token_set_y("CONFIG_DEBUG_MUTEXES");
	    token_set_y("CONFIG_DEFAULT_AS");
	    token_set_y("CONFIG_DETECT_SOFTLOCKUP");
	    token_set_y("CONFIG_INET_DIAG");
	    token_set_y("CONFIG_INET_TCP_DIAG");
	    token_set_y("CONFIG_IP_FIB_HASH");
	    token_set_y("CONFIG_SLAB");
	    token_set_y("CONFIG_TCP_CONG_BIC");

	    token_set("CONFIG_DEFAULT_IOSCHED", "\"anticipatory\"");
	    token_set("CONFIG_X86_L1_CACHE_SHIFT", "4");
	    token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");

	    token_set_y("CONFIG_GENERIC_HWEIGHT");
	    token_set_y("CONFIG_BITREVERSE");
	}
    }

    if (token_get("CONFIG_RG_UML"))
    {
	token_set("LIBC_ARCH", "i386");
	token_set_y("CONFIG_M386");
	token_set_y("CONFIG_FLASH_IN_FILE");
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_HAS_MMU");
	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_DYN_LINK");
	set_big_endian(0);
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_HW_CLOCK");
	token_set_y("CONFIG_PROC_FS");
	token_set_y("CONFIG_EXT2_FS");
    }

    if (!token_get("CONFIG_RELEASE")) 
    {
	token_set_y("CONFIG_DEF_KEYS");
    }

    if (token_get("CONFIG_MPC8272ADS"))
    {
	/* Generic PPC configs */

	token_set_y("CONFIG_PPC");

	/* Platform options */

	token_set_y("CONFIG_CPM2");
	token_set_y("CONFIG_8272");
	token_set_y("CONFIG_8260");
	token_set_y("CONFIG_PQ2ADS");
	token_set_y("CONFIG_ADS8272");
	token_set_y("CONFIG_HIGHMEM");

	/* MTD */

	token_set_y("CONFIG_RG_MTD");
	token_set_y("CONFIG_RG_MTD_DEFAULT_PARTITION");
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_I4");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set("CONFIG_MTD_PHYSMAP_BANKWIDTH", "4");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x800000");
	token_set("CONFIG_MTD_PHYSMAP_START", "0xFF800000");
	
	/* Serial drivers */

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_CPM");
	token_set_y("CONFIG_SERIAL_CPM_SCC1");
	token_set_y("CONFIG_SERIAL_CPM_SCC4");
	token_set_y("CONFIG_SERIAL_CPM_CONSOLE");

	/* BUS */

	token_set_y("CONFIG_PCI_8260");

	/* RamDisk/CRAMFS/FS options */

	token_set_y("CONFIG_RG_INITFS_RAMFS");

	if (!token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_RG_MODFS_CRAMFS");

	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");

	token_set_y("CONFIG_HZ_250");
	token_set("CONFIG_HZ", "250");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");

	if (token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_LSP_FLASH_LAYOUT");
    }

    if (token_get("CONFIG_EP8248"))
    {
	static char cmd_line[255];

	/* Build */

	set_big_endian(1);
	token_set("ARCH", "ppc");
	token_set("LIBC_ARCH", "powerpc");
	token_set_y("CONFIG_CPU_BIG_ENDIAN");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_ULIBC_SHARED");
	token_set_y("CONFIG_DYN_LINK");
	token_set_y("CONFIG_BOOTLDR_UBOOT");

	/* Command line */
	
	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE")); 
	token_set("CONFIG_CMDLINE", cmd_line);
	
	/* Advanced setup */

	token_set("CONFIG_TASK_SIZE", "0x80000000");
	token_set("CONFIG_KERNEL_START", "0xc0000000");
	token_set("CONFIG_LOWMEM_SIZE", "0x30000000");
	token_set("CONFIG_HIGHMEM_START", "0xfe000000");

	/* Platform options */

	token_set_y("CONFIG_CPM2");
	token_set_y("CONFIG_8272");
	token_set_y("CONFIG_EP8248");
	token_set_y("CONFIG_8260");

	/* Processor */

	token_set_y("CONFIG_PPC_STD_MMU");
	token_set_y("CONFIG_6xx");

	/* Code maturity level options */

	token_set_y("CONFIG_PPC32");
	token_set_y("CONFIG_PPC");
	token_set_y("CONFIG_HAVE_DEC_LOCK");
	token_set_y("CONFIG_RWSEM_XCHGADD_ALGORITHM");
	token_set_y("CONFIG_PPC_FPU");

	/* MTD */
	
	token_set_y("CONFIG_RG_MTD");
	token_set_y("CONFIG_RG_MTD_DEFAULT_PARTITION");
	token_set_y("CONFIG_MTD");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x800000");
	token_set("CONFIG_MTD_PHYSMAP_START", "0xFF800000");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");

	/* Serial drivers */

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_CPM");
	token_set_y("CONFIG_SERIAL_CPM_SMC1");
	token_set_y("CONFIG_SERIAL_CPM_CONSOLE");

	/* General_setup */

	token_set("CONFIG_CC_ALIGN_JUMPS", "0");
	token_set("CONFIG_CC_ALIGN_LOOPS", "0");
	token_set("CONFIG_CC_ALIGN_LABELS", "0");
	token_set("CONFIG_CC_ALIGN_FUNCTIONS", "0");

	/* RamDisk/CRAMFS/FS options */

	token_set_y("CONFIG_RG_INITFS_RAMFS");

	if (!token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_RG_MODFS_CRAMFS");

	/* Flash layout */

	if (token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_LSP_FLASH_LAYOUT");
    }
    
    if (token_get("CONFIG_MPC8349_ITX"))
    {
	/* Generic PPC configs */
	token_set_y("CONFIG_PPC");
	token_set("CONFIG_BOOTLDR_UBOOT_COMP", "gzip");

	/* General ARCH */
	token_set_y("CONFIG_GENERIC_NVRAM");
	token_set_y("CONFIG_SCHED_NO_NO_OMIT_FRAME_POINTER");
	
	/* Processor */
	token_set_y("CONFIG_PPC_GEN550");

	/* Platform options */
	token_set_y("CONFIG_MPC8349_ITX");
	token_set_y("CONFIG_83xx");
	token_set_y("CONFIG_PPC_83xx");
	token_set_y("CONFIG_MPC834x");
	
	token_set_y("CONFIG_HZ_250");
	token_set("CONFIG_HZ", "250");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	
	/* Exists in Linux-2.6.12 but PPC compilation fails if it is on,
	 * taken from Freescale Linux-2.6.13 .config */
	token_set_y("CONFIG_SECCOMP");
	
	token_set_y("CONFIG_ISA_DMA_API");

	/* Bus */
	token_set_y("CONFIG_MPC83xx_PCI2");

	/* UART */

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "4");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "4");

	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");

	/* RamDisk/CRAMFS/FS options */

	token_set_y("CONFIG_RG_INITFS_RAMFS");

	if (token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_LSP_FLASH_LAYOUT");
	else
	    token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }

    if (token_get("CONFIG_PPC"))
    {
	/* Note: These may be not the generic PPC configs,
	 * but the only PPC we have now are FreeScale(s).
	 * TODO: divide this section into PPC generic, etc..
	 * (e.g. CONFIG_6xx or CONFIG_CPU_BIG_ENDIAN may not be 
	 * correct for any PPC)
	 */
	static char cmd_line[255];

	/* Build */

	set_big_endian(1);
	token_set("ARCH", "ppc");
	token_set("LIBC_ARCH", "powerpc");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_ULIBC_SHARED");
	token_set_y("CONFIG_DYN_LINK");
	token_set_y("CONFIG_BOOTLDR_UBOOT");

	/* Advanced setup */

	token_set("CONFIG_TASK_SIZE", "0x80000000");
	token_set("CONFIG_KERNEL_START", "0xc0000000");
	token_set("CONFIG_LOWMEM_SIZE", "0x30000000");
	token_set("CONFIG_HIGHMEM_START", "0xfe000000");

	/* General ARCH */

	token_set_y("CONFIG_RWSEM_XCHGADD_ALGORITHM");
	token_set_y("CONFIG_PPC32");
	
	/* Processor */

	token_set_y("CONFIG_PPC_STD_MMU");
	token_set_y("CONFIG_6xx");
	token_set_y("CONFIG_PPC_FPU");

	/* Bus */

	token_set_y("CONFIG_PCI");

	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    /* General */

	    token_set_y("CONFIG_OBSOLETE_INTERMODULE");

	    /* Bus */

	    token_set_y("CONFIG_PCI_DOMAINS");
	    token_set_y("CONFIG_PPC_INDIRECT_PCI");

	    /* GCC setup */

	    token_set("CONFIG_CC_ALIGN_JUMPS", "0");
	    token_set("CONFIG_CC_ALIGN_LOOPS", "0");
	    token_set("CONFIG_CC_ALIGN_LABELS", "0");
	    token_set("CONFIG_CC_ALIGN_FUNCTIONS", "0");
	}

	/* Command line */

	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE")); 

	token_set("CONFIG_CMDLINE", cmd_line);
    }

    if (token_get("CONFIG_BCM947_COMMON"))
    {
	set_big_endian(0);
	token_set("CONFIG_BLK_DEV_RAM_SIZE", "8000");
	token_set_y("CONFIG_BLK_DEV_LOOP");
	token_set("CONFIG_CMDLINE", 
	    "console=ttyS0,115200 root=/dev/ram0 rw");
	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_NONCOHERENT_IO");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_MIPS_BRCM");
	token_set_y("CONFIG_BCM947XX");
	token_set_y("CONFIG_BCM947");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_SERIAL");
	token_set_y("CONFIG_SERIAL_CONSOLE");
	token_set_y("CONFIG_SYSCTL");
	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set("ENDIANESS_SUFFIX", "el30");
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_BCM4310");
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_NEW_TIME_C");
	token_set_y("CONFIG_NEW_IRQ");
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set_y("CONFIG_KCORE_ELF");
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_ADV_OPTIONS");
	token_set_y("CONFIG_MTD_CFI_NOSWAP");
	token_set_y("CONFIG_MTD_CFI_GEOMETRY");
	token_set_y("CONFIG_MTD_CFI_B2");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set_y("CONFIG_MTD_BCM947XX");
	token_set_m("CONFIG_IL");
	token_set_y("CONFIG_IL_47XX");
	token_set_m("CONFIG_WL");
	token_set_y("CONFIG_WL_AP");
	token_set_y("CONFIG_NET_PCI");
	token_set_y("CONFIG_NET_RADIO");
	token_set_m("CONFIG_HERMES");
	token_set_y("CONFIG_RAMFS");
	token_set_y("CONFIG_PARTITION_ADVANCED");
	/*token_set("CONFIG_NET_WIRELESS");
	#token_set_m("CONFIG_USB");
	#token_set("CONFIG_USB_DEVICEFS");
	#token_set_m("CONFIG_USB_OHCI");*/
	token_set_y("CONFIG_CROSSCOMPILE");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	token_set_y("CONFIG_CRAMFS_FS");
	token_set_y("CONFIG_RG_MODFS");
	/* Uncompress is done by CFE/PMON which does not support other methods
	 */
	token_set("CONFIG_RG_KERNEL_COMP_METHOD", "gzip");
    }
    
    if (token_get("CONFIG_BCM963XX_COMMON"))
    {
	static char cmd_line[255];

	/* Linux/Build generic */
	set_big_endian(1);
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_MIPS32");

	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_DMA_NONCOHERENT");
	    token_set_y("CONFIG_TRAD_SIGNALS");
	    token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "5");
	    token_set_y("CONFIG_PAGE_SIZE_4KB");
	    token_set("CONFIG_LOG_BUF_SHIFT", "14");
	    token_set_y("CONFIG_RG_TTYP");
	    token_set_y("CONFIG_CC_OPTIMIZE_FOR_SIZE");
	    token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	    token_set_y("CONFIG_32BIT");
	    token_set_y("CONFIG_BUILD_ELF32");
	    token_set_y("CONFIG_CPU_MIPS32_R1");
	    token_set_y("CONFIG_FLATMEM");
	    token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	    token_set_y("CONFIG_BOOT_ELF32");
	    token_set_y("CONFIG_LINUX_SKB_REUSE");
	    token_set("CONFIG_HZ", "200");
	}

	if (token_get("CONFIG_RG_OS_LINUX_24"))
	{
	    token_set_y("CONFIG_BINFMT_ELF");
	    token_set_y("CONFIG_SYSCTL");
	    token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	    token_set_y("CONFIG_UNIX98_PTYS");
	    token_set_y("CONFIG_NONCOHERENT_IO");
	    token_set_y("CONFIG_NEW_TIME_C");
	    token_set_y("CONFIG_NEW_IRQ");
	    token_set_y("CONFIG_SERIAL");
	    token_set_y("CONFIG_SERIAL_CONSOLE");

	    /* RamDisk/CRAMFS/FS options */
	    token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	    token_set_y("CONFIG_RG_MODFS");
	    token_set_y("CONFIG_CRAMFS_FS");
	    token_set("CONFIG_BLK_DEV_RAM_SIZE", "8000");
	    if (!IS_DIST("GTWX5803"))
		token_set("CONFIG_RAMDISK_SIZE", "512");
	    token_set_y("CONFIG_RAMFS");
	}
	
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set_y("CONFIG_KCORE_ELF");
	token_set_y("CONFIG_CROSSCOMPILE");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	
	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_MIPS_BRCM");
	token_set_y("CONFIG_BCM963XX");
	
	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    token_set_y("CONFIG_DYN_LINK");
	    /* Loop devices */
	    token_set_y("CONFIG_BLK_DEV_LOOP");
	}

	if (!token_get_str("CONFIG_SDRAM_SIZE"))
	{
	    token_set("CONFIG_SDRAM_SIZE",
		token_get_str("CONFIG_BCM963XX_SDRAM_SIZE"));
	}
	
	/* Command line */
	
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	{
	    sprintf(cmd_line, "console=ttyS0,115200 root=/dev/ram0 rw %s",
		token_get("CONFIG_RG_RGLOADER") ? "quiet" : ""); 
	    token_set("CONFIG_CMDLINE", cmd_line);
	}
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set("CONFIG_CMDLINE", "console=ttyS0,115200");
	
	/* MTD */
	
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	/* New MTD configs (Linux-2.6) */
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	    token_set_y("CONFIG_MTD_CFI_I1");
	    token_set_y("CONFIG_MTD_CFI_I2");
	    token_set_y("CONFIG_MTD_CFI_UTIL");
	    token_set_y("CONFIG_OBSOLETE_INTERMODULE");
	}
	if (token_get("CONFIG_HW_DSP"))
	    token_set_y("CONFIG_RG_DSP_THREAD");
	    
	if (token_get("CONFIG_ATM"))
	    token_set_y("CONFIG_ATM_VBR_AS_NRT");
    }
  
    if (token_get("CONFIG_BROADCOM_9636X") ||
	token_get("CONFIG_BROADCOM_9338X"))
    {
	static char cmd_line[255];
	static char mtdparts[255];

	set_big_endian(1);
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");
	token_set_y("CONFIG_HAS_MMU");

        if (!token_get("CONFIG_RG_RGLOADER"))
            token_set_y("CONFIG_DYN_LINK");

	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_MIPS_BRCM");

	token_set_y("CONFIG_BRCM_DCACHE_SHARED");
	token_set_y("CONFIG_BCM_BOARD");
	token_set_y("CONFIG_BCM_SERIAL");
	token_set_y("CONFIG_BCM_SERIAL_CONSOLE");
	if (!token_get("CONFIG_BROADCOM_9338X"))
	    token_set_y("CONFIG_BCM_PKTDMA");
	if (token_get("CONFIG_HW_DSL_WAN"))
	{
	    token_set_y("CONFIG_ATM");
	    token_set_m("CONFIG_BCM_ADSL");
	}
	token_set_y("CONFIG_IRQ_CPU");
	token_set_y("CONFIG_DMA_NONCOHERENT");
	token_set_y("CONFIG_DMA_NEED_PCI_MAP_STATE");
	token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "4");
	if (token_get("CONFIG_BCM93383"))
	{
	    token_set_y("CONFIG_CPU_MIPS32_R2");
	    token_set_y("CONFIG_CPU_MIPSR2");
	}
	else
	{
	    token_set_y("CONFIG_CPU_MIPS32_R1");
	    token_set_y("CONFIG_CPU_MIPSR1");
	}
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_32BIT");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_GENERIC_HARDIRQS_NO__DO_IRQ");
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set_y("CONFIG_MIPS_MT_DISABLED");
	token_set_y("CONFIG_GENERIC_IRQ_PROBE");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_SELECT_MEMORY_MODEL");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLATMEM_MANUAL");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_HZ_1000");
	token_set("CONFIG_HZ", "1000");
	token_set_y("CONFIG_BROKEN_ON_SMP");
	token_set_y("CONFIG_TRAD_SIGNALS");
	token_set("CONFIG_LOG_BUF_SHIFT", "14");
	token_set_y("CONFIG_MAGIC_SYSRQ");

	token_set_y("CONFIG_LINUX_SKB_REUSE_BCM9636X");

	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	if (token_get("CONFIG_BROADCOM_9338X"))
	{
	    if (token_get("CONFIG_BCM93380"))
	    {
		token_set_m("CONFIG_BCM_BCMDSP");
		token_set_y("CONFIG_BCM_LOT1");
		token_set_y("CONFIG_BCM_VECTOR");
		token_set_y("CONFIG_BCM_UPPER_MEM");
	    }
	    else if (token_get("CONFIG_BCM93383"))
		token_set_y("CONFIG_BCM_APROUTER");
	    token_set_y("CONFIG_BCM_PLATFORM_DEVS");
	}
	if (token_get("CONFIG_HW_FLASH_NOR"))
	{
	    token_set_y("CONFIG_MTD_CFI_I1");
	    token_set_y("CONFIG_MTD_CFI_I2");
	    token_set_y("CONFIG_MTD_CFI");
	    token_set_y("CONFIG_MTD_CFI_AMDSTD");
	    token_set_y("CONFIG_MTD_GEN_PROBE");
	    token_set_y("CONFIG_MTD_CFI_UTIL");
	    token_set_y("CONFIG_MTD_BCM963XX");
	}
	token_set_y("CONFIG_MTD_COMPLEX_MAPPINGS");
	token_set_y("CONFIG_MTD_SLRAM");	
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	if (token_get("CONFIG_HW_FLASH_NAND"))
	{
	    token_set_y("CONFIG_MTD_BRCMNAND");
	    token_set("CONFIG_MTD_BRCMNAND_VERSION", "4");
	    token_set("CONFIG_MTD_NAND", "n");
	    token_set("CONFIG_MTD_NAND_IDS", "n");
	    token_set_y("CONFIG_MTD_CHAR");
	    token_set_y("CONFIG_MTD_BLOCK");
	}
	
	token_set_y("CONFIG_SPI");
	token_set_y("CONFIG_SPI_MASTER");

        if (!token_get("CONFIG_BROADCOM_9338X") &&
	    !token_get("CONFIG_RG_RGLOADER"))
        {
            token_set_y("CONFIG_PCI");
            token_set_y("CONFIG_PCIEPORTBUS");
            token_set_y("CONFIG_BCM_PCI");
        }

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");

	sprintf(cmd_line, "console=ttyS0,115200 ");
	if (token_get("CONFIG_HW_BCM96362"))
	{
	    sprintf(mtdparts + strlen(mtdparts),
		"bcm63xx-nand.0:0x02800000@0x01700000(%s),"
		"0x01400000@0x200000(JFFS2) ",
		token_get_str("CONFIG_RG_UBI_PARTITION"));
	}

	token_set("CONFIG_RG_CMDLINE_MTDPARTS", mtdparts);
	token_set("CONFIG_CMDLINE", cmd_line);

	if (token_get("CONFIG_ATM"))
	    token_set_m("CONFIG_BCM_XTMCFG");
    }

    if (token_get("CONFIG_AR9") || token_get("CONFIG_VR9"))
    {
	static char mtdparts[255];

	/* Linux/Build generic */
	set_big_endian(1);
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");

	token_set_y("CONFIG_LANTIQ_XWAY");

	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_MIPS32_R2");
	token_set_y("CONFIG_CPU_MIPSR2");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_CPU_MIPSR2_IRQ_VI");
	token_set_y("CONFIG_CPU_MIPSR2_IRQ_EI");
	token_set_y("CONFIG_CPU_MIPSR2_SRS");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set_y("CONFIG_HZ_100");
	token_set("CONFIG_HZ", "100");
	token_set_y("CONFIG_DMA_NONCOHERENT");
	token_set_y("CONFIG_DMA_NEED_PCI_MAP_STATE");
	token_set_y("CONFIG_BOOT_ELF32");
	token_set_y("CONFIG_IRQ_CPU");
	token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "5");

	token_set_y("CONFIG_MIPS_MT_DISABLED");
	token_set_y("CONFIG_MIPS_MT");
	token_set_y("CONFIG_PROC_FS");	
	token_set_y("CONFIG_MTSCHED");
	token_set_y("CONFIG_MIPS_VPE_LOADER");
	token_set_y("CONFIG_MIPS_VPE_LOADER_TOM");
	token_set_y("CONFIG_IFX_VPE_EXT");
	token_set_y("CONFIG_SOFT_WATCHDOG_VPE");
	token_set_y("CONFIG_WATCHDOG");

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_IFX_ASC");
	token_set_y("CONFIG_SERIAL_IFX_ASC_CONSOLE");
	token_set("CONFIG_IFX_ASC_DEFAULT_BAUDRATE", "115200");
	token_set_y("CONFIG_IFX_ASC_CONSOLE_ASC1");
	token_set_y("CONFIG_MAGIC_SYSRQ");

	token_set_y("CONFIG_LTQ");
	token_set_y("CONFIG_32BIT");
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set("CONFIG_IFX_DMA_DESCRIPTOR_NUMBER", "64");
	token_set_y("CONFIG_IFX_CLOCK_CHANGE");
	token_set_y("CONFIG_LTQ_ADDON");
	token_set_y("CONFIG_IFX_PMCU");
	token_set_y("CONFIG_IFX_CPUFREQ");
	if (token_get("CONFIG_AR9"))
	{
            if (token_get("CONFIG_HW_DSL_WAN"))
                token_set_y("CONFIG_IFXMIPS_DSL_CPE_MEI");
	}
	else
	{
	    token_set_y("CONFIG_IFX_DCDC");
            if (token_get("CONFIG_HW_VDSL_WAN"))
                token_set_y("CONFIG_DSL_MEI_CPE_DRV");
	}
	token_set_y("CONFIG_IFX_GPIO");
	token_set_y("CONFIG_IFX_PMU");
	token_set_y("CONFIG_IFX_RCU");

	if (token_get("CONFIG_IFX_PTM"))
	    token_set_y("CONFIG_IFX_PTM_RX_NAPI");
#if 0
	token_set_y("CONFIG_IFX_LED");
	token_set_y("CONFIG_IFX_LEDC");
	token_set_y("CONFIG_IFX_WDT");
#endif

	token_set_y("CONFIG_SOFT_FLOAT");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_GENERIC_IRQ_PROBE");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_SCHED_NO_NO_OMIT_FRAME_POINTER");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_BROKEN_ON_SMP");
	token_set_y("CONFIG_BLK_DEV_LOOP");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_TRAD_SIGNALS");
	
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_SLRAM");	
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	if (token_get("CONFIG_HW_FLASH_NOR"))
	{
	    token_set_y("CONFIG_MTD_CFI_I1");
	    token_set_y("CONFIG_MTD_CFI_I2");
	    token_set_y("CONFIG_MTD_CFI_INTELEXT");
	    token_set_y("CONFIG_MTD_CFI_AMDSTD");
	    token_set_y("CONFIG_MTD_CFI_UTIL");
	    token_set_y("CONFIG_MTD_IFX_NOR");
	}
	token_set_y("CONFIG_MTD_COMPLEX_MAPPINGS");
	if (token_get("CONFIG_HW_FLASH_NAND"))
	    token_set_y("CONFIG_MTD_IFX_NAND");

	token_set_y("CONFIG_SPI");
	token_set_y("CONFIG_SPI_MASTER");
	token_set_y("CONFIG_IFX_SPI");
	token_set_y("CONFIG_IFX_SPI_ASYNCHRONOUS");
	
	if (token_get("CONFIG_AR9"))
	    token_set_y("CONFIG_MII1_AUTONEG");
	
	if (token_get("CONFIG_USB"))
	{
	    token_set_m("CONFIG_USB_HOST_IFX");
	    token_set_y("CONFIG_USB_HOST_IFX_B");
	    token_set_y("CONFIG_USB_HOST_IFX_UNALIGNED_ADJ");
	}

	if (token_get("CONFIG_PCI"))
	{
	    token_set_y("CONFIG_IFX_PCI");
	    token_set_y("CONFIG_IFX_PCI_HW_SWAP");
	    token_set("CONFIG_IFX_PCI_CLOCK_DELAY_NANO_SECONDS", "1");
	    token_set("CONFIG_IFX_PCI_CLOCK_DELAY_TENTH_NANO_SECOND", "0");
	}
	if (token_get("CONFIG_PCIEPORTBUS"))
	{
	    token_set_y("CONFIG_IFX_PCIE");
	    token_set_y("CONFIG_IFX_PCIE_HW_SWAP");
	    token_set_y("CONFIG_IFX_PCIE_1ST_CORE");

	    token_set_y("CONFIG_PCI_MSI");
	}	    

        if (token_get("CONFIG_HW_DSL_WAN") || token_get("CONFIG_HW_VDSL_WAN"))
            token_set_m("CONFIG_IFX_DSL_API");

	if (token_get("CONFIG_IFX_PPA"))
	{
	    token_set_y("CONFIG_IFX_PPA_API_DIRECTPATH");
            token_set_y("CONFIG_IFX_PPA_API_DIRECTPATH_BRIDGING");
	}
	if (token_get("CONFIG_ATM"))
	    token_set_y("CONFIG_ATM_VBR_AS_NRT");

	if (!token_get("CONFIG_RG_RGLOADER"))
            token_set_y("CONFIG_DYN_LINK");

	if (token_get_str("CONFIG_RG_UBI_PARTITION") &&
	    token_get("CONFIG_RG_UBI_PARTITION_SIZE") &&
	    token_get("CONFIG_RG_UBI_PARTITION_OFFSET"))
	{
	    sprintf(mtdparts, "ifx_nand:0x%08x@0x%08x(%s)",
		token_get("CONFIG_RG_UBI_PARTITION_SIZE"),
		token_get("CONFIG_RG_UBI_PARTITION_OFFSET"),
		token_get_str("CONFIG_RG_UBI_PARTITION"));
	}

	if (*mtdparts)
	    token_set("CONFIG_RG_CMDLINE_MTDPARTS", mtdparts);

	if (!token_get("CONFIG_RG_LANTIQ_XWAY_DYN_DATAPATH"))
	    token_set_y("CONFIG_RG_LANTIQ_XWAY_STATIC_DATAPATH");
    }
    if (token_get("CONFIG_P400_REF"))
    {
	token_set_y("CONFIG_BOOTLDR_UBOOT");
	token_set_y("CONFIG_LOADADDR_SYM_TEXT");

	token_set_y("CONFIG_EXTEND_IRQ_VECTOR");

    	/* Flash/MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CHAR");
	token_set_y("CONFIG_MTD_BLOCK");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");

	/* Kernel Configs */
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set_y("CONFIG_BROKEN_ON_SMP");
	token_set_y("CONFIG_LOCK_KERNEL");
        token_set_y("CONFIG_CLEAN_COMPILE");
        token_set_y("CONFIG_SWAP");
        token_set_y("CONFIG_KALLSYMS");
        token_set_y("CONFIG_EPOLL");
	token_set_y("CONFIG_FUTEX");
        token_set_y("CONFIG_BLK_DEV_INITRD");
	token_set("CONFIG_BLK_DEV_RAM_COUNT", "16");
	token_set_y("CONFIG_BLK_DEV_RAM");
        token_set_y("CONFIG_CRAMFS");
        token_set_y("CONFIG_DMA_NONCOHERENT");
	token_set_y("CONFIG_INPUT");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_TRAD_SIGNALS");
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");
	token_set_y("CONFIG_HAS_MMU");

	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");

	/* I can't explain this one... */
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
 
	token_set_y("CONFIG_ZLIB_INFLATE");
	token_set_y("CONFIG_ZLIB_DEFLATE");

	/* Ramdisk */
	token_set_y("CONFIG_EMBEDDED_RAMDISK");

	/* File Systems */	
	token_set_y("CONFIG_EXT2_FS");

	/* Encryption */
        token_set_y("CONFIG_CRYPTO_HASH");
        token_set_y("CONFIG_CRYPTO_ALGAPI");
	token_set_y("CONFIG_CRYPTO_AES");
        token_set_y("CONFIG_CRYPTO_HMAC");
        token_set_y("CONFIG_CRYPTO_SHA1");
        token_set_y("CONFIG_CRYPTO");


	/* Not sure on Kernel Configs */
#if 0
	token_set("CONFIG_SYSVIPC_SEMMNI", "128");
	token_set("CONFIG_SYSVIPC_SEMMSL", "250");
        token_set_y("CONFIG_KOBJECT_UEVENT");
        token_set_y("CONFIG_IKCONFIG");
#endif
	
	/* CPU selection */
	token_set_y("CONFIG_CPU_MIPS32");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");

	/* General MIPS */
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_CPU_MIPS32_R1");

	/* MIPS specific settings */
	token_set_y("CONFIG_BOOT_ELF32");
	token_set_y("CONFIG_32BIT");
        token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "5");
#if 0
	token_set_y("CONFIG_MIPS_BOARDS_GEN");
#endif

	/* General_setup */
	token_set("CONFIG_CC_ALIGN_JUMPS", "0");
	token_set("CONFIG_CC_ALIGN_LOOPS", "0");
	token_set("CONFIG_CC_ALIGN_LABELS", "0");
	token_set("CONFIG_CC_ALIGN_FUNCTIONS", "0");

	/* Loop devices */
	token_set_y("CONFIG_BLK_DEV_LOOP");

	token_set_y("CONFIG_CROSSCOMPILE");

#if 0 /* Consider to add when USB device is set */
	token_set_y("CONFIG_HOTPLUG");
#endif

	token_set("CONFIG_CMDLINE", "console=ttyS0,38400 rw%s",
	    token_get("CONFIG_RG_KGDB") ? " nohalt" : "");

	if (token_get("CONFIG_HW_DSP"))
	    token_set_y("CONFIG_RG_DSP_THREAD");
	
	token_set("CONFIG_HZ", "200");
    }
    
    if (token_get("CONFIG_MACH_ADI_FUSIV") ||
	token_get("CONFIG_MACH_IKAN_MIPS"))
    {
	static char cmd_line[255];

	/* CPU */
	if (token_get("CONFIG_MACH_ADI_FUSIV"))
	    token_set_y("CONFIG_CPU_LX4189");
	else
	    token_set_y("CONFIG_CPU_MIPS32_R2");
	
	/* Serial */
	token_set_y("CONFIG_MAGIC_SYSRQ");
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    /* Command line */
	    token_set("CONFIG_CMDLINE", "console=ttyS0,115200");

	    token_set_y("CONFIG_SERIAL_8250");
	    token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	    token_set("CONFIG_SERIAL_8250_NR_UARTS", "1");
	    token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "1");
	    token_set_y("CONFIG_SERIAL_CORE");
	    token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	}
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	{
	    /* Command line */
	    sprintf(cmd_line, "console=ttyS0,115200 root=/dev/ram0 rw %s",
		token_get("CONFIG_RG_RGLOADER") ? "quiet" : ""); 
	    token_set("CONFIG_CMDLINE", cmd_line);

	    token_set_y("CONFIG_SERIAL");
	    token_set_y("CONFIG_SERIAL_CONSOLE");
	}

    	/* Flash/MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");

	/* New MTD configs (Linux-2.6) */
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	    token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	    token_set_y("CONFIG_MTD_CFI_I1");
	    token_set_y("CONFIG_MTD_CFI_I2");
	    token_set_y("CONFIG_MTD_CFI_UTIL");
	}

	/* PCI */
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_PCI_AUTO");
	/* Looks like CONFIG_NEW_PCI is not present in Linux-2.6  */
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	    token_set_y("CONFIG_NEW_PCI");

	/* Kernel type */
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLATMEM_MANUAL");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_HZ", "100");
	token_set_y("CONFIG_HZ_100");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_32BIT");
	
	if (token_get("CONFIG_ATM"))
	    token_set_y("CONFIG_ATM_VBR_AS_RT");
    }
    
    if (token_get("CONFIG_CPU_LX4189") || token_get("CONFIG_CPU_MIPS32_R2"))
    {
	token_set_y("CONFIG_HAS_MMU");
	if (token_get("CONFIG_CPU_MIPS32_R2"))
	    token_set_y("CONFIG_CPU_MIPS32");
	
	/* Linux/Build generic */
	token_set("ARCH", "mips");
	token_set("LIBC_ARCH", "mips");

	if (token_get("CONFIG_RG_OS_LINUX_24"))
	{
	    token_set_y("CONFIG_BINFMT_ELF");
	    token_set_y("CONFIG_SYSCTL");
	    token_set_y("CONFIG_CPU_HAS_PREFETCH");
	    token_set_y("CONFIG_EXPERIMENTAL");
	    token_set_y("CONFIG_CPU_HAS_WB");
	    token_set_y("CONFIG_KCORE_ELF");
	    token_set_y("CONFIG_CROSSCOMPILE");

	    /* RamDisk/CRAMFS/FS options */
	    token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	    token_set_y("CONFIG_CRAMFS_FS");

	    if (!token_get("CONFIG_RG_RGLOADER"))
	    {
		/* Loop devices */
		token_set_y("CONFIG_BLK_DEV_LOOP");
	    }
	    token_set("CONFIG_BLK_DEV_RAM_SIZE", "8000");
	    token_set("CONFIG_RAMDISK_SIZE", "1024");
	    token_set_y("CONFIG_RAMFS");

	    token_set_y("CONFIG_NONCOHERENT_IO");
	    token_set_y("CONFIG_NEW_TIME_C");
	    token_set_y("CONFIG_NEW_IRQ");
	}

	/* The following standard Linux MIPS configs are not set for Lexra:
	 * CONFIG_CPU_MIPS32
	 * CONFIG_CPU_HAS_LLSC
	 * CONFIG_UNIX98_PTY_COUNT (256)
	 * CONFIG_UNIX98_PTYS
	 * CONFIG_CPU_HAS_SYNC
	 */

	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_DMA_NONCOHERENT");
	    token_set_y("CONFIG_TRAD_SIGNALS");
	    token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "5");
	    token_set_y("CONFIG_PAGE_SIZE_4KB");
	    token_set("CONFIG_LOG_BUF_SHIFT", "14");
	    token_set_y("CONFIG_RG_TTYP");
	}

	token_set_y("CONFIG_CPU_BIG_ENDIAN");
	token_set("TARGET_ENDIANESS", "BIG");

	/* MIPS specific settings */
	if (token_get("CONFIG_CPU_LX4189"))
	    token_set_y("CONFIG_MIPS_PATENTFREE");
	token_set_y("CONFIG_MIPS32");
	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_BOOT_ELF32");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	
	token_set_y("CONFIG_CROSSCOMPILE");
#if 0
	/* Wireless Recheck */
	token_set_m("CONFIG_WL");
	token_set_y("CONFIG_WL_AP");
	token_set_y("CONFIG_NET_PCI");
	token_set_y("CONFIG_NET_RADIO");
	token_set_m("CONFIG_HERMES");
	token_set("CONFIG_NET_WIRELESS");
#endif
    }

    if (token_get("CONFIG_KS8695_COMMON"))
    {
	token_set_y("CONFIG_SOFT_FLOAT");
    }
    
    if (token_get("CONFIG_ARMNOMMU"))
    {
	token_set_y("CONFIG_DEBUG_USER");
	token_set("ARCH", "armnommu");
	token_set("LIBC_ARCH", "arm");
	token_set_y("CONFIG_ARM");
	token_set("CONFIG_RAM_LIMIT", "16");
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_NO_UNALIGNED_ACCESS");
	token_set_y("CONFIG_BINUTILS_NEW");
	token_set_y("CONFIG_SOFT_FLOAT");
	if (token_get("CONFIG_RG_OS_LINUX"))
	{	
	    token_set_y("CONFIG_KERNEL_ELF");
	    token_set_y("CONFIG_BINFMT_FLAT");
	    token_set_y("CONFIG_MALLOC_CONSERVE_RAM");
	    token_set_y("CONFIG_SMALL_STACK");
	    token_set_y("CONFIG_TEXT_SECTIONS");
	    token_set_y("CONFIG_NO_FRAME_POINTER");
	    if (!token_get("CONFIG_LSP_DIST"))
		token_set_y("CONFIG_EXTRA_ALLOC");
	}
	token_set("CONFIG_RG_KERNEL_COMP_METHOD", "gzip");
    }

    if (token_get("CONFIG_CX821XX_COMMON"))
    {
	set_big_endian(0);
	token_set_y("CONFIG_CONEXANT_COMMON");
	token_set("CONFIG_ARCH_MACHINE", "cx821xx");
	token_set("CONFIG_NOMMU_PAGECACHE_THRESHOLD", "200");
	token_set("CONFIG_FLASH_POS", "0x00400000");
	token_set_y("CONFIG_SMALL_FLASH");
	token_set_y("CONFIG_RG_FLASH_LINEAR");
	token_set_y("CONFIG_RG_RMT_UPD_LOW_MEM");
	token_set_y("CONFIG_BLK_DEV_LOOP");
	token_set("CONFIG_EXP_BUS_SHL", "0");
	token_set("CONFIG_BOOT_RAM_POS", "0xf00000");
	token_set("CONFIG_BOOT_SP_POS", "0xf00000");
	token_set("CONFIG_SDRAM_BASE", "0x800000");
	token_set("CONFIG_SDRAM_SIZE", "0x800000");
	token_set("CONFIG_PHYS_SDRAM_SIZE", "0x800000");
	token_set("CONFIG_BOOT_FREE_MEM_START", "0xc00000");
	token_set("CONFIG_BOOT_FREE_MEM_END", "0xd00000");
	token_set("CONFIG_RAM_LIMIT", "8");
	/* Taken from CNX BSP */
	token_set_y("MAGIC_ROM_PTR");
	token_set_y("CONFIG_ARCH_CNXT");
	token_set_y("CONFIG_ARCH_CX821XX");
	token_set_y("CONFIG_CPU_ARM940T");
	token_set_y("CONFIG_SERIAL_CNXT");
	token_set_y("CONFIG_CPU_ARM940_CPU_IDLE");
	token_set_y("CONFIG_CPU_ARM940_I_CACHE_ON");
	token_set_y("CONFIG_CPU_ARM940_D_CACHE_ON");
	token_set_y("CONFIG_CPU_ARM940_WRITETHROUGH");
	token_set_y("CONFIG_NO_PGT_CACHE");
	token_set("CONFIG_RAMDISK_SIZE", "256");
	token_set("CONFIG_CRAMFS_RAM_ADDR", "0xf00000");
	token_set_y("CONFIG_CRAMFS_FS");
	token_set_y("CONFIG_CRAMFS_LINEAR");
	if (token_get("CONFIG_KNCX821XX_PCB"))
	    token_set_y("CONFIG_CX821XX_MXIC_FLASH");
    }

    if (token_get("CONFIG_KS8695_COMMON"))
    {
	static char cmd_line[255];

 	token_set("ARCH", "arm");
 	token_set("LIBC_ARCH", "arm");
 	token_set_y("CONFIG_HAS_MMU");
 	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_CPU_32");
 	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_CPU_LITTLE_ENDIAN");
	token_set("TARGET_ENDIANESS", "LITTLE");
 	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_CPU_32v4");
 	token_set_y("CONFIG_RAMFS"); 
	token_set("CONFIG_BLK_DEV_RAM_SIZE", "16384");
	token_set_y("CONFIG_ARCH_KS8695");
	token_set_y("CONFIG_CPU_ARM922T");
	token_set_y("CONFIG_CPU_ARM922_CPU_IDLE");
	token_set_y("CONFIG_CPU_ARM922_I_CACHE_ON");
	token_set_y("CONFIG_CPU_ARM922_D_CACHE_ON");

	token_set_y("CONFIG_NO_FRAME_POINTER");
	token_set_y("CONFIG_FPE_FASTFPE");
 	token_set_y("CONFIG_KCORE_ELF");
 	token_set_y("CONFIG_BINFMT_ELF");
 	token_set_y("CONFIG_SYSCTL");

	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_DYN_LINK");

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_CRAMFS_FS");
	token_set_y("CONFIG_RG_MODFS");
	
	token_set_y("CONFIG_PCI");
	/* ? Replace with CONFIG_RG_... ? */
	token_set_y("CONFIG_PCI_KS8695P"); 
	token_set_y("CONFIG_ARCH_KS8695P");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyAM0");

	sprintf(cmd_line, "console=%s,38400 root=/dev/ram0 rw%s",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"), 
	    token_get("CONFIG_RG_KGDB") ? " nohalt" : "");
		
	token_set("CONFIG_CMDLINE", cmd_line);

	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");

	/* To be replaced by matching CONFIG_RG_... */
	token_set_y("CONFIG_SERIAL_KS8695");
	token_set_y("CONFIG_SERIAL_KS8695_CONSOLE");

	if (!token_get("CONFIG_LSP_DIST"))
	    token_set_m("CONFIG_RG_KRGLDR");	

	/* token_set_y("CONFIG_PLD"); needed for altera ether00 support */

	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");

	/* Debugging configurations */
 	token_set_y("CONFIG_ALIGNMENT_TRAP");
 	token_set("CONFIG_ALIGNMENT_TRAP_MODE", "2");
	token_set_y("CONFIG_DEBUG_USER");
    }
    
    if (token_get("CONFIG_BOOTLDR_UBOOT"))
	token_set_y("CONFIG_SILENT_CONSOLE");    

    if ((token_get("CONFIG_RG_RMT_MNG") || token_get("CONFIG_RG_DSLHOME"))
	&& token_get("CONFIG_OPENRG"))
    {
	token_set_y("CONFIG_RG_SSI");
    }

    if (token_get("CONFIG_RG_OSS_RMT"))
    {
	token_set_y("CONFIG_RG_WAN_UPGRADE");
	token_set_y("CONFIG_RG_SSI");
    }

    if (token_get("CONFIG_RG_3G_WAN") || token_get("CONFIG_RG_3G_VOICE") ||
	token_get("CONFIG_RG_3G_SMS"))
	token_set_y("CONFIG_RG_3G");

    if (token_get("CONFIG_RG_3G_SMS"))
        token_set_y("CONFIG_RG_LIBICONV");

    if (token_get("CONFIG_RG_3G"))
    {
	token_set_y("CONFIG_RG_COMGT");
	token_set_y("CONFIG_USB");
	token_set_y("CONFIG_USB_SERIAL");
	token_set_y("CONFIG_USB_SERIAL_OPTION");
	token_set_y("CONFIG_USB_DEVICEFS");
	token_set_y("CONFIG_HOTPLUG");
	token_set_y("CONFIG_RG_SERIAL");
    }

    if (token_get("CONFIG_RG_3G_WAN"))
	token_set_m("CONFIG_RG_PPPOS");

    if (token_get("CONFIG_RG_PPPOS"))
    {
	token_set_y("CONFIG_RG_SERIAL");
	token_set_y("CONFIG_RG_PPP_PRECONF");
	token_set("CONFIG_RG_PPPOS_PRECONF_DEV_NAME",
	    token_get_str("CONFIG_GSM_MODEM_DEV_NAME"));
        token_set_y("CONFIG_CRC_CCITT");
    }

    if (token_get("CONFIG_RG_PPPOES") && token_get("CONFIG_RG_PPPOE_RELAY"))
	conf_err("ERROR: Cannot config both PPPoE server and relay\n");

    if (token_get("CONFIG_RG_SSI_PAGES"))
    {
	token_set_y("CONFIG_RG_SSI");
        token_set_y("CONFIG_LOCAL_WBM_LIB");
    }

    if (token_get("CONFIG_RG_URL_KEYWORD_FILTER"))
	token_set_y("CONFIG_RG_HTTP_PROXY");
    
    if (token_get("CONFIG_RG_CHILDREN_NETWORK"))
	token_set_y("CONFIG_RG_PURESIGHT");

    if (token_get("CONFIG_RG_PURESIGHT"))
    {
	token_set_y("CONFIG_RG_CXX");
	token_set_y("CONFIG_RG_THREADS");
	token_set_y("CONFIG_RG_LIBEVENT");
	token_set_y("CONFIG_RG_HTTP_PROXY");
    }

    if (token_get("CONFIG_RG_TFTP_UPGRADE"))
    {
	token_set_y("CONFIG_RG_TFTP_SERVER");
	token_set_y("CONFIG_RG_RMT_UPDATE");
    }

    if (token_get("OPENRG_DEBUG"))
    {
	token_set_y("CONFIG_NFS_FS");
	if (!token_get("CONFIG_VALGRIND"))
	    token_set_y("OPENRG_DEBUG_EXEC_SH");
	token_set_y("OPENRG_DEBUG_MODULES");
	token_set_y("CONFIG_RG_NO_OPT");
	/* if we use debug info some elf's exeeded the
	 * cramfs file default upper limit size(2^24) so
	 * we need to increase it 
	 */
	token_set("CRAMFS_SIZE_WIDTH", "28"); /* 256MB */
#if 0 /* Uncomment for file/executable debug */
	token_set_y("OPENRG_DEBUG_FILE_OPEN");
#endif
    }
    else
	token_set("CRAMFS_SIZE_WIDTH", "24"); /* 16MB */

    if (token_get("CONFIG_NFS_FS"))
    {
	token_set_y("CONFIG_LOCKD");
	token_set_y("CONFIG_SUNRPC");
    }

    if (token_get("CONFIG_RG_PERM_STORAGE"))
	token_set_y("CONFIG_GEN_RG_DEFAULT");

    /*  The following 3 CONFIGS control the system behavior in regards to
     *  freeing cached pages. This configuration makes the system try to work
     *  constantly at no less then HIGH*PAGE_SIZE free memory if possible. 
     */

    if (token_get("CONFIG_FREEPAGES_THRESHOLD"))
    {
	token_set("CONFIG_FREEPAGES_MIN", "32");
	token_set("CONFIG_FREEPAGES_LOW", "256");
	token_set("CONFIG_FREEPAGES_HIGH", "384");
    }

    if (token_get("CONFIG_RG_VOATM_ELCP") || token_get("CONFIG_RG_VOATM_CAS"))
	token_set_y("CONFIG_RG_VOATM");
    
    if (token_get("CONFIG_RG_VOATM"))
	token_set_y("CONFIG_RG_TELEPHONY");

    if (token_get("CONFIG_FREESWAN"))
    {
	token_set_y("CONFIG_GMP");
	token_set_y("CONFIG_IPSEC");
	token_set_y("CONFIG_IPSEC_DRIVER");
	token_set_y("CONFIG_IPSEC_IPIP");
	token_set_y("CONFIG_IPSEC_AH");
	token_set_y("CONFIG_IPSEC_AUTH_HMAC_MD5");
	token_set_y("CONFIG_IPSEC_AUTH_HMAC_SHA1");
	token_set_y("CONFIG_IPSEC_ESP");
	token_set_y("CONFIG_IPSEC_ENC_1DES");
	token_set_y("CONFIG_IPSEC_ENC_3DES");
	token_set_y("CONFIG_IPSEC_ENC_NULL");
	token_set_y("CONFIG_IPSEC_ENC_AES");
 	if(!token_get("CONFIG_ELLIPTIC_IPSEC"))
	    token_set_y("CONFIG_IPSEC_IPCOMP");
	token_set_y("CONFIG_IPSEC_NAT_TRAVERSAL");
	token_set_y("CONFIG_IPSEC_DEBUG");
	token_set_y("CONFIG_PLUTO_DEBUG");
	token_set_y("CONFIG_RG_FREESWAN");
	token_set_y("CONFIG_RG_X509");
	token_set_m("CONFIG_RG_NETBIOS_RT");
	token_set_y("CONFIG_RG_IPSEC_NO_SECRET_FILE");
	if (!strcmp(os, "LINUX_24") || !strcmp(os, "LINUX_26"))
	{
	    token_set_y("CONFIG_RG_IPSEC_IKE_ALG");
	    token_set_y("CONFIG_RG_IPSEC_ESP_ALG");
	    token_set_y("CONFIG_IPSEC_ALG");
	}
	if (token_get("CONFIG_RG_IPSEC_ESP_ALG") &&
	    !token_get("CONFIG_IPSEC_ALG_JOINT"))
	{
	    if (token_get("CONFIG_IPSEC_ENC_1DES"))
		token_set_m("CONFIG_IPSEC_ALG_1DES");
	    if (token_get("CONFIG_IPSEC_ENC_3DES"))
		token_set_m("CONFIG_IPSEC_ALG_3DES");
	    if (token_get("CONFIG_IPSEC_ENC_NULL"))
		token_set_m("CONFIG_IPSEC_ALG_NULL");
	    if (token_get("CONFIG_IPSEC_ENC_AES"))
		token_set_m("CONFIG_IPSEC_ALG_AES");
	    if (token_get("CONFIG_IPSEC_AUTH_HMAC_MD5"))
		token_set_m("CONFIG_IPSEC_ALG_MD5");
	    if (token_get("CONFIG_IPSEC_AUTH_HMAC_SHA1"))
		token_set_m("CONFIG_IPSEC_ALG_SHA1");
	}
	/* Although NO_KERNEL_ALG/NO_IKE_ALG have reverse-logic (flag which
	 * is set if another flag is unset), we leave them as is, because they
	 * are part of freeswan library, which is given in source. */
	if (!token_get("CONFIG_RG_IPSEC_ESP_ALG"))
	    token_set_y("NO_KERNEL_ALG");
	if (!token_get("CONFIG_RG_IPSEC_IKE_ALG"))
	    token_set_y("NO_IKE_ALG");
    }

    if (token_get("CONFIG_RG_PPTPS"))
	token_set_y("CONFIG_LIBDES");

    if (token_get("CONFIG_RG_PPTPC"))
	token_set_y("CONFIG_LIBDES");

    if (token_get("CONFIG_SL2312_COMMON_RG"))
    {
	token_set_y("CONFIG_SL2312_COMMON");
	token_set("CONFIG_RAMDISK_SIZE", "512");
	
 	token_set_y("CONFIG_BLK_DEV_IDE");
 	token_set_y("CONFIG_BLK_DEV_SL2312_IDE");
 	token_set_y("CONFIG_BLK_DEV_IDEPCI");
 	token_set_y("CONFIG_BLK_DEV_IDEDMA");
 	token_set_y("CONFIG_BLK_DEV_IDEDMA_PCI");
 	token_set_y("CONFIG_BLK_DEV_SL2312_IDE_CHANNEL_1");
 	token_set_y("CONFIG_BLK_DEV_IDEDISK");
 	token_set_y("CONFIG_BLK_DEV_IDECD");
 	token_set_y("CONFIG_BLK_DEV_IDE_MODES");
 	token_set_y("CONFIG_IDE");
	
	token_set_y("CONFIG_PCI");
 	token_set_y("CONFIG_PCI_NAMES");
 	token_set_y("CONFIG_NET_PCI");
	token_set_y("CONFIG_PCI_RESET");
    }
    
    if (token_get("CONFIG_SIBYTE_SB1250"))
    {
	/* Linux configs */
	/* General MIPS */
	token_set("ARCH", "mips");
	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_MODULES");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_NEW_TIME_C");
	token_set_y("CONFIG_BOOT_ELF32");
	/* Sibyte */
	token_set_y("CONFIG_SMP");
	token_set("CONFIG_NR_CPUS", "2");
	token_set_y("CONFIG_CPU_SB1");
	token_set_y("CONFIG_CPU_SB1_PASS_1");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_CPU_HAS_LLSC");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	/* Current toolchain doesn't support -msb1-pass1-workarounds,
	 * only required by modules, we should understand why
	token_set_y("CONFIG_SB1_PASS_1_WORKAROUNDS");
	*/
	token_set_y("CONFIG_SIBYTE_SB1xxx_SOC");
	token_set_y("CONFIG_SIBYTE_CFE");
	token_set_y("CONFIG_NET_SB1250_MAC"); /* network */
	token_set_y("CONFIG_SERIAL_NONSTANDARD");
	token_set_y("CONFIG_SIBYTE_SB1250_DUART"); /* serial */
	token_set_y("CONFIG_SIBYTE_SB1250_DUART_CONSOLE");
	token_set_y("CONFIG_TRAD_SIGNALS");
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_NO_ISA");
	token_set_y("CONFIG_SWAP_IO_SPACE");
	token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "5");
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_DMA_COHERENT");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");
#if 0 /* TODO: our GCC is not capable of building 64bit code,
       * neither ulibc supports MIPS 64 bit */
	token_set_y("CONFIG_64BIT");
	token_set_y("CONFIG_BUILD_ELF64");
#else
	token_set_y("CONFIG_32BIT");
	token_set_y("CONFIG_BUILD_ELF32");
#endif
	/* general */
	token_set_y("CONFIG_CROSSCOMPILE");
	token_set_y("CONFIG_SYSVIPC");
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_KCORE_ELF");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	token_set_y("CONFIG_NET");
	token_set_y("CONFIG_PACKET");
	token_set_y("CONFIG_PACKET_MMAP");
	//token_set_y("CONFIG_NETLINK_DEV");
	token_set_y("CONFIG_UNIX");
	token_set_y("CONFIG_INET");
	token_set_y("CONFIG_NETDEVICES");
	token_set_y("CONFIG_NET_ETHERNET");

	/* maybe not used by kernel !!! */
	token_set("CONFIG_CMDLINE", "console=ttyS0,115200");

	/* OpenRG configs */
	set_big_endian(1);
	token_set("LIBC_ARCH", "mips");
	token_set_y("CONFIG_HAS_MMU");
    }

    if (token_get("CONFIG_CAVIUM_OCTEON"))
    {
	/* Linux configs */
	/* Generic */
	token_set_y("CONFIG_64BIT");
	token_set_y("CONFIG_BUILD_ELF64");
	token_set_y("CONFIG_SMP");
	token_set("CONFIG_NR_CPUS", "16");
	token_set("CONFIG_HZ", "250");
	token_set_y("CONFIG_HZ_250");
	// token_set_y("CONFIG_COMPAT");
	token_set_y("CONFIG_EPOLL");
	token_set_y("CONFIG_FUTEX");
	token_set_y("CONFIG_GENERIC_IRQ_PROBE");
	token_set_y("CONFIG_IRQ_PER_CPU");
	// token_set_y("CONFIG_GENERIC_ISA_DMA");
	token_set_y("CONFIG_HAVE_MEMORY_PRESENT");
	token_set_y("CONFIG_SPARSEMEM");
	token_set_y("CONFIG_SPARSEMEM_STATIC");
	token_set_y("CONFIG_SPARSEMEM_MANUAL");
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_HW_HAS_PCI");
	token_set_y("CONFIG_PCI_LEGACY_PROC");
	// token_set_y("CONFIG_ISA");
	token_set_y("CONFIG_SWAP_IO_SPACE");
	token_set_y("CONFIG_PAGE_SIZE_4KB");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");
	token_set_y("CONFIG_RT_MUTEXES");
	token_set_y("CONFIG_BITREVERSE");
	token_set_y("CONFIG_PLIST");
	token_set_y("CONFIG_RTC_LIB");
	token_set_y("CONFIG_SCHED_NO_NO_OMIT_FRAME_POINTER");
	token_set_y("CONFIG_RESOURCES_64BIT");

	/* MIPS Octeon */
	token_set("ARCH", "mips");
	token_set_y("CONFIG_MIPS");
	token_set_y("CONFIG_EXPERIMENTAL");
	token_set_y("CONFIG_MODULES");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	// token_set_y("CONFIG_MIPS32_COMPAT");
	token_set_y("CONFIG_CPU_HAS_PREFETCH");
	token_set_y("CONFIG_CPU_HAS_SYNC");
	token_set("CONFIG_MIPS_L1_CACHE_SHIFT", "7");
	token_set_y("CONFIG_CPU_MIPSR2");
	token_set_y("CONFIG_CPU_CAVIUM_OCTEON");
	token_set("CONFIG_CAVIUM_OCTEON_CVMSEG_SIZE", "2");
	token_set_y("CONFIG_CAVIUM_OCTEON_HW_FIX_UNALIGNED");
	token_set("CONFIG_CAVIUM_OCTEON_NUM_PACKET_BUFFERS", "0");
	token_set_y("CONFIG_CAVIUM_OCTEON_REFERENCE_BOARD");
	token_set_y("CONFIG_CAVIUM_OCTEON_SPECIFIC_OPTIONS");
	token_set_y("CONFIG_CAVIUM_OCTEON_USER_IO");
	token_set_y("CONFIG_CAVIUM_OCTEON_USER_MEM");
	token_set("CONFIG_CAVIUM_RESERVE32", "0");
	token_set_y("CONFIG_IRQ_CPU_OCTEON");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2_EXCEPTION");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2_INTERRUPT");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2_LOW_LEVEL_INTERRUPT");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2_MEMCPY");
	token_set_y("CONFIG_CAVIUM_OCTEON_LOCK_L2_TLB");
	token_set_y("CONFIG_HUGETLBFS");
	token_set_y("CONFIG_HUGETLB_PAGE");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	token_set_y("CONFIG_REPLACE_EMULATED_ACCESS_TO_THREAD_POINTER");
	token_set_y("CONFIG_FAST_ACCESS_TO_THREAD_POINTER");
	token_set_y("CONFIG_USE_RI_XI_PAGE_BITS");
	token_set("CONFIG_TEMPORARY_SCRATCHPAD_FOR_KERNEL", "1");
	token_set_y("CONFIG_CAVIUM_OCTEON_WATCHDOG");
	token_set_y("CONFIG_WEAK_ORDERING");
	token_set("OCTEON_CPPFLAGS_GLOBAL_ADD", 
	    "-DUSE_RUNTIME_MODEL_CHECKS=1 "
	    "-DCVMX_ENABLE_PARAMETER_CHECKING=0 "
	    "-DCVMX_ENABLE_CSR_ADDRESS_CHECKING=0 "
	    "-DCVMX_ENABLE_POW_CHECKS=0");

	token_set_y("CONFIG_PROC_KCORE");
	token_set_y("CONFIG_LOCK_KERNEL");
	//token_set_y("CONFIG_STOP_MACHINE");

	/* Octeon devices TODO: apply devices patch
	token_set_y("CONFIG_GEN_RTC");
	token_set_y("CONFIG_CAVIUM_OCTEON_BOOTBUS_COMPACT_FLASH");
	token_set_y("CONFIG_GEN_RTC");
	token_set_y("CONFIG_I2C");
	token_set_y("CONFIG_I2C_ALGOBIT");
	token_set_y("CONFIG_I2C_CHARDEV");
	token_set_y("CONFIG_I2C_OCTEON_TWSI");
	token_set_y("CONFIG_SENSORS_DS1337");
	*/

	/* Uart */
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "4");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "4");
	token_set_y("CONFIG_SERIAL_8250_EXTENDED");
	token_set_y("CONFIG_SERIAL_8250_SHARE_IRQ");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");

	/* TODO: Recheck if needed */
	token_set_y("CONFIG_PACKET_MMAP");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set_y("CONFIG_LEGACY_PTYS");
	token_set("CONFIG_LEGACY_PTY_COUNT", "256");

	/* maybe not used by kernel !!! */
	token_set("CONFIG_CMDLINE", "console=ttyS0,115200");

	/* OpenRG configs */
	set_big_endian(1);
	token_set("LIBC_ARCH", "mips");
	token_set_y("CONFIG_HAS_MMU");
    }    
    
    if (token_get("CONFIG_SL2312_COMMON"))
    {
 	static char cmd_line[255];
 	
	token_set_m("CONFIG_RG_KRGLDR");

	set_big_endian(0);
 	token_set("ARCH", "arm");
 	token_set("LIBC_ARCH", "arm");
 	token_set_y("CONFIG_HAS_MMU");
 	token_set_y("CONFIG_ARM");
 	token_set("CONFIG_ALIGNMENT_TRAP_MODE", "2");
 	token_set_y("CONFIG_UID16");
 	token_set_y("CONFIG_CPU_32");
 	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
 	token_set_y("CONFIG_EXPERIMENTAL");
 	token_set_y("CONFIG_CPU_32v4");
 	token_set_y("CONFIG_BLK_DEV_LOOP"); 
 	token_set_y("CONFIG_RAMFS"); 
 	/* floating point emulator, check */
 	token_set_y("CONFIG_FPE_NWFPE");
 	/* adds /proc/kcore in elf format for dbg */
 	token_set_y("CONFIG_KCORE_ELF");
 	/* support a.out format */
 	token_set_y("CONFIG_BINFMT_AOUT");
 	/* handle unaligned access (TODO remove) */
 	token_set_y("CONFIG_ALIGNMENT_TRAP");
 	/* debug purposes - when user mode crashes gives some info */
 	token_set_y("CONFIG_DEBUG_USER");

 	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_RG_NETTOOLS_ARP");
 		
 	sprintf(cmd_line, "%sconsole=%s,115200 root=/dev/ram0 rw %s",
	    token_get("CONFIG_SL2312_COMMON_RGLOADER") ? "quiet " : "", 
 	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"),
	    token_get("CONFIG_RG_KGDB") ? "nohalt" : "");
 		
 	token_set("CONFIG_CMDLINE", cmd_line);
 	token_set("CONFIG_ARCH_MACHINE", "sl2312");
 	token_set_y("CONFIG_SERIAL_CONSOLE");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
 
 	token_set_y("CONFIG_MTD");

 	token_set_y("CONFIG_SL2312_SHARE_PIN");
 
 	token_set_y("CONFIG_BINFMT_ELF");
 	token_set_y("CONFIG_DEVPTS_FS");
 	token_set_y("CONFIG_SYSCTL");
 	token_set_y("CONFIG_UNIX98_PTYS");
 	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
 	token_set("CONFIG_BLK_DEV_RAM_SIZE", "16384");

 	if (token_get("CONFIG_RG_DEV"))
 	{
 	    /* debug. kernel larger and slower */
 	    token_set_y("CONFIG_FRAME_POINTER");
 	}
 	token_set_y("CONFIG_SL2312_LPC");
 	token_set_y("CONFIG_SL2312_LPC_IT8712");
 	token_set_y("CONFIG_CPU_FA52X");
 	token_set_y("CONFIG_CPU_FA52X_I_CACHE_ON");
 	token_set_y("CONFIG_CPU_FA52X_D_CACHE_ON");
 	token_set_y("CONFIG_IT8712_GPIO");
 	token_set_y("CONFIG_SERIAL_IT8712");
 	token_set_y("CONFIG_SERIAL_IT8712_CONSOLE");
 	token_set("CONFIG_KERNEL_START", "0xc0000000");
	token_set_y("CONFIG_IDEDMA_AUTO");
	token_set_y("CONFIG_CRAMFS_FS");
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_FACTORY_SETTINGS");
    }
    
    if (token_get("CONFIG_CX8620X_COMMON_RGLOADER"))
    {
	/* Override HW settings to save RAM for network boot feature */
	token_set("CONFIG_CX8620X_SDRAM_SIZE", "16");
	/* Enable boot of image from network */
	token_set_y("CONFIG_RG_NETWORK_BOOT");
	token_set_y("CONFIG_CX8620X_COMMON");
	token_set("CONFIG_FORCE_MAX_ZONEORDER", "12");
    }
 
    if (token_get("CONFIG_CX8620X_COMMON"))	
    {
	static char cmd_line[255];
	
	set_big_endian(0);
	token_set_y("CONFIG_CONEXANT_COMMON");
	token_set_y("CONFIG_CX8620X");
	token_set_y("CONFIG_ARCH_CX8620X");
	token_set("CONFIG_ARCH_MACHINE", "cx8620x");

	token_set("ARCH", "arm");
	token_set("LIBC_ARCH", "arm");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_32v5");
	token_set_y("CONFIG_CPU_ARM926T");
	
	token_set_y("CONFIG_NO_UNALIGNED_ACCESS");
	token_set("CONFIG_ALIGNMENT_TRAP_MODE", "2"); // ?
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	//token_set_y("CONFIG_EXPERIMENTAL");
	//token_set_y("CONFIG_SOFT_FLOAT");

	token_set("CONFIG_BLK_DEV_RAM_SIZE", "8192");
	token_set_y("CONFIG_RAMFS");

	token_set_y("CONFIG_CRAMFS_FS");
	if (!token_get("CONFIG_LSP_DIST"))
	{
	    token_set_y("CONFIG_RG_MODFS");
	    token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	}

	token_set("CONFIG_SDRAM_SIZE",
	    token_get_str("CONFIG_CX8620X_SDRAM_SIZE"));

	token_set("CONFIG_ZBOOT_ROM_TEXT", "0");
	token_set("CONFIG_ZBOOT_ROM_BSS", "0");
	/* adds /proc/kcore in elf format for dbg */
	token_set_y("CONFIG_KCORE_ELF");
	/* handle unaligned access (TODO remove) */
	token_set_y("CONFIG_ALIGNMENT_TRAP");
	/* debug purposes - when user mode crashes gives some info */
	token_set_y("CONFIG_DEBUG_USER");
	
	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    /* XXX: Move PCI_RESET to RGLOADER after B14803 is fixed */
	    token_set_y("CONFIG_DYN_LINK");
	    token_set_y("CONFIG_PCI");
	    token_set_y("CONFIG_PCI_RESET");
	}

#if 0
	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    token_set_y("CONFIG_DYN_LINK");
	    token_set_y("CONFIG_RG_NETTOOLS_ARP");
	    token_set_y("CONFIG_PCI_NAMES");
	    token_set_y("CONFIG_NET_PCI");
	}
#endif

	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw%s",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"), 
	    token_get("CONFIG_RG_KGDB") ? " nohalt" : "");
		
	token_set("CONFIG_CMDLINE", cmd_line);
	token_set_y("CONFIG_SERIAL_CONSOLE");
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CONCAT");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set_y("CONFIG_SERIAL");
	token_set_y("CONFIG_SYSCTL");	
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	if (!token_get("CONFIG_LSP_DIST"))
	    token_set_m("CONFIG_RG_KRGLDR");	
	token_set_y("CONFIG_MAGIC_SYSRQ");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");
    }

    if (token_get("CONFIG_COMCERTO_COMMON"))	
    {
	static char cmd_line[255];

	/* Needed to build vendor/mindspeed folder */
	token_set_y("CONFIG_MINDSPEED_COMMON");

	/* Needed to build vendor/mindspeed/comcerto folder */
	token_set_y("CONFIG_COMCERTO");
	token_set_y("CONFIG_COMCERTO_SINGLE_IMAGE");
	token_set("CONFIG_ARCH_MACHINE", "comcerto");
	
	/* Processor */
	set_big_endian(0);
	token_set("ARCH", "arm");
	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_UID16");
	token_set("LIBC_ARCH", "arm");
	token_set_y("CONFIG_CPU_ARM920T");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_32v4");
	token_set_y("CONFIG_CPU_ABRT_EV4T");
	token_set_y("CONFIG_CPU_CACHE_V4WT");
	token_set_y("CONFIG_CPU_CACHE_VIVT");
	token_set_y("CONFIG_CPU_COPY_V4WB");
	token_set_y("CONFIG_CPU_TLB_V4WBI");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4096");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");

	token_set_y("CONFIG_ARCH_COMCERTO");
	token_set_y("CONFIG_ARCH_M828XX");
	token_set_y("CONFIG_CSP_16M");
	token_set_y("CONFIG_M828XX_PROC_FS");

	/* MSP Processor*/
	token_set("MSP_CODE_LOCATION", "/lib/modules/msp.axf");
	token_set("MSP_NODE_LOCATION", "/dev/msp");

	/* Flash Memory */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_COMCERTO");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set_y("CONFIG_MTD_CONCAT");
	token_set_y("CONFIG_MTD_CFI_INTELEXT");
	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_CFI_UTIL");
	token_set("CONFIG_MTD_CFI_AMDSTD_RETRY", "2");
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");

	/* SDRAM memory */
	token_set("CONFIG_SDRAM_SIZE", "112"); /* Needed by OpenRG */

	/* PCI bus */
	token_set_y("CONFIG_PCI"); 
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");

	/* Serial ports */
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_COMCERTO");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "2");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "2");

	/* SPI */
	token_set_y("CONFIG_SPI");
	token_set_y("CONFIG_COMCERTO_SPI");
	token_set_y("CONFIG_SPI_SI3220");

	/* Console */
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");

	/* SmartMedia*/
#if 0
	token_set_y("CONFIG_MTD_NAND");
	token_set_y("CONFIG_MTD_NAND_COMCERTO");
#endif

	/* File System */
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	/* Kernel args */
	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw%s",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"),
	    token_get("CONFIG_RG_KGDB") ? " nohalt" : "");
	token_set("CONFIG_CMDLINE", cmd_line);

	/* Various Options */
	token_set("CONFIG_ZBOOT_ROM_TEXT", "0");
	token_set("CONFIG_ZBOOT_ROM_BSS", "0");
	/* adds /proc/kcore in elf format for dbg */
	token_set_y("CONFIG_KCORE_ELF");
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_UNIX98_PTYS");
	/* handle unaligned access */
	token_set_y("CONFIG_ALIGNMENT_TRAP");
	/* debug purposes - when user mode crashes gives some info */
	token_set_y("CONFIG_DEBUG_USER");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	
	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_DYN_LINK");
    }
    
    /* DALI: Common section for 821XX */
    if (token_get("CONFIG_COMCERTO_COMMON_821XX"))	
    {
	static char cmd_line[255];

	/* Needed to build vendor/mindspeed folder */
	token_set_y("CONFIG_MINDSPEED_COMMON");

	/* Needed to build vendor/mindspeed/comcerto folder */
	token_set_y("CONFIG_COMCERTO");
	token_set_y("CONFIG_COMCERTO_SINGLE_IMAGE");
	token_set("CONFIG_ARCH_MACHINE", "comcerto");
	
	/* debug infp*/
	token_set_y("CONFIG_DEBUG_INFO");
	token_set_y("CONFIG_KALLSYMS");

	/* Processor */
	set_big_endian(0);
	token_set("ARCH", "arm");
	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_MMU");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_UID16");
	token_set("LIBC_ARCH", "arm");
	token_set_y("CONFIG_HAS_MMU");

	token_set_y("CONFIG_ARCH_COMCERTO");
	token_set_y("CONFIG_ARCH_M821XX");
	token_set_y("CONFIG_OUTER_CACHE");
	token_set_y("CONFIG_COMCERTO_L2CC");
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_V6");
	token_set_y("CONFIG_CPU_32v6");
	token_set_y("CONFIG_CPU_ABRT_EV6");
	token_set_y("CONFIG_CPU_CACHE_V6");
	token_set_y("CONFIG_CPU_CACHE_VIPT");
	token_set_y("CONFIG_CPU_COPY_V6");
	token_set_y("CONFIG_CPU_TLB_V6");
	token_set("CONFIG_HZ", "100");

	/* PCI bus */
	token_set_y("CONFIG_PCI"); 

	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4096");
	token_set_y("CONFIG_SELECT_MEMORY_MODEL");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");

	/* handle unaligned access */
	token_set_y("CONFIG_ALIGNMENT_TRAP");

	/* Network interfaces */
	token_set_y("CONFIG_MII");
	token_set_y("CONFIG_NET_ETHERNET");
	token_set_y("CONFIG_NET_COMCERTO");
	token_set_y("CONFIG_COMCERTO_ETH");
	token_set_y("CONFIG_COMCERTO_VED_M821");
//	token_set_y("CONFIG_COMCERTO_MSP_HOTPLUG");
	token_set_y("CONFIG_NETDEVICES");
	token_set_y("CONFIG_PHYLIB");	

	if (token_get("CONFIG_HW_DSP"))
	{
	    /* MSP Processor*/
	    token_set("MSP_CODE_LOCATION", "/lib/modules/msp.axf");

	    // this is needed to load MSP
	    if (token_get("CONFIG_COMCERTO_MSP_HOTPLUG"))
	    {
		token_set_y("CONFIG_HOTPLUG");		
		token_set_y("CONFIG_FW_LOADER");
	    }
	    else
		token_set("MSP_NODE_LOCATION", "/dev/msp");
	}

	/* Flash Memory */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CONCAT");

	token_set_y("CONFIG_MTD_CFI");
	token_set_y("CONFIG_MTD_GEN_PROBE");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");

	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_CFI_UTIL");

	token_set_y("CONFIG_MTD_COMPLEX_MAPPINGS");

	/* SmartMedia*/
	token_set_y("CONFIG_MTD_NAND");
	
	token_set_y("CONFIG_MTD_NAND_VERIFY_WRITE");
	token_set_y("CONFIG_MTD_NAND_IDS");

	token_set_y("CONFIG_MTD_NAND_COMCERTO");

	token_set_y("CONFIG_OBSOLETE_INTERMODULE");

	/* Serial ports */
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "1");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "1");
	/* Console */
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set("CONFIG_RG_CONSOLE_DEVICE", "ttyS0");

	/* SPI */
	token_set_y("CONFIG_SPI_MSPD");
	token_set_y("CONFIG_COMCERTO_SPI");
	
	/* File System */
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	/*USB*/
	if (token_get("CONFIG_HW_USB_STORAGE"))
	{
	    token_set_y("CONFIG_USB");
	    token_set_y("CONFIG_USB_DEVICEFS");
	    token_set_y("CONFIG_USB_EHCI_HCD");
	    token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
	}
	/* Kernel args */
	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw%s mem=%dM",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"),
	    token_get("CONFIG_RG_KGDB") ? " nohalt" : "",
	    token_get("CONFIG_SDRAM_SIZE"));
	token_set("CONFIG_CMDLINE", cmd_line);

	/* Various Options */
	token_set("CONFIG_ZBOOT_ROM_TEXT", "0");
	token_set("CONFIG_ZBOOT_ROM_BSS", "0");
	/* adds /proc/kcore in elf format for dbg */
	/* revisit the kcore_elf if errors for elf */
 	token_set_y("CONFIG_KCORE_ELF"); 
	token_set_y("CONFIG_SYSCTL");
	token_set_y("CONFIG_UNIX98_PTYS");

	/* debug purposes - when user mode crashes gives some info */
	token_set_y("CONFIG_DEBUG_USER");
	token_set_y("CONFIG_BINFMT_ELF");
	token_set("CONFIG_UNIX98_PTY_COUNT", "256");
	token_set_y("CONFIG_MAGIC_SYSRQ");
	
	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_DYN_LINK");

    }

    if (token_get("CONFIG_COMCERTO_COMMON_83XXX"))
    {
	static char cmd_line[255];

	/* Needed to build vendor/mindspeed folder */
	token_set_y("CONFIG_MINDSPEED_COMMON");
	token_set_y("CONFIG_COMCERTO");
	
	/* debug info*/
	token_set_y("CONFIG_DEBUG_INFO");
	token_set_y("CONFIG_KALLSYMS");

	/* Processor */
	set_big_endian(0);
	token_set("ARCH", "arm");
	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_MMU");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_HARDIRQS_SW_RESEND");
	token_set_y("CONFIG_UID16");
	token_set("LIBC_ARCH", "arm");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_V6");
	token_set_y("CONFIG_CPU_32v6");
	token_set_y("CONFIG_CPU_ABRT_EV6");
	token_set_y("CONFIG_CPU_CACHE_V6");
	token_set_y("CONFIG_CPU_CACHE_VIPT");
	token_set_y("CONFIG_CPU_COPY_V6");
	token_set_y("CONFIG_CPU_TLB_V6");
	token_set_y("CONFIG_CPU_CP15");
	token_set_y("CONFIG_CPU_CP15_MMU");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set("CONFIG_ZONE_DMA_FLAG", "1");
	token_set_y("CONFIG_ZONE_DMA");
	token_set_y("CONFIG_FRAME_POINTER");
	token_set_y("CONFIG_FUTEX");
	token_set_y("CONFIG_EPOLL");
	token_set_y("CONFIG_RT_MUTEXES");
	token_set_y("CONFIG_PLIST");

	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set_y("CONFIG_SERIAL_8250_PCI");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "1");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "1");
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");

	/* Flash Memory */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_CONCAT");

	token_set_y("CONFIG_MTD_CFI");
	token_set_y("CONFIG_MTD_GEN_PROBE");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");

	token_set_y("CONFIG_MTD_CFI_AMDSTD");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_CFI_UTIL");

	token_set_y("CONFIG_MTD_COMPLEX_MAPPINGS");

	token_set_y("CONFIG_ARCH_COMCERTO");
	token_set_y("CONFIG_ARCH_M83XXX");
	token_set("CONFIG_HZ", "100");

	/* PCI */
	token_set_y("CONFIG_PCI");
	token_set_y("CONFIG_PCI_MSI");

	/* Although C1K knows to handle several types of unaligned access,
	 * we still need software handler for those not handled by C1K
	 * (e.g. unaligned ldmia command) */ 
	token_set_y("CONFIG_ALIGNMENT_TRAP");

        /* Use noalign to instruct ARM not to generate alignment traps
	 * (C1K alignment logic will attempt to handle them) */
	sprintf(cmd_line, "console=%s,115200 root=/dev/ram0 rw "
	    "mtdparts=\\\"comcertoflash.0:0x%x@0(%s)\\\" noalign",
	    token_get_str("CONFIG_RG_CONSOLE_DEVICE"),
	    token_get("CONFIG_RG_FLASH_LAYOUT_SIZE") * 1024 * 1024,
	    token_get_str("RG_PROD_STR"));
	token_set("CONFIG_CMDLINE", cmd_line);
    }

    if (token_get("CONFIG_AVALANCHE_COMMON"))
    {
	static char cmd_line[255];

	set_big_endian(1);
	token_set("ARCH", "arm");
	token_set("LIBC_ARCH", "arm");
	token_set("CONFIG_ARCH_MACHINE", "avalanche");
	token_set_y("CONFIG_ARCH_AVALANCHE");
	token_set_y("CONFIG_HAS_MMU");

	/* processor */
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_V6");
	token_set_y("CONFIG_CPU_32v6K");
	token_set_y("CONFIG_CPU_32v6");
	token_set_y("CONFIG_CPU_ABRT_EV6");
	token_set_y("CONFIG_CPU_CACHE_V6");
	token_set_y("CONFIG_CPU_CACHE_VIPT");
	token_set_y("CONFIG_CPU_COPY_V6");
	token_set_y("CONFIG_CPU_TLB_V6");
	token_set_y("CONFIG_CPU_CP15");
	token_set_y("CONFIG_CPU_CP15_MMU");
	token_set_y("CONFIG_ARM_THUMB");
	token_set_y("CONFIG_CPU_DCACHE_WRITETHROUGH");
	token_set_y("CONFIG_ICST525");
	token_set_y("CONFIG_ARM_AMBA");

	/* general */
	token_set_y("CONFIG_DMA_NONCOHERENT");
	token_set_y("CONFIG_SWAP_IO_SPACE");
	token_set_y("CONFIG_NONCOHERENT_IO");
	token_set("CONFIG_HZ", "100");
	token_set_y("CONFIG_AEABI");
	token_set_y("CONFIG_SELECT_MEMORY_MODEL");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLATMEM_MANUAL");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4");
	token_set_y("CONFIG_ALIGNMENT_TRAP");
 	token_set("CONFIG_ALIGNMENT_TRAP_MODE", "2");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_IRQ_PROBE");
	token_set_y("CONFIG_FRAME_POINTER");
	token_set_y("CONFIG_STANDALONE");
	token_set_y("CONFIG_BLK_DEV_LOOP");
	token_set_y("CONFIG_BASE_FULL");
	token_set_y("CONFIG_PM");
	token_set_y("CONFIG_FUTEX");
	token_set_y("CONFIG_RT_MUTEXES");
	token_set_y("CONFIG_PLIST");
	token_set_y("CONFIG_EPOLL");
	token_set_y("CONFIG_RAMFS");
	/* Removed as not part of Integral because of footprint, 200KB space.
	 * Only for DEBUG purposes. */
	 if (!token_get("CONFIG_RG_INTEGRAL"))
	     token_set_y("CONFIG_KALLSYMS");
	token_set_y("CONFIG_LOCK_KERNEL");
	token_set_y("CONFIG_BROKEN_ON_SMP");
	token_set("CONFIG_SYSVIPC_SEMMNI", "128");
	token_set("CONFIG_SYSVIPC_SEMMSL", "250");
	token_set_y("CONFIG_OBSOLETE_INTERMODULE");

	/* avalanche SoC common */
	token_set_y("CONFIG_ARM_AVALANCHE_SOC");
	token_set("CONFIG_CPU_FREQUENCY_AVALANCHE", "400");
	token_set("CONFIG_ARM_AVALANCHE_TOP_MEM_RESERVED", "0x200000");
	token_set_y("CONFIG_AVALANCHE_GENERIC_GPIO");
	token_set_y("CONFIG_ARM_AVALANCHE_TIMER16");
	token_set_y("CONFIG_ARM_AVALANCHE_INTC");
	token_set_y("CONFIG_AVALANCHE_INTC_PACING");
	token_set_y("CONFIG_AVALANCHE_INTC_LEGACY_SUPPORT");

	/* ppd */
	token_set_y("CONFIG_ARM_AVALANCHE_PPD");
	
	/* spi */
	token_set_y("CONFIG_SPI");
	token_set_y("CONFIG_SPI_MASTER");
	token_set_y("CONFIG_SPI_TI_CODEC");
	token_set_y("CONFIG_ARM_AVALANCHE_SPI");

	/* wdt */
#if 0
	if (token_get("CONFIG_MACH_PUMA5EVM"))
	{
	    token_set_y("CONFIG_WATCHDOG");
	    token_set_y("CONFIG_WATCHDOG_NOWAYOUT");
	    token_set_y("CONFIG_ARM_AVALANCHE_WDTIMER");
	}
#endif

	/* mtd */
	token_set("CONFIG_MTD_PHYSMAP_START", "0x48000000");
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CHAR");
	token_set_y("CONFIG_MTD_BLOCK");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set_y("CONFIG_MTD_CFI_I1");
	token_set_y("CONFIG_MTD_CFI_I2");
	token_set_y("CONFIG_MTD_SFL");
	token_set("CONFIG_SFL_SFI", "1");
	token_set("CONFIG_SFL_FAST_READ", "1");
	token_set("CONFIG_AVALANCHE_SFL_CLK", "40000000");
	token_set("CONFIG_MTD_PHYSMAP_LEN", "0x1000000");

	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_SLRAM");

	/* i2c */
	token_set_y("CONFIG_I2C");
	token_set_y("CONFIG_I2C_CHARDEV");
	token_set_y("CONFIG_I2C_ALGOAVALANCHE");
	token_set_y("CONFIG_I2C_AVALANCHE");
	token_set("CONFIG_I2C_AVALANCHE_FORCE_POLLED_MODE", "0");
	token_set("CONFIG_I2C_AVALANCHE_CLOCK", "400000");
	token_set("CONFIG_I2C_AVALANCHE_SPIKE_FILTER", "0");

	/* serial */
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "2");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "2");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set("CONFIG_AVALANCHE_NUM_SER_PORTS", "2");
	token_set("CONFIG_AVALANCHE_CONSOLE_PORT", "1");

	/* leds */
	token_set_y("CONFIG_ARM_AVALANCHE_COLORED_LED");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	sprintf(cmd_line, "console=ttyS0,115200n8 root=/dev/ram0 rw");
	token_set("CONFIG_CMDLINE", cmd_line);
    }

    if (token_get("CONFIG_FEROCEON_COMMON"))
    {
	static char mtdparts[255];

	set_big_endian(0);
	
	token_set_y("CONFIG_FEROCEON");
	token_set_y("CONFIG_ARCH_FEROCEON");

	/* Generic Kernel configs */
	token_set_y("CONFIG_ARM");
	token_set_y("CONFIG_HAS_MMU");
	token_set_y("CONFIG_HAS_IOMEM");
	token_set_y("CONFIG_HAS_IOPORT");
	token_set_y("CONFIG_DMA");
	token_set_y("CONFIG_HAS_DMA");
        token_set_y("CONFIG_PCI");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4096");
	token_set_y("CONFIG_GENERIC_CLOCKEVENTS");
	token_set_y("CONFIG_GENERIC_IRQ_PROBE");
	token_set_y("CONFIG_RWSEM_GENERIC_SPINLOCK");
	token_set_y("CONFIG_GENERIC_CALIBRATE_DELAY");
	token_set_y("CONFIG_ALIGNMENT_TRAP");
	token_set_y("CONFIG_PLIST");
        token_set_y("CONFIG_FEROCEON_PROC");
        token_set_y("CONFIG_FEROCEON_WATCHDOG");
	token_set_y("CONFIG_MAGIC_SYSRQ");

	token_set_y("CONFIG_CC_OPTIMIZE_FOR_SIZE");
	token_set_y("CONFIG_KALLSYMS");
	token_set_y("CONFIG_BASE_FULL");
	token_set_y("CONFIG_FUTEX");
	token_set_y("CONFIG_EPOLL");
	token_set_y("CONFIG_SHMEM");
	token_set_y("CONFIG_RT_MUTEXES");

	/* Processor Type */
	token_set_y("CONFIG_CPU_32");
	token_set_y("CONFIG_CPU_ARM926T");
	token_set_y("CONFIG_CPU_32v5");
	token_set_y("CONFIG_CPU_ABRT_EV5TJ");
	token_set_y("CONFIG_CPU_CACHE_VIVT");
	token_set_y("CONFIG_CPU_COPY_V4WB");
	token_set_y("CONFIG_CPU_TLB_V4WBI");
	token_set_y("CONFIG_CPU_CP15");
	token_set_y("CONFIG_CPU_CP15_MMU");

	/* Processor Features */
	token_set_y("CONFIG_ARM_THUMB");
	token_set_y("CONFIG_CPU_DCACHE_WRITETHROUGH");

	/* SoC options */
	token_set_y("CONFIG_MV88F6500");

        /* Etherenet Support */
        if (token_get("CONFIG_MV_ETHERNET"))
        {
            token_set("CONFIG_MV_ETH_0_MTU", "1500");
            token_set("CONFIG_MV_ETH_0_MACADDR", "\"00:00:00:00:00:80\"");
            token_set("CONFIG_MV_ETH_1_MTU", "1500");
            token_set("CONFIG_MV_ETH_1_MACADDR", "\"00:00:00:00:00:81\"");
            token_set("CONFIG_MV_ETH_2_MTU", "1500");
            token_set("CONFIG_MV_ETH_2_MACADDR", "\"00:00:00:00:00:82\"");
            token_set_y("CONFIG_MV_ETH_TOOL");
            token_set_y("CONFIG_MV_ETH_PROC");
            token_set("CONFIG_MV_ETH_TIMER_PERIOD", "10");
            token_set("CONFIG_NET_SKB_HEADROOM", "96");
            token_set_y("CONFIG_MV_ETH_PNC");
            token_set_y("CONFIG_MV_ETH_PNC_PARSER");
            /* Rx/Tx Queue configuration */
            token_set("CONFIG_MV_ETH_RXQ", "8");
            token_set("CONFIG_MV_ETH_TXQ", "8");
            token_set("CONFIG_MV_ETH_RXQ_DESC", "1024");
            token_set("CONFIG_MV_ETH_RXQ_DEF", "0");
            token_set("CONFIG_MV_ETH_TXQ_DESC", "1024");
            token_set("CONFIG_MV_ETH_TXQ_DEF", "0");
            /* Advanced Features */
            token_set("CONFIG_MV_ETH_CLEANUP_TIMER_PERIOD", "10");
            token_set("CONFIG_MV_ETH_TX_DONE_TIMER_PERIOD", "10");
            token_set("CONFIG_MV_ETH_TXDONE_COAL_PKTS", "16");
            token_set("CONFIG_MV_ETH_RX_COAL_PKTS", "32");
            token_set("CONFIG_MV_ETH_RX_COAL_USEC", "100");
        }

	if (token_get("CONFIG_RG_PSE_HW"))
	{
	    token_set("CONFIG_RG_PSE_HW_PLAT_PATH", 
                "vendor/marvell/feroceon/rg/modules");
            /* Marvell's HW PSE Configs */
            token_set_y("CONFIG_MV_INCLUDE_TPM");
	}

	if (token_get("CONFIG_MV_INCLUDE_TPM"))
	{
            token_set_y("CONFIG_MV_ETH_HWF");
            token_set_y("CONFIG_MV_ETH_BM");
            token_set_y("CONFIG_MV_ETH_PMT");
       	    token_set_y("CONFIG_MV_TPM");
	    token_set_y("CONFIG_MV_TPM_SYSFS_HELP");
            token_set_y("CONFIG_MV_TPM_Z2_VER");
	    token_set_y("CONFIG_MV_ETH_PNC_AGING");
	    token_set("CONFIG_MV_PON_TCONTS", "8");
	    token_set("CONFIG_MV_EPON_LLID", "1");
	    token_set_y("CONFIG_MV_TPM_MC");
	}

	if (token_get("CONFIG_MV_INCLUDE_PON"))
	{
	    token_set_y("CONFIG_MV_PON");
	    //token_set_y("CONFIG_MV_TPM_XML_SETUP");            
	    token_set("CONFIG_MV_PON_TXP_DEF", "0");
	    token_set("CONFIG_MV_PON_TXQ_DEF", "0");
	    token_set("CONFIG_MV_PON_TCONTS", "8");
	    token_set("CONFIG_MV_EPON_LLID", "1");
	    token_set_y("CONFIG_PON_Z2");
	    token_set_y("CONFIG_MV_GPON");
	    token_set_y("CONFIG_MV_EPON");
	    token_set_y("CONFIG_MV_GPON_HW_INTERRUPT");
	    token_set_y("CONFIG_MV_GPON_DEBUG_PRINT");
	    token_set_y("CONFIG_MV_GPON_STATIC_GEM_PORT");
	    token_set_y("CONFIG_MV_EPON_HW_INTERRUPT");
	    token_set("CONFIG_MV_EPON_DBA_LOW_RATE_VALUE", "2000");
	    token_set("CONFIG_MV_EPON_DBA_LOW_RATE_THRESHOLD", "20");
	    token_set("CONFIG_MV_EPON_DBA_LOW_RATE_CONST_VALUE", "256");
	    token_set("CONFIG_MV_EPON_DBA_HIGH_RATE_THRESHOLD", "8192");
	    token_set("CONFIG_MV_EPON_DBA_HIGH_RATE_VALUE", "2000");            
        }

	if (token_get("CONFIG_MV_INCLUDE_CUST"))
	{
	    token_set_y("CONFIG_MV_CUST");
	    token_set_y("CONFIG_MV_CUST_IGMP_HANDLE");
	    token_set_y("CONFIG_MV_CUST_FLOW_MAP_HANDLE");

	    token_set_y("CONFIG_MV_ETH_RX_SPECIAL");
	    token_set_y("CONFIG_MV_ETH_TX_SPECIAL");
            token_set_y("CONFIG_POSIX_MQUEUE");            
        }

        /* Buffer Management Support */
        if (token_get("CONFIG_MV_ETH_BM"))
        {
            token_set_y("CONFIG_MV_ETH_BM_CPU");
            token_set("CONFIG_MV_ETH_BM_0_PKT_SIZE", "0");
            token_set("CONFIG_MV_ETH_BM_1_PKT_SIZE", "0");
            token_set("CONFIG_MV_ETH_BM_2_PKT_SIZE", "0");
            token_set("CONFIG_MV_ETH_BM_3_PKT_SIZE", "256");
            token_set_y("CONFIG_MV_ETH_BM_PORT_0");
            token_set("CONFIG_MV_ETH_BM_PORT_0_LONG_POOL", "0");
            token_set("CONFIG_MV_ETH_BM_PORT_0_SHORT_POOL", "3");
            token_set("CONFIG_MV_ETH_BM_PORT_0_LONG_BUF_NUM", "2048");
            token_set("CONFIG_MV_ETH_BM_PORT_0_SHORT_BUF_NUM", "4096");
            token_set_y("CONFIG_MV_ETH_BM_PORT_1");
            token_set("CONFIG_MV_ETH_BM_PORT_1_LONG_POOL", "1");
            token_set("CONFIG_MV_ETH_BM_PORT_1_SHORT_POOL", "3");
            token_set("CONFIG_MV_ETH_BM_PORT_1_LONG_BUF_NUM", "2048");
            token_set("CONFIG_MV_ETH_BM_PORT_1_SHORT_BUF_NUM", "4096");
            token_set_y("CONFIG_MV_ETH_BM_PORT_2");
            token_set("CONFIG_MV_ETH_BM_PORT_2_LONG_POOL", "2");
            token_set("CONFIG_MV_ETH_BM_PORT_2_SHORT_POOL", "3");
            token_set("CONFIG_MV_ETH_BM_PORT_2_LONG_BUF_NUM", "2048");
            token_set("CONFIG_MV_ETH_BM_PORT_2_SHORT_BUF_NUM", "4096");
        }

        if (token_get("CONFIG_MV_ETH_HWF"))
        {
            token_set("CONFIG_MV_ETH_HWF_TXQ_DROP", "60");
            token_set("CONFIG_MV_ETH_HWF_TXQ_DROP_RND", "0");
        }

        if (token_get("CONFIG_MV_ETH_PNC_PARSER"))
        {
            token_set("CONFIG_MV_ETH_PNC_MCAST_NUM", "8");
            token_set("CONFIG_MV_ETH_PNC_VLAN_PRIO", "1");
            token_set("CONFIG_MV_ETH_PNC_ETYPE", "0");
            token_set("CONFIG_MV_ETH_PNC_DSCP_PRIO", "4");
        }

	/* Kernel Features */
	token_set_y("CONFIG_HZ_100");
	token_set("CONFIG_HZ", "100");
	token_set_y("CONFIG_AEABI");
	token_set_y("CONFIG_OABI_COMPAT");
	token_set_y("CONFIG_SELECT_MEMORY_MODEL");
	token_set_y("CONFIG_FLATMEM_MANUAL");
	token_set_y("CONFIG_FLATMEM");
	token_set_y("CONFIG_FLAT_NODE_MEM_MAP");
	token_set("CONFIG_SPLIT_PTLOCK_CPUS", "4096");
	token_set_y("CONFIG_ALIGNMENT_TRAP");

	/* Boot options */
	token_set("CONFIG_ZBOOT_ROM_TEXT", "0");
	token_set("CONFIG_ZBOOT_ROM_BSS", "0");
	token_set("CONFIG_CMDLINE", "console=ttyS0,115200 root=/dev/ram0 rw");

	/* Feroceon CPU Frequency scaling */
	token_set_y("CONFIG_CPU_FREQ");
	token_set_y("CONFIG_CPU_FREQ_TABLE");
	token_set_y("CONFIG_CPU_FREQ_STAT");
	token_set_y("CONFIG_CPU_FREQ_DEFAULT_GOV_USERSPACE");
	token_set_y("CONFIG_CPU_FREQ_GOV_USERSPACE");
	token_set_y("CONFIG_CPU_FREQ_GOV_ONDEMAND");
	token_set_y("CONFIG_CPU_FREQ_FEROCEON_KW2");

	/* Input device support */
	token_set_y("CONFIG_INPUT");

	/* Serial drivers */
	token_set_y("CONFIG_SERIAL_8250");
	token_set_y("CONFIG_SERIAL_8250_CONSOLE");
	token_set("CONFIG_SERIAL_8250_NR_UARTS", "4");
	token_set("CONFIG_SERIAL_8250_RUNTIME_UARTS", "4");

	/* Non-8250 serial port support */
	token_set_y("CONFIG_SERIAL_CORE");
	token_set_y("CONFIG_SERIAL_CORE_CONSOLE");
	token_set_y("CONFIG_UNIX98_PTYS");
	token_set_y("CONFIG_LEGACY_PTYS");
	token_set("CONFIG_LEGACY_PTY_COUNT", "16");

	/* Kernel hacking */
	token_set("CONFIG_LOG_BUF_SHIFT", "14");
	token_set_y("CONFIG_FRAME_POINTER");

	/* SATA */
	if (token_get("CONFIG_HW_SATA"))
	{
	    token_set_y("CONFIG_SCSI_MVSATA");
	    token_set_y("CONFIG_SATA_NO_DEBUG");
	    token_set_y("CONFIG_MV_INCLUDE_INTEG_SATA");
	    token_set_y("CONFIG_MV_SATA_ENABLE_1MB_IOS");
	}

	/* MTD */
	token_set_y("CONFIG_MTD");
	token_set_y("CONFIG_MTD_FEROCEON");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_1");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_2");
	token_set_y("CONFIG_MTD_MAP_BANK_WIDTH_4");
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_CMDLINE_PARTS");
	token_set_y("CONFIG_MTD_SLRAM");	
	if (token_get("CONFIG_HW_FLASH_NAND"))
	{
	    token_set_y("CONFIG_MV_NAND");
	    token_set_y("CONFIG_MV_NAND_BOOT");
	    token_set_y("CONFIG_NAND_RS_ECC_SUPPORT");
	    token_set_y("CONFIG_MV_INCLUDE_NAND");
	}

#if 0 /* Use U-Boot mtdparts cmdline */
	if (token_get_str("CONFIG_RG_UBI_PARTITION") &&
	    token_get("CONFIG_RG_UBI_PARTITION_SIZE"))
	{
	    sprintf(mtdparts, "nand_mtd:0x00200000@0x00000000(U-Boot),"
                "%d@0x00200000(%s),0x04000000@0x04000000(FS)",
		token_get("CONFIG_RG_UBI_PARTITION_SIZE"),
		token_get_str("CONFIG_RG_UBI_PARTITION"));
	}
#endif

	if (*mtdparts)
	    token_set("CONFIG_RG_CMDLINE_MTDPARTS", mtdparts);

        if (token_get("CONFIG_HW_USB_HOST_EHCI"))
        {
            token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
            token_set_y("CONFIG_MV_INCLUDE_USB");
        }

	if (token_get("CONFIG_IPSEC_USE_FEROCEON_CRYPTO"))
	{
	    token_set_y("CONFIG_MV_INCLUDE_CESA");
	    token_set_y("CONFIG_MV_CESA");
	    token_set_y("CONFIG_MV_CESA_OCF");
	    token_set_y("CONFIG_OCF_OCF");
	    token_set("CONFIG_MV_CESA_CHANNELS", "1");
	}

        if (token_get("CONFIG_RG_FIBERTEC"))
        {
	    token_set_y("CONFIG_RFC1918_ONLY_NAT");

            /* Fibertec Network Topology:
             * 1. DATA V-Network. DATA Bridge:
             *    - DATA LAN (Management VLAN - PVID 1)
             *    - DATA Wireless Access Point
             * 2. GUEST V-Network. GUEST Bridge:
             *    - GUEST LAN (VLAN 4)
             *    - GUEST Wireless Access Point
             * 3. PUBLIC V-Network. PUBLIC Bridge:
             *    - PUBLIC LAN (VLAN 3)
             *    - PUBLIC WAN (VLAN 3)
             * 4. MEDIA V-Network. VLAN 100.
	     * 5. DATA WAN VLAN. VLAN 2 
             * 6. PUBLIC IPSEC Tunnel.
	     * 7. PUBLIC Wireless Access Point
             */
            dev_add_desc("eth0.1", DEV_IF_USER_VLAN, DEV_IF_NET_INT,
                "Data LAN Ethernet");
            dev_add_desc("eth0.4", DEV_IF_USER_VLAN, DEV_IF_NET_INT,
                "Guest LAN Ethernet");
            dev_add_desc("eth0.3", DEV_IF_USER_VLAN, DEV_IF_NET_INT,
                "Public LAN Ethernet");
            dev_add_desc("eth1.3", DEV_IF_USER_VLAN, DEV_IF_NET_EXT,
                "Public WAN Ethernet");
            dev_add_desc("eth0.100", DEV_IF_USER_VLAN, DEV_IF_NET_INT,
                "MEDIA LAN Ethernet");
	    if (!token_get("CONFIG_FIBERTEC_DOGFOOD"))
	    {
		dev_add_desc("eth1.2", DEV_IF_USER_VLAN, DEV_IF_NET_EXT,
			"Data WAN Ethernet");
	    }

            if (token_get("CONFIG_HW_PON_WAN"))
            {
                dev_add_desc("pon0.2", DEV_IF_USER_VLAN, DEV_IF_NET_EXT,
                    "Data WAN PON");
            }

	    if (token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
	    {
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0.1", "ath0", NULL);
		dev_add_bridge("br1", DEV_IF_NET_INT, "eth0.4", "ath1", NULL);
		dev_add_bridge("br2", DEV_IF_NET_EXT, "eth0.3", "eth1.3", NULL);
	    }
	    else if (token_get("CONFIG_MV_WIFI_8764") || token_get("CONFIG_MV_WIFI_8864"))
	    {
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0.1", 
		    "wdev0ap0", "wdev1ap0", NULL);
		dev_add_bridge("br1", DEV_IF_NET_INT, "eth0.4", 
		    "wdev0ap1", "wdev1ap1", NULL);
		dev_add_bridge("br2", DEV_IF_NET_EXT, "eth0.3", "eth1.3", NULL);
	    }

	    dev_add("ips0", DEV_IF_IPSEC_CONN, DEV_IF_NET_EXT);
        }
	else
	{
	    if (token_get("CONFIG_RG_ATHEROS_HW_AR5416"))
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", "ath0", NULL);
	    else if (token_get("CONFIG_QUANTENNA_QHS_71X"))
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", "host0", NULL);
 	    else if (token_get("CONFIG_MV_WIFI_8764") || token_get("CONFIG_MV_WIFI_8864"))
	    {
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", "wdev0ap0",
		    "wdev1ap0", NULL);
	    }
	}

	/* i2c support */
	token_set_y("CONFIG_I2C");
	token_set_y("CONFIG_I2C_CHARDEV");
	token_set_y("CONFIG_I2C_MV64XXX");

	token_set_y("CONFIG_RG_GDBSERVER");
    }

    if (token_get("CONFIG_RG_IPPHONE") || token_get("CONFIG_RG_ATA") ||
	token_get("CONFIG_RG_PBX"))
    {
	token_set_y("CONFIG_RG_VOIP");

	if (!token_get("CONFIG_RG_JPKG") &&
	    !token_get_str("CONFIG_HW_NUMBER_OF_FXS_PORTS"))
	{
	    token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "0");
	}
    }

    if (token_get("CONFIG_RG_PBX"))
    {
	if (token_get("CONFIG_RG_VOIP_ASTERISK"))
	    token_set_y("CONFIG_RG_LIBMAD");
	/* 0 indicates unlimited number of trunks */
	token_set("CONFIG_RG_MAX_NUMBER_OF_TRUNKS",
	    token_get("CONFIG_RG_INTEGRAL") ? "1" : "0");
    }

    if (token_get("CONFIG_RG_VOIP_RV_SIP"))
    {
	token_set_y("CONFIG_RG_VOIP_RV");
	token_set_y("CONFIG_RG_VOIP_SIP");
    }

    if (token_get("CONFIG_RG_VOIP_RV_H323"))
    {
	token_set_y("CONFIG_RG_VOIP_RV");
	token_set_y("CONFIG_RG_VOIP_H323");
    }

    if (token_get("CONFIG_RG_VOIP_RV_MGCP"))
    {
	token_set_y("CONFIG_RG_VOIP_RV");
	token_set_y("CONFIG_RG_VOIP_MGCP");
    }

    if (token_get("CONFIG_RG_VOIP_OSIP"))
	token_set_y("CONFIG_RG_VOIP_SIP");

    if (token_get("CONFIG_RG_VOIP_ASTERISK_SIP"))
    {
	token_set_y("CONFIG_RG_VOIP_ASTERISK");
	token_set_y("CONFIG_RG_VOIP_SIP");
	/* VoIP RTP statistics */
	if (token_get("CONFIG_RG_REDUCE_SUPPORT"))
	{
	    /* 50 KB */
	    token_set("CONFIG_RG_SYSLOG_VOIP_RTP_STATS_SIZE", "51200");
	    token_set_y("CONFIG_RG_LOG_VOIP_RTP_STATS");
	}
    }

    if (token_get("CONFIG_RG_VOIP_ASTERISK_H323"))
    {
	token_set_y("CONFIG_RG_VOIP_ASTERISK");
	token_set_y("CONFIG_RG_VOIP_H323");
	token_set_y("CONFIG_RG_CXX");
    }

    if (token_get("CONFIG_RG_VOIP_ASTERISK_MGCP_GW"))
    {
	token_set_y("CONFIG_RG_VOIP_ASTERISK");
	token_set_y("CONFIG_RG_VOIP_MGCP");
    }

    if (token_get("CONFIG_RG_VOIP_ASTERISK_MGCP_CALL_AGENT"))
    {
	token_set_y("CONFIG_RG_VOIP_ASTERISK");
	token_set_y("CONFIG_RG_VOIP_MGCP_CALL_AGENT");
    }

    if (token_get("CONFIG_RG_VOIP_SIP"))
	token_set_y("CONFIG_RG_VOIP_USE_SIP_ALG");

    if (token_get("CONFIG_RG_VOIP_RV") || token_get("CONFIG_RG_VOIP_OSIP") ||
	token_get("CONFIG_RG_VOIP_ASTERISK") || 
	token_get("CONFIG_RG_VOIP_MEDIA5"))
    {
	token_set_y("CONFIG_RG_VOIP");
    }
    
    if (token_get("CONFIG_RG_VOIP") && !token_get("CONFIG_RG_VOIP_MEDIA5"))
	token_set_m("CONFIG_RG_JRTP");

    if (token_get("CONFIG_RG_WPS") &&
	!token_get("CONFIG_RALINK_RT2880") &&
	!token_get("CONFIG_RALINK_RT2860"))
    {
	token_set_y("CONFIG_RG_WSC");
    }

    if (token_get("CONFIG_RG_WSC"))
	token_set_y("CONFIG_RG_CXX");

    if (token_get("CONFIG_RG_VOIP_ASTERISK"))
    {
	/* DND depends on rg_conf_print and rg_conf_set */
	token_set_y("CONFIG_ULIBC_DO_C99_MATH");
	token_set_y("CONFIG_ULIBC_SHARED");
    }

    if (token_get("CONFIG_RG_CABLEHOME") || token_get("CONFIG_RG_DSL_CH"))
    {
	token_set_y("CONFIG_RG_TODC");
	token_set_y("CONFIG_RG_PING");
	token_set_y("CONFIG_RG_SYSLOG_REMOTE");
	token_set_y("CONFIG_RG_SSL");
	token_set_y("CONFIG_RG_ALG_H323");
	token_set_y("CONFIG_RG_ALG_MSNMS");
	token_set_y("CONFIG_RG_ALG_AIM");
	token_set_y("CONFIG_RG_ALG_PPTP");
	token_set_y("CONFIG_RG_ALG_IPSEC");
	token_set_y("CONFIG_RG_ALG_L2TP");
	token_set_y("CONFIG_RG_CH_EVT_RPT");
    }

    if (token_get("CONFIG_RG_CABLEHOME"))
    {
	/* Configuration for CableHome that we don't want for DSL-CH */
	if (token_get("CONFIG_RG_CABLEHOME_10"))
	    token_set_y("CONFIG_RG_RMT_FW_CONF");
	if (!token_get("CONFIG_RG_CABLEHOME_EMBEDDED"))
	{
	    token_set_y("CONFIG_RG_STANDALONE_PS");
	    token_set_y("CONFIG_RG_RMT_UPDATE");
	}

	token_set_y("CONFIG_RG_CH_FW");
	token_set_y("CONFIG_RG_CONN_SPEED");
	token_set_y("CONFIG_RG_KERBEROS");
	token_set_y("CONFIG_RG_CERT");
	token_set_y("CONFIG_RG_XML");
	token_set_m("CONFIG_RG_PKTCBL");
	token_set_y("CABLEHOME_TEP_BUG");
	token_set_m("CONFIG_RG_BRIDGE");
	token_set_m("CONFIG_RG_USFS");
	token_set_m("CONFIG_RG_BROUTE");
	token_set_m("CONFIG_RG_CHWAN");
    }

    if (token_get("CONFIG_RG_WAN_UPGRADE") || 
	token_get("CONFIG_RG_RMT_FW_CONF") ||
	token_get("CONFIG_RGLOADER_CLI") ||
	token_get("CONFIG_RG_DDNS"))
    {
	token_set_y("CONFIG_RG_WGET");
    }

    if (token_get("CONFIG_RG_WAN_UPGRADE"))
    {
	if (!token_get("CONFIG_HW_CABLE_WAN"))
	    token_set_y("CONFIG_RG_WAN_UPGRADE_IN_GUI");
    }

    if (token_get("CONFIG_MTD")) 
    {
	token_set_y("CONFIG_MTD_PARTITIONS");
	token_set_y("CONFIG_MTD_BLOCK");
	if (token_get("CONFIG_HW_FLASH_NOR"))
	{
	    token_set_y("CONFIG_MTD_CFI");
	    token_set_y("CONFIG_MTD_GEN_PROBE");
	}
	if (token_get("CONFIG_HW_FLASH_NAND"))
	{
	    token_set_default("CONFIG_MTD_NAND", "y");
	    token_set_default("CONFIG_MTD_NAND_IDS", "y");
	    token_set_y("CONFIG_MTD_CHAR");
	}
	token_set_y("CONFIG_MTD_BLKDEVS");

	if (token_get("CONFIG_MTD_UBI"))
	{
	    token_set_y("CONFIG_MTD_CHAR");
	    token_set("CONFIG_MTD_UBI_GLUEBI", token_get_str("CONFIG_MTD_UBI"));
	    if (!token_get_str("CONFIG_MTD_UBI_WL_THRESHOLD"))
		token_set("CONFIG_MTD_UBI_WL_THRESHOLD", "4096");
	    if (!token_get_str("CONFIG_MTD_UBI_BEB_RESERVE"))
		token_set("CONFIG_MTD_UBI_BEB_RESERVE", "2");
	}
    }

    if (token_get("CONFIG_RG_MTD_UTILS"))
	token_set_y("CONFIG_SYSFS");

    if (token_get("CONFIG_RG_KGDB"))
    {
	if (token_get("CONFIG_ARM") || token_get("CONFIG_ARCH_SOLOS") ||
	    token_get("CONFIG_BCM96358") || token_get("CONFIG_DANUBE") ||
	    token_get("CONFIG_CENTILLIUM_P400"))

	{
	    token_set_y("CONFIG_KGDB");
	    token_set_y("CONFIG_KGDB_SERIAL");
	    if (token_get("CONFIG_ARCH_SOLOS"))
		token_set_y("CONFIG_KGDB_SOLOS_SERIAL");
	    if (token_get("CONFIG_BCM96358"))
		token_set_y("CONFIG_KGDB_BCM963XX_SERIAL");
	    if (token_get("CONFIG_CENTILLIUM_P400"))
	    {
		token_set_y("CONFIG_KGDB_8250");
		token_set_y("CONFIG_KGDB_SIMPLE_SERIAL");
		token_set("CONFIG_KGDB_BAUDRATE", "38400");
		token_set("CONFIG_KGDB_PORT_NUM", "0");
	    }
	    token_set_y("CONFIG_MAGIC_SYSRQ");
	}
	else if (token_get("CONFIG_MIPS"))
	{
	    token_set_y("CONFIG_KGDB");
	    /* Mips moved to CONFIG_KGDB from 2.4.21.mips.
	     * This config is kept for compatability with platforms which still
	     * need it.
	     */
	    token_set_y("CONFIG_REMOTE_DEBUG");
	}
    }

    if (token_get("CONFIG_RG_DOCSIS"))
    {
	token_set("ULIBC_DIR", "vendor/ti/ulibc");
	/* TI configs */
	token_set_y("CONFIG_TI_CLI");
	token_set_y("CONFIG_TI_COMMON_COMPONENTS_NO_SNMP_SO_LIBS");
	token_set_y("CONFIG_TI_COMMON_COMPONENTS_NO_SO_LIBS");
	token_set_y("CONFIG_TI_COMMON_COMPONENTS_TICC");
	token_set_y("CONFIG_TI_COMMON_COMPONENTS_UNIFIED_SNMP_AGENT");
	token_set_y("CONFIG_TI_COMMON_COMPONENTS");
	token_set_y("CONFIG_TI_PCD");
	token_set_y("CONFIG_TI_D3_BPI");
	token_set_y("CONFIG_TI_D3_CHANNEL_MANAGER");
	token_set_y("CONFIG_TI_D3_DISPATCHER");
	token_set_y("CONFIG_TI_D3_DOCSIS_DB");
	token_set_y("CONFIG_TI_D3_DOCSIS_MAC_DRIVER");
	token_set_y("CONFIG_TI_D3_DOCSIS_MANAGER");
	token_set_y("CONFIG_TI_D3_DS_CHANNEL_LIST_FREQ_DB");
	token_set_y("CONFIG_TI_D3_DS_SG_RESOLUTION");
	token_set_y("CONFIG_TI_D3_FREQUENCY_SCAN");
	token_set_y("CONFIG_TI_D3_IP_DB");
	token_set_y("CONFIG_TI_D3_IP_PROV");
	token_set_y("CONFIG_TI_D3_MAC_MNGR");
	token_set_y("CONFIG_TI_D3_MDD_COLLECT");
	token_set_y("CONFIG_TI_D3_MDD_DB");
	token_set_y("CONFIG_TI_D3_PROVISIONING");
	token_set_y("CONFIG_TI_D3_REGISTRATION");
	token_set_y("CONFIG_TI_D3_ROLL_BACK");
	token_set_y("CONFIG_TI_D3_STARTUP");
	token_set_y("CONFIG_TI_D3_SW_DL");
	token_set_y("CONFIG_TI_D3_UCD_DB");
	token_set_y("CONFIG_TI_D3_UCD_MNGR");
	token_set_y("CONFIG_TI_D3_USSGRES");
	token_set_y("CONFIG_TI_D3_UTILS");
	token_set_y("CONFIG_TI_DEVICE_INDEX_REUSE");
	token_set_y("CONFIG_TI_DEVICE_PROTOCOL_HANDLING");
	token_set_y("CONFIG_TI_DNSC_RULI");
	token_set_m("CONFIG_TI_DOCSIS_BRIDGE");
	token_set_y("CONFIG_TI_DOCSIS_EGRESS_HOOK");
	token_set_y("CONFIG_TI_EGRESS_HOOK");
	token_set_y("CONFIG_TI_DOCSIS");
	token_set_y("CONFIG_TI_DS_MNGR");
	token_set_y("CONFIG_TI_EVENT_MGR");
	token_set_y("CONFIG_TI_ICC");

	token_set_y("CONFIG_TI_KERBEROS_SHARED");
	token_set_y("CONFIG_TI_KERBEROS");
	token_set_y("CONFIG_TI_L2_SELECTIVE_FORWARDER");
	token_set_y("CONFIG_TI_LOGGER_DEBUG_ENABLE");
	token_set_y("CONFIG_TI_LOGGER");
	token_set_y("CONFIG_TI_META_DATA");
	token_set_y("CONFIG_TI_IP_PKTINFO_SOCKOPT");
	token_set_y("CONFIG_TI_NETDK");
	token_set_y("CONFIG_TI_NET_STATS");
	token_set_y("CONFIG_TI_NVM");
	token_set_y("CONFIG_TI_GPTIMER");
	token_set_y("CONFIG_TI_NETUTILS");
	token_set_y("CONFIG_TI_ENVUTILS");
	token_set_y("CONFIG_TI_SCHED_UTIL");
	token_set_y("CONFIG_TI_IOSTAT_UTIL");
	token_set_y("CONFIG_TI_SEND_MNGT_MSG");
	token_set_y("CONFIG_TI_SHMDB");
	token_set_y("CONFIG_TI_SNMP_AGENT");
	token_set_y("CONFIG_TI_SSL_SHARED");
	token_set_y("CONFIG_TI_SSL");
	token_set_y("CONFIG_TI_STATEMACHINE");
	token_set_y("CONFIG_TI_TFTP");
	token_set_y("CONFIG_TI_TIUDHCPC_T1_T2_OVERRIDE");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_BOOTFILE");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_BOOTSIZE");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_BROADCAST");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_COOKIESVR");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_IPTTL");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_LPRSVR");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_MESSAGE");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_MTU");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_NTPSVR");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_ROOTPATH");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_SWAPSVR");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_TFTP");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_TIMESVR");
	token_set_y("CONFIG_TI_TIUDHCPC_OPT_WINS");
	token_set_y("CONFIG_TI_TIUDHCP");
	token_set_y("CONFIG_TI_TLVPARSER");
	token_set_y("CONFIG_TI_TODC");
	token_set_y("CONFIG_TI_US_MNGR");
	token_set_y("CONFIG_TI_WATCHDOG_RT");

	token_set("CONFIG_TI_ICC_DEFAULT_ENDPOINTS", "120");
	token_set("CONFIG_TI_ICC_DEFAULT_SHM_SIZE", "512000");
	token_set("CONFIG_TI_LOGGER_BYTES_LIMIT", "20000");
	token_set("CONFIG_TI_LOGGER_DESTINATION_IP", "192.168.100.100");
	token_set("CONFIG_TI_LOGGER_DESTINATION_PORT", "4571");
	token_set("CONFIG_TI_LOGGER_FILENAME", "/var/tmp/logfile.log");
	token_set("CONFIG_TI_LOGGER_MESSAGE_LIMIT", "100");
	token_set("CONFIG_TI_NVM_PATH", "/nvram/");
	token_set("CONFIG_TI_PUMA5_VERSION", "1.0");
	token_set("CONFIG_TI_TIUDHCPC_MAX_BACKOFF_TIME", "64");
	token_set("CONFIG_TI_TIUDHCPC_MAX_CONFIG_ATTEMPTS_THRESHOLD", "5");
	token_set("CONFIG_TI_TIUDHCPC_MAX_OPTION_BUFSIZE", "400");
	token_set("CONFIG_TI_TIUDHCPC_RESTART_DELAY", "10");

	/* Required kernel support */
	token_set_y("CONFIG_LLC");
	token_set_y("CONFIG_NET_KEY");

	/* Required OpenRG support */
	/* token_set_y("CONFIG_BRIDGE_UTILS");	using TI's source bridge */
	token_set_y("CONFIG_RG_SSL");
	if (!token_get("CONFIG_LSP_DIST"))
	    token_set_y("CONFIG_RG_RMT_UPDATE");
	token_set_y("CONFIG_RG_OPENSSL_COMMON");

	token_set_m("CONFIG_JFFS2_FS");
	token_set_y("CONFIG_JFFS2_RTIME");
	token_set_y("CONFIG_JFFS2_ZLIB");

	/* DHCP parameters */
	if (token_get("CONFIG_MACH_PUMA5"))
	{
	    if (token_get("CONFIG_MACH_PUMA5_BVW3653"))
	    {
		token_set("CONFIG_VENDOR_NAME", "Hitron Technologies");
		token_set("CONFIG_VENDOR_ID", "0x0005CA");
		token_set("CONFIG_VENDOR_INFO", "PacketCable 1.5 EMTA");
		token_set("CONFIG_VENDOR_MODEL", "BVW-3653");
	    }
	    else if (token_get("CONFIG_MACH_PUMA5EVM"))
	    {
		token_set("CONFIG_VENDOR_INFO", "DOCSIS 3.0 Cable Modem");
		token_set("CONFIG_VENDOR_NAME", "Texas Instruments CBC Ltd.");
		token_set("CONFIG_VENDOR_MODEL", "TNETC550");
		token_set("CONFIG_VENDOR_ID", "0x0050F1");
	    }
	    else if (token_get("CONFIG_MACH_PUMA5_MVG3420N"))
	    {
		token_set("CONFIG_VENDOR_INFO", "DOCSIS 3.0 Cable Modem");
		token_set("CONFIG_VENDOR_NAME", "Mototech");
		token_set("CONFIG_VENDOR_MODEL", "MVG3420N");
		token_set("CONFIG_VENDOR_ID", "0x0050BF");
	    }
	    else
		conf_err("ERROR: Unknown PUMA5 DOCSIS Modem\n");
	}
	else
	    conf_err("ERROR: Unknown DOCSIS Modem\n");

	token_set("CONFIG_RG_INSTALLATION_WIZARD", "n");
    }


    if (token_get("CONFIG_RG_PACKET_CABLE"))
    {
	/* TI configs */
	token_set_y("CONFIG_TI_PACM");
	token_set_y("CONFIG_TI_PACM_IF_DOCSIS");
	token_set("CONFIG_TI_PACM_MTA_INTERFACE", "mta0");
	token_set_y("CONFIG_TI_PACM_VOIM_PROXY");
	token_set_y("CONFIG_TI_PACM_DOIM_DOCSIS_IF");
	token_set_y("CONFIG_TI_PACM_SECURITY");
	
	token_set_y("CONFIG_TI_PACM_SERVICES");
	token_set_y("CONFIG_TI_PACM_SNMP");
	token_set_y("CONFIG_TI_PACM_VENDOR_APP");
	token_set_y("CONFIG_TI_PACM_PROVISION");
	token_set_y("CONFIG_TI_PACM_PROV_CFM");
	token_set_y("CONFIG_TI_PACM_PROV_DHCP");
	token_set_y("CONFIG_TI_PACM_DEBUG");
	token_set("CONFIG_TI_PACM_MAX_NUMBER_OF_ENDPOINTS","4");

	token_set_y("CONFIG_TI_VOICE_CLIENT");
	token_set_y("CONFIG_TI_VOICE_NCSDK");

	/* TI configs */
	token_set_y("CONFIG_TI_VOICEAPP");
	token_set_y("CONFIG_TI_MASDK");
	token_set_y("CONFIG_TI_MASDK_CORE_OPTIONAL_LIBS_SUPPORT");
	token_set_y("CONFIG_TI_MASDK_MSU");
	token_set_y("CONFIG_TI_MASDK_RTCP");	
	token_set_y("CONFIG_TI_MASDK_PMA");

	token_set("CONFIG_TI_MASDK_DSPIMAGE", "c21.ld");

	token_set_y("CONFIG_TI_MASDK_PFORM_TIUHAL_TI_SOC");
	token_set_y("CONFIG_TI_MASDK_PFORM_TIU_ANALOG_TID_SUPPORT");
	token_set_y("CONFIG_VP880_ZARLINK_TID_SUPPORT");

	token_set_y("CONFIG_VP880_ZARLINK_DUAL_FXS_TRKR");
	token_set_y("CONFIG_VP880_TRKR_2FXS_PTC014_TID_SUPPORT");

	token_set_y("CONFIG_SILABS_TID_SUPPORT");
	token_set_y("CONFIG_SILABS_PROSLIC_TID_SUPPORT");
	token_set_y("CONFIG_DUAL_SI3210_TID_SUPPORT");
	token_set_y("CONFIG_SILABS_SI321X_HV_TID_SUPPORT");

	token_set_y("CONFIG_TI_MASDK_VQM");

	/* IPsec support */
	token_set_y("CONFIG_TI_IPSEC_TOOLS");
	token_set_y("CONFIG_RG_FLEX");
	token_set_y("CONFIG_INET_ESP");
	token_set_y("CONFIG_XFRM");
	token_set_y("CONFIG_XFRM_USER");
	token_set_y("CONFIG_INET_XFRM_MODE_TRANSPORT");
	token_set_y("CONFIG_INET_XFRM_MODE_TUNNEL");
	token_set_y("CONFIG_NET_IPIP");
	token_set_y("CONFIG_INET_TUNNEL");
	token_set_y("CONFIG_CRYPTO");
	token_set_y("CONFIG_CRYPTO_ALGAPI");
	token_set_y("CONFIG_CRYPTO_HASH");
	token_set_y("CONFIG_CRYPTO_HMAC");
	token_set_y("CONFIG_CRYPTO_NULL");
	token_set_y("CONFIG_CRYPTO_MD5");
	token_set_y("CONFIG_CRYPTO_SHA1");
	token_set_y("CONFIG_CRYPTO_DES");
	token_set_y("CONFIG_CRYPTO_AES");
    }

    if (token_get("CONFIG_MAGIC_SYSRQ") && token_get("CONFIG_BCM963XX"))
	token_set_y("CONFIG_BCM_SERIAL_CONSOLE");
	
    /* IPV6 support */
    if (token_get("CONFIG_RG_IPV6"))
    {
  	token_set_y("CONFIG_IPV6");
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    /* support route information when receiving router advertisements */
	    token_set_y("CONFIG_IPV6_ROUTER_PREF");
	    token_set_y("CONFIG_IPV6_ROUTE_INFO");
	}
	if (token_get("CONFIG_RG_OS_LINUX_26") &&
	    token_get("CONFIG_RG_IPV6_OVER_IPV4_TUN"))
	{
	    token_set_y("CONFIG_IPV6_SIT");
	    token_set_y("CONFIG_INET_TUNNEL");
	}
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	    token_set_y("CONFIG_IPV6_RESTRICTED_DOUBLE_BIND");
	token_set_y("CONFIG_RG_FLEX");
	token_set_y("CONFIG_RG_RADVD");
    }

    if (token_get("CONFIG_RG_PERM_STORAGE") &&
	!token_get("CONFIG_RG_FLASHDISK") && !token_get("CONFIG_MTD") &&
	!token_get("CONFIG_RG_UML") && !IS_DIST("JPKG_UML"))
    {
	token_set_m("CONFIG_FLASH");
    }

    /* DOS gcc cannot optimize */
    if (!token_get("CONFIG_GCC_CANNOT_OPTIMIZE"))
	token_set_y("CONFIG_GCC_CAN_INLINE");
    
    if (token_get("CONFIG_RG_DHCPS"))
    {
	token_set_y("CONFIG_RG_PING");
	token_set_y("CONFIG_SYSCTL");
    }

    if (token_get("CONFIG_RG_OS_LINUX_24"))
    {
	if (token_get("CONFIG_RG_NETLINK_LIBNL") ||
	    token_get("CONFIG_RG_DHCPS"))
	{
	    /* ARPD is required in order to receive neighbor notifications */
	    token_set_y("CONFIG_ARPD");
	}
    }
    
    if (token_get("CONFIG_RG_8021X_TLS") || token_get("CONFIG_RG_8021X_TTLS") ||
	token_get("CONFIG_RG_8021X_MD5") || token_get("CONFIG_RG_8021X_RADIUS"))
    {
	token_set_y("CONFIG_RG_8021X");
    }

    if (token_get("CONFIG_RG_8021X"))
	token_set_m("CONFIG_RG_8021X_PKTFIL");
    if (token_get("CONFIG_FREESWAN") ||
       	token_get("CONFIG_RG_8021X_TLS") ||
       	token_get("CONFIG_RG_8021X_TTLS"))
    {
	token_set_y("CONFIG_RG_X509");
    }
    
    if (token_get("CONFIG_RG_SSL") || token_get("CONFIG_RG_X509"))
	token_set_y("CONFIG_RG_CERT");

    if (token_get("CONFIG_RG_RMT_FW_CONF") || 
	token_get("CONFIG_RG_UCD_SNMP_V3") || 
	token_get("CONFIG_RG_MAIL_SERVER") ||
	token_get("CONFIG_RG_JNET_CLIENT") ||
	token_get("CONFIG_RG_RMT_UPDATE"))
    {
	token_set_y("CONFIG_RG_CRYPTO");
    }

    if (token_get("CONFIG_RG_CERT") || token_get("CONFIG_RG_CRYPTO"))
	token_set_y("CONFIG_RG_OPENSSL");

    if (token_get("CONFIG_RG_OPENSSL"))
	token_set_y("CONFIG_RG_CRYPTO");

    if (token_get("CONFIG_RG_UPNP_AV"))
	token_set_y("CONFIG_RG_SQLITE");

    if (token_get("CONFIG_RG_ACCESS_DLNA"))
    {
	token_set_y("CONFIG_RG_SQLITE");
	token_set_y("CONFIG_RG_LIBJPEG");
    }

    if (token_get("CONFIG_RG_IGD") || token_get("CONFIG_RG_TR_064") ||
	token_get("CONFIG_RG_UPNP_AV") || token_get("CONFIG_RG_UPNP_SNIFFER"))
    {
	token_set_y("CONFIG_RG_UPNP");
    }

    if (token_get("CONFIG_RG_UPNP") || token_get("CONFIG_RG_DSLHOME"))
    {
	token_set_y("CONFIG_RG_XML");
	token_set_y("CONFIG_RG_MGT_IGD");
    }

    if (token_get("CONFIG_OPENRG") && token_get("CONFIG_RG_WBM"))
    {
	token_set_y("CONFIG_RG_SESSION_MEMORY");
	token_set_y("CONFIG_RG_WBM_JS_DOWNLOAD");
	token_set_y("CONFIG_RG_WBM_UNAUTHENTICATED_ACCESS");
    }

    if (token_get("CONFIG_OPENRG") &&
	(token_get("CONFIG_RG_WBM") || token_get("CONFIG_RG_SSI")))
    {
	token_set_y("CONFIG_RG_HTTPS");
	token_set_y("CONFIG_RG_LIBIMAGE_DIM");
	/* Add all themes if no theme was added previously */
	if (!is_gui_selected())
	    select_default_themes();
    }

    if (token_get("CONFIG_RG_LIBIMAGE_DIM"))
	token_set_y("CONFIG_RG_LIBIMAGE");

    if (token_get("CONFIG_RG_SSI"))
	token_set_y("CONFIG_RG_HTTPS");

    if (token_get("CONFIG_RG_HTTPS"))
	token_set_y("CONFIG_HTTP_CGI_PROCESS");

    if (token_get("CONFIG_RG_ALG_MSNMS"))
	token_set_y("CONFIG_RG_ALG_SIP");

    if (token_get("CONFIG_RG_PPP_DEFLATE"))
    {
	token_set_y("CONFIG_ZLIB_INFLATE");
	token_set_y("CONFIG_ZLIB_DEFLATE");
    }

    if (token_get("CONFIG_RG_FFS") && !token_get_str("CONFIG_RG_FFS_MNT_DIR"))
	token_set("CONFIG_RG_FFS_MNT_DIR", "/mnt/ffs");

    if (token_get("CONFIG_JFFS2_FS"))
    {
	if (!strcmp(os, "LINUX_26"))
	    token_set_y("CONFIG_JFFS2_FS_WRITEBUFFER");
	token_set_y("CONFIG_JFFS2_ZLIB");
	token_set_y("CONFIG_JFFS2_RTIME");	
	token_set_y("CONFIG_JFFS2_SUMMARY");
    }
    if (token_get("CONFIG_JFFS2_ZLIB"))
    {
	token_set_y("CONFIG_ZLIB_DEFLATE");
	token_set_y("CONFIG_ZLIB_INFLATE");
    }

    if (token_get("CONFIG_RG_TZ_COMMON") || token_get("CONFIG_RG_TZ_FULL"))
    {
	token_set_y("CONFIG_RG_AUTO_DST");
	if (!token_get("CONFIG_RG_TZ_YEARS"))
	    token_set("CONFIG_RG_TZ_YEARS", "7");
    }

    /* If a firewall feature is requested, enable one of the possible firewall
     * modules.
     */
    if (token_get("CONFIG_RG_FIREWALL") || token_get("CONFIG_RG_NAT") ||
	token_get("CONFIG_RG_RNAT"))
    {
	token_set_y("CONFIG_RG_GENERIC_PROXY");
	token_set_y("CONFIG_RG_ALG_USERSPACE");
	token_set_m("CONFIG_RG_JFW");
	token_set_y("CONFIG_RG_NETOBJ");
    }
    else if (token_get("CONFIG_RG_WEB_AUTH") && !token_get("CONFIG_RG_JPKG"))
	conf_err("Preconditions for CONFIG_RG_WEB_AUTH are not fulfilled\n");

    if (token_get("CONFIG_ZERO_CONFIGURATION"))
	token_set_y("CONFIG_ZC_AUTO_CONFIG_NON_PNP");

    if (token_get("CONFIG_RG_WPA"))
	token_set_y("CONFIG_RG_WPA_WBM");

    if (token_get("CONFIG_RG_8021X"))
	token_set_y("CONFIG_RG_8021X_WBM");

    if (token_get("CONFIG_RG_USB_SLAVE"))
    {
	token_set_m("CONFIG_RG_RNDIS");
	token_set_y("CONFIG_USB_RNDIS");
    }

    if (token_get("CONFIG_PRISM2_PCI"))
    {
	token_set_y("CONFIG_INTERSIL_COMMON");
        target_os_enable_wireless();
	token_set_y("CONFIG_PRISM2_LOAD_FIRMWARE");
    }

    if (token_get("CONFIG_ISL38XX"))
    {
	token_set_y("CONFIG_INTERSIL_COMMON");
        target_os_enable_wireless();
	token_set_y("INTERSIL_EVENTS");
    }

    if (token_get("CONFIG_ISL_SOFTMAC"))
    {
	token_set_y("CONFIG_INTERSIL_COMMON");
        target_os_enable_wireless();
    }

    if (token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_RG_BOOTSTRAP");
    
    if (token_get("CONFIG_RG_FOOTPRINT_REDUCTION"))
    {
	token_set_y("CONFIG_RG_KERNEL_NEEDED_SYMBOLS");
	token_set_y("CONFIG_RG_MKLIBS");
	if (token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_SMALL_FLASH");
    }

    if (token_get("CONFIG_RG_OS_LINUX_26"))
    {
	if (!token_get("CONFIG_RG_RGLOADER"))
            token_set_y("CONFIG_RG_LARGE_FILE_SUPPORT");
	token_set_y("CONFIG_RG_MAINFS");

	if (token_get("CONFIG_RG_INITFS_RAMFS"))
	{
	    token_set("CONFIG_INITRAMFS_SOURCE", "$(RAMDISK_MOUNT_POINT)");
	    token_set("CONFIG_INITRAMFS_ROOT_UID", "$(shell id -u)");
	    token_set("CONFIG_INITRAMFS_ROOT_GID", "$(shell id -g)");
	}

	token_set_y("CONFIG_CRAMFS_FS");

	if (token_get("CONFIG_RG_MODFS_CRAMFS"))
	{
	    token_set_y("CONFIG_CRAMFS_FS");
	    token_set_y("CONFIG_RG_MODFS");
	}
	
	if (token_get("CONFIG_CRAMFS_FS"))
	{
	    if (!token_get("CONFIG_RG_JPKG_SRC"))
		token_set_y("CONFIG_CRAMFS_FS_COMMON");

	    token_set_y("CONFIG_CRAMFS");
	    token_set_y("CONFIG_ZLIB_INFLATE");

	    /* Optimized CRAMFS compression, a little slower. */
	    token_set_y("CONFIG_CRAMFS_DYN_BLOCKSIZE");

	    if (!token_get_str("CONFIG_RG_CRAMFS_COMP_METHOD"))
	    {
		if (token_get("CONFIG_RG_NONCOMPRESSED_USERMODE_IMAGE"))
		    token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "none");
		else
		    token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "lzma");
	    }

	    if (token_get("CONFIG_CRAMFS_DYN_BLOCKSIZE"))
	    {
		if (!strcmp(token_get_str("CONFIG_RG_CRAMFS_COMP_METHOD"),
		    "lzma"))
		{
		    token_set("CONFIG_CRAMFS_BLKSZ", "65536");
		}
		else
		    token_set("CONFIG_CRAMFS_BLKSZ", "32768");
	    }

	}	
	if (!token_get("CONFIG_RG_RGLOADER"))
	    token_set_y("CONFIG_DEBUG_USER");
	token_set_y("CONFIG_BLK_DEV_INITRD");
    }

    if (token_get("CONFIG_RG_OS_LINUX"))
    {
	/* PCAP Socket type for libpcap/tcpdump */
	token_set_y("CONFIG_RG_PCAP_SOCKET");
    }


    if (token_get("CONFIG_RG_OS_LINUX_24"))
    {
	if (token_get("CONFIG_SIMPLE_RAMDISK"))
	    token_set_y("CONFIG_RG_INITFS_RAMDISK");

	if (!token_get("CONFIG_RG_INITFS_RAMDISK"))
	    token_set_y("CONFIG_RG_ROOTFS_TMPFS");

	if (token_get("CONFIG_RG_MODFS"))
	{
	    token_set_y("CONFIG_ROMFS_FS");
	    token_set_y("CONFIG_RG_MODFS_ROMFS");
	}

	if (token_get("CONFIG_RG_ROOTFS_TMPFS"))
	{
	    token_set_y("CONFIG_TMPFS");
	    token_set_y("CONFIG_RG_INITFS_CRAMFS");
	}

	token_set_y("CONFIG_RG_LARGE_FILE_SUPPORT");

	if (token_get("CONFIG_RG_INITFS_RAMDISK"))
	{
	    token_set_y("CONFIG_BLK_DEV_INITRD");
	    token_set_y("CONFIG_BLK_DEV_RAM");
	    token_set_y("CONFIG_RG_ROOTFS_RAMDISK");
	    if (!token_get("CONFIG_RG_UML"))
		token_set_y("CONFIG_EMBEDDED_RAMDISK");
	    if (!token_get("CONFIG_RG_RGLOADER") ||
		token_get("CONFIG_RG_JPKG"))
	    {
		token_set_y("CONFIG_EXT2_FS");
	    }
	}

	if (token_get("CONFIG_CRAMFS_FS"))
	{
	    token_set_y("CONFIG_CRAMFS_FS_COMMON");

	    token_set_y("CONFIG_CRAMFS");
	    token_set_y("CONFIG_ZLIB_INFLATE");

	    /* Optimized CRAMFS compression, a little slower. */
	    token_set_y("CONFIG_CRAMFS_DYN_BLOCKSIZE");

	    if (!token_get_str("CONFIG_RG_CRAMFS_COMP_METHOD"))
	    {
		if (token_get("CONFIG_RG_NONCOMPRESSED_USERMODE_IMAGE"))
		    token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "none");
		else
		    token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "lzma");
	    }

	    if (token_get("CONFIG_CRAMFS_DYN_BLOCKSIZE"))
	    {
		if (!strcmp(token_get_str("CONFIG_RG_CRAMFS_COMP_METHOD"),
		    "lzma"))
		{
		    token_set("CONFIG_CRAMFS_BLKSZ", "65536");
		}
		else
		    token_set("CONFIG_CRAMFS_BLKSZ", "32768");
	    }
	}
    }

    if (token_get("CONFIG_CRAMFS_FS") && token_get("CONFIG_PROC_FS"))
    {
	token_set_y("CONFIG_RG_CRAMFS_MONITOR");
	token_set("CONFIG_RG_CRAMFS_MONITOR_FILE", 
	    "fs/cramfs_block_uncompressed");
    }

    {
	char *kernel_comp_method;
	
	/* Figure out the kernel compression method needed config. */
	kernel_comp_method = 
	    token_get_str("CONFIG_RG_KERNEL_COMP_METHOD") ? : "";
	
	if (!strcmp(kernel_comp_method, "lzma"))
	    token_set_y("CONFIG_RG_LZMA_COMPRESSED_KERNEL");
	if (!strcmp(kernel_comp_method, "gzip"))
	    token_set_y("CONFIG_RG_GZIP_COMPRESSED_KERNEL");
	if (!strcmp(kernel_comp_method, "bzip2"))
	    token_set_y("CONFIG_RG_BZIP2_COMPRESSED_KERNEL");

	if (token_get("CONFIG_RG_BZIP2_COMPRESSED_KERNEL"))
	    token_set_y("CONFIG_RG_BZIP2");
    }
    
    if (token_get("CONFIG_HW_USB_HOST_UHCI"))
    {
	 if (!token_get("CONFIG_RG_OS_LINUX_26"))
	 {
	     token_set_y("CONFIG_RG_USB");
	     token_set("CONFIG_USB_UHCI",
		 token_get_str("CONFIG_HW_USB_HOST_UHCI"));
	 }
	 else
	 {
	     token_set_y("CONFIG_USB");
	     token_set("CONFIG_USB_UHCI_HCD",
		 token_get_str("CONFIG_HW_USB_HOST_UHCI"));
	 }
	token_set_y("CONFIG_USB_DEVICEFS");
    }

    if (token_get("CONFIG_HW_USB_HOST_OHCI"))
    {
	if (!token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_RG_USB");
	    token_set("CONFIG_USB_OHCI",
		token_get_str("CONFIG_HW_USB_HOST_OHCI"));
	}
	else
	{
	    token_set_y("CONFIG_USB");
	    token_set("CONFIG_USB_OHCI_HCD",
		token_get_str("CONFIG_HW_USB_HOST_OHCI"));
	}
	token_set_y("CONFIG_USB_DEVICEFS");
    }

    if (token_get("CONFIG_HW_USB_HOST_EHCI"))
    {
	if (!token_get("CONFIG_RG_OS_LINUX_26"))
	    token_set_y("CONFIG_RG_USB");
	else
	    token_set_y("CONFIG_USB");
	token_set_y("CONFIG_USB_DEVICEFS");
	token_set("CONFIG_USB_EHCI_HCD",
	    token_get_str("CONFIG_HW_USB_HOST_EHCI"));
    }

    if (token_get("CONFIG_RG_WEBCAM"))
    {
        token_set_y("CONFIG_RG_LIBJPEG");
        token_set_y("CONFIG_HW_CAMERA_USB_PWC");
        token_set_y("CONFIG_HW_CAMERA_USB_OV511");
        token_set_y("CONFIG_HW_CAMERA_USB_SPCA5XX");
    }

    if (token_get("CONFIG_HW_FIREWIRE"))
    {
	token_set("CONFIG_IEEE1394", token_get_str("CONFIG_HW_FIREWIRE"));
	token_set("CONFIG_IEEE1394_OHCI1394",
	    token_get_str("CONFIG_HW_FIREWIRE"));
    }

    if (token_get("CONFIG_HW_USB_STORAGE"))
	token_set("CONFIG_USB_STORAGE", token_get_str("CONFIG_HW_USB_STORAGE"));

    /* Currently SBP2 is only supported as module */
    if (token_get("CONFIG_HW_FIREWIRE_STORAGE"))
	token_set_m("CONFIG_IEEE1394_SBP2");

    if (token_get("CONFIG_HW_UML_LOOP_STORAGE"))
    {
	token_set_y("CONFIG_BLK_DEV_LOOP");
	token_set("CONFIG_RG_DISK_EMULATION_COUNT", "4");
    }

    if (token_get("CONFIG_RG_EVENT_LOGGING"))
    {
    	token_set_y("CONFIG_RG_SYSLOG");
	token_set_m("CONFIG_RG_LOG_DEV");
	token_set("CONFIG_LOG_FILES_MIN_SIZE", "12288");
	token_set("CONFIG_LOG_FILES_MAX_SIZE", "16384");
	token_set_y("CONFIG_RG_KERN_LOG");
	token_set_y("CONFIG_SYSLOG_UNIXSOCK_SUPPORT");
	token_set_y("CONFIG_RG_SYSLOG_REMOTE"); 
    }
    if (token_get("CONFIG_RG_FW_ICSA"))
    {
	/* 128 KB */
	token_set("CONFIG_RG_LOG_DEV_BUF_SIZE", "131072");
	token_set("CONFIG_RG_SYSLOG_FW_DEF_SIZE", "131072");
	/* Secured transmission Secured Login; HTTP-S; Telnet-S */
	token_set_y("CONFIG_RG_SSL");
    }
    else if (token_get("CONFIG_RG_SYSLOG") &&
	!token_get("CONFIG_RG_LOG_DEV_BUF_SIZE"))
    {
	/* 16 KB */
	token_set("CONFIG_RG_LOG_DEV_BUF_SIZE", "16384");
	token_set("CONFIG_RG_SYSLOG_FW_DEF_SIZE", "16384");
    }

    if (!token_get("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY") &&
	(token_get("CONFIG_RG_JPKG") ||
	token_get("CONFIG_GLIBC") || 
	(token_get("CONFIG_ULIBC") && (token_get("ULIBC_IN_TOOLCHAIN") ||
	token_get("LIBC_IN_TOOLCHAIN")))))
    {
	/* There is a bug (B28967) in glibc/ulibc, which is fixed in pkg/ulibc.
	 * Workaround this bug in other libcs (glibc, or ulibc from toolchain) 
	 * by using code from this glibc directory.
	 */
	token_set_y("CONFIG_RG_SYSLOG_GLIBC");
    }
    if (token_get("CONFIG_RG_SYSLOG") || token_get("CONFIG_RG_SYSLOG_GLIBC"))
	token_set_y("CONFIG_RG_SYSLOG_COMMON");

    /* OV511 Video Camera */
    if (token_get("CONFIG_HW_CAMERA_USB_OV511"))
    {
	token_set("CONFIG_USB_OV511", 
	    token_get_str("CONFIG_HW_CAMERA_USB_OV511"));
    }

    /* PWC Video Camera */
    if (token_get("CONFIG_HW_CAMERA_USB_PWC"))
        token_set("CONFIG_USB_PWC", token_get_str("CONFIG_HW_CAMERA_USB_PWC"));

    /* SPCA5XX Video Camera */
    if (token_get("CONFIG_HW_CAMERA_USB_SPCA5XX"))
    {
	token_set("CONFIG_USB_SPCA5XX",
	    token_get_str("CONFIG_HW_CAMERA_USB_SPCA5XX"));
    }

    /* Video Device */
    if (token_get("CONFIG_HW_CAMERA_USB_OV511") ||
        token_get("CONFIG_HW_CAMERA_USB_PWC") ||
        token_get("CONFIG_HW_CAMERA_USB_SPCA5XX"))
    {
	token_set_y("CONFIG_VIDEO_DEV");
	token_set_y("CONFIG_VIDEO_PROC_FS");
    }

    /* PCMCIA/Cardbus support */
    if (token_get("CONFIG_HW_PCMCIA_CARDBUS"))
    {
	token_set_y("CONFIG_HOTPLUG");
	token_set_y("CONFIG_CARDBUS");
	token_set("CONFIG_PCMCIA", token_get_str("CONFIG_HW_PCMCIA_CARDBUS"));
    }

    /* Firewire IEEE1394 and USB storage require SCSI */
    if (token_get("CONFIG_IEEE1394_SBP2") || token_get("CONFIG_USB_STORAGE"))
    {
	token_set_y("CONFIG_SCSI");
	token_set_y("CONFIG_SCSI_MULTI_LUN");
	token_set_y("CONFIG_BLK_DEV_SD");
	token_set("CONFIG_SD_EXTRA_DEVS", "32");
    }

    if (token_get("CONFIG_RG_BLUETOOTH"))
    {
	if (token_get("CONFIG_RG_OS_LINUX_24"))
	{
	    token_set_y("CONFIG_BLUEZ");
	    token_set_y("CONFIG_BLUEZ_L2CAP");
	    token_set_y("CONFIG_BLUEZ_RFCOMM");
	    token_set_y("CONFIG_BLUEZ_SCO");
	    token_set_y("CONFIG_BLUEZ_BNEP");
	    token_set_y("CONFIG_BLUEZ_BNEP_MC_FILTER");
	    token_set_y("CONFIG_BLUEZ_BNEP_PROTO_FILTER");

	    if (token_get("CONFIG_HW_USB_HOST_OHCI") ||
		token_get("CONFIG_HW_USB_HOST_UHCI"))
	    {
		token_set_m("CONFIG_BLUEZ_HCIUSB");
	    }
	}
	
	if (token_get("CONFIG_RG_OS_LINUX_26"))
	{
	    token_set_y("CONFIG_BT");
	    token_set_y("CONFIG_BT_L2CAP");
	    token_set_y("CONFIG_BT_RFCOMM");
	    token_set_y("CONFIG_BT_SCO");
	    token_set_y("CONFIG_BT_BNEP");
	    token_set_y("CONFIG_BT_BNEP_MC_FILTER");
	    token_set_y("CONFIG_BT_BNEP_PROTO_FILTER");
	}

	token_set_y("CONFIG_RG_BLUETOOTH_PAN");
	token_set_y("CONFIG_HOTPLUG");
    }

    if (token_get_str("CONFIG_RG_CODEPAGE"))
    {
	token_set_y("CONFIG_NLS");
	token_set_y("CONFIG_NLS_CODEPAGE_437");
	token_set_y("CONFIG_NLS_CODEPAGE_850");
	token_set_y("CONFIG_NLS_UTF8");
	token_set_y("CONFIG_NLS_ISO8859_1");
	token_set("CONFIG_NLS_DEFAULT",
	    token_get_str("CONFIG_FAT_DEFAULT_IOCHARSET"));
    }
    else
	token_set("CONFIG_RG_CODEPAGE", "\"\"");

    /* Software reset/restore_defaults */
    if (token_get("CONFIG_HW_BUTTONS"))
	token_set_m("CONFIG_RG_HW_BUTTONS");

    /* limit maximum size of EXT2 partition for running check disk
     * on machines with 32MB RAM or less */
    if (token_get("CONFIG_RG_DISK_MNG"))
    {
	char *size;

	if ((size = token_get_str("CONFIG_SDRAM_SIZE")) && atoi(size) <= 32)
	{
	    token_set("CONFIG_RG_MAX_FAT_CHECK_SIZE", "1");
	    token_set("CONFIG_RG_MAX_EXT2_CHECK_SIZE", "80");
	}
    }

    if (token_get("CONFIG_RG_RGLOADER"))
    {
	token_set_m("CONFIG_RG_KOS");
	
	/* We need it for insmod */
	token_set_y("CONFIG_RG_BUSYBOX");
	
	/* XXX - shouldnt be required for RGLoader */
	token_set_y("CONFIG_RG_LANG");
	token_set("CONFIG_RG_LANGUAGES", "DEF");
	token_set("CONFIG_RG_DIST_LANG", "eng");
    }

    if (token_get("CONFIG_RG_RGLOADER") || 
	token_get("CONFIG_RG_RGLOADER_CLI_CMD"))
    {
	token_set_y("CONFIG_RGLOADER_CLI");
	token_set_y("CONFIG_RG_CLI");
	token_set_y("CONFIG_RG_PERM_STORAGE");
        token_set_y("CONFIG_RG_PING");
	token_set_y("CONFIG_RG_CLI_SERIAL");

	token_set_y("CONFIG_RG_WGET");
    }

    if (token_get("CONFIG_RG_CLI") && token_get("CONFIG_RG_HTTPS"))
	token_set_y("CONFIG_RG_CLI_CGI");

    if (is_dsp_configurable())
    {
	token_set_y("CONFIG_RG_AUDIO_MGT");
    }

    /* Set the default language used for this distribution */
    if (!token_get_str("CONFIG_RG_DIST_LANG"))
	token_set("CONFIG_RG_DIST_LANG", "eng");
	
    if (!token_get_str("RG_LAN_IP_SUBNET"))
	token_set("RG_LAN_IP_SUBNET", "1");

    if (token_get("CONFIG_RG_FLASH_ONLY_ON_BOOT"))
	token_set_y("CONFIG_RG_RMT_UPGRADE_IMG_IN_MEM");

    if (token_get("CONFIG_ULIBC") && token_get("CONFIG_DYN_LINK"))
	token_set_y("CONFIG_ULIBC_SHARED");

    if (token_get("CONFIG_RG_JFW") || token_get("CONFIG_RG_PPTPC"))
	token_set_m("CONFIG_RG_FRAG_CACHE");

    if (token_get("CONFIG_RG_ATHEROS"))
    {
	token_set_y("CONFIG_CRYPTO");
        token_set_y("CONFIG_CRYPTO_HASH");
        token_set_y("CONFIG_CRYPTO_ALGAPI");
	token_set_y("CONFIG_CRYPTO_HMAC");
	token_set_y("CONFIG_CRYPTO_MD5");
	token_set_y("CONFIG_CRYPTO_AES");
	token_set_y("CONFIG_RG_HOSTAPD");
	token_set_y("CONFIG_HOSTAPD_DRIVER_MADWIFI");
	token_set_y("CONFIG_HOSTAPD_RSN_PREAUTH");
    }

    if (token_get("CONFIG_MV_WIFI_8764") || token_get("CONFIG_MV_WIFI_8864"))
    {
	token_set_y("CONFIG_RG_HOSTAPD");
	token_set_y("CONFIG_HOSTAPD_DRIVER_MARVELL");
	token_set_y("CONFIG_HOSTAPD_V_0_8");
	token_set_y("CONFIG_RG_WIRELESS_TOOLS");
    }

    if (token_get("CONFIG_RG_PPP_COMMON") || token_get("CONFIG_RG_8021X") ||
	token_get("CONFIG_RG_RADIUS_CLIENT") ||
	token_get("CONFIG_RG_RADIUS_SERVER"))
    {
	token_set_y("CONFIG_RG_AUTH");
    }
    if (!token_get("CONFIG_LSP_DIST") && hw)
    {
	token_set_y("CONFIG_RG_TZ");
	token_set_y("CONFIG_RG_MGT");

	/* decide inside openssl what exactly we want:
	 * since PPP needs 4 header files from openssl */
	token_set_y("CONFIG_RG_OPENSSL_COMMON");
    }
    if (token_get("CONFIG_RG_OPENSSL_COMMON"))
    {
	token_set_y("CONFIG_RG_OPENSSL_MD5");
	if (!token_get("CONFIG_RG_RGLOADER"))
	{
	    token_set_y("CONFIG_RG_OPENSSL_SHA");
	    token_set_y("CONFIG_RG_OPENSSL_DES");
	    token_set_y("CONFIG_RG_OPENSSL_MD4");
	}
    }
    if (token_get("CONFIG_RG_TARGET_LINUX"))
	token_set_y("CONFIG_RG_LZMA");
    if (token_get("CONFIG_RG_CABLEHOME") || token_get("CONFIG_RG_DSL_CH"))
	token_set_y("CONFIG_RG_CABLEHOME_COMMON");
    if (token_get("CONFIG_RG_TCPDUMP"))
    {
	token_set_y("CONFIG_RG_LIBPCAP");
	token_set_y("CONFIG_RG_TERMCAP");
    }
    if (token_get("CONFIG_RG_DHCPC") || token_get("CONFIG_RG_DHCPS") ||
	token_get("CONFIG_RG_DHCPR"))
    {
	token_set_y("CONFIG_RG_DHCP_COMMON");
    }
    if (token_get("CONFIG_RG_DHCPV6C") || token_get("CONFIG_RG_DHCPV6S"))
	token_set_y("CONFIG_RG_DHCPV6_COMMON");
    if (token_get("CONFIG_RG_RGLOADER") || 
	token_get("CONFIG_RG_RGLOADER_CLI_CMD"))
    {
	token_set_y("CONFIG_RG_RGLOADER_COMMON");
    }
    if (token_get("CONFIG_FREESWAN") || token_get("CONFIG_LIBDES"))
	token_set_y("CONFIG_FREESWAN_COMMON");
    if (token_get("CONFIG_RG_PING") ||
	token_get("CONFIG_RG_CONN_SPEED") ||
	token_get("CONFIG_RG_TRACEROUTE") ||
	token_get("CONFIG_RG_TCP_DETECT") ||
	token_get("CONFIG_RG_ARP_PING") )
    {
	token_set_y("CONFIG_RG_TEST_TOOLS");
    }
    if (token_get("CONFIG_RG_FILESERVER_ACLS"))
    {
	token_set_y("CONFIG_RG_ATTR");
	token_set_y("CONFIG_RG_ACL");
    }
    if (token_get("CONFIG_RG_FS_BACKUP"))
    {
	token_set_y("CONFIG_RG_STAR");
	token_set_y("CONFIG_RG_BACKUP");
    }
    if (token_get("CONFIG_RG_SAMPLES") || token_get("CONFIG_RG_TUTORIAL") || 
	token_get("CONFIG_RG_TUTORIAL_ADVANCED") || 
	token_get("CONFIG_GUI_TUTORIAL"))
    {
	token_set_y("CONFIG_RG_SAMPLES_COMMON");
    }

    if (token_get("CONFIG_RG_SPEED_TEST"))
	token_set_y("CONFIG_RG_IPERF");

    if (token_get("CONFIG_RG_IPERF"))
	token_set_y("CONFIG_RG_CXX");

    if (token_get("CONFIG_RG_VOIP_MEDIA5"))
    {
	token_set_y("CONFIG_RG_CXX");
	token_set_y("CONFIG_RG_VOIP_SIP");
    }

    if (token_get("CONFIG_RG_CXX") && token_get("CONFIG_ULIBC"))
	token_set_y("CONFIG_UCLIBCXX");

    if (token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_RG_TELNETS");

    if (token_get("CONFIG_RG_SSH"))
    {
	token_set_y("CONFIG_RG_CLI_SERVER");
	token_set_y("CONFIG_RG_OPENSSL_BLOWFISH");
	token_set_y("CONFIG_RG_OPENSSL_CAST");
    }

    if (token_get("CONFIG_RG_JPKG_BIN") &&
	token_get("CONFIG_HW_80211N_AIRGO_AGN100"))
    {
	token_set_y("CONFIG_RG_JPKG_BUILD_LIBC");
    }

    if (!token_get_str("RMT_UPG_SITE"))
	token_set("RMT_UPG_SITE", "update.jungo.com");

    token_set("RMT_UPG_URL", "http%s://%s/",
	token_get("CONFIG_RG_SSL") ? "s" : "", token_get_str("RMT_UPG_SITE"));
    
    if (token_get("CONFIG_RG_RMT_UPDATE"))
    {
        if (token_get("CONFIG_RG_JPKG"))
	{
	    token_set_y("CONFIG_RG_RMT_UPD_MD5");
	    token_set_y("CONFIG_RG_RMT_UPD_RSA");
	}
	if (!token_get("CONFIG_RG_RMT_UPD_MD5"))
	    token_set_y("CONFIG_RG_RMT_UPD_RSA");
    }

    if (token_get("CONFIG_RG_JNET_CLIENT"))
    {
	/* XXX Remove this when B31487 is fixed */
	token_set("RMT_UPG_URL", "http://%s/", token_get_str("RMT_UPG_SITE"));
    }

    if (!token_get("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY"))
    {
	token_set_y("CONFIG_RG_ZLIB");
	token_set_y("CONFIG_RG_KERNEL");
	token_set_y("CONFIG_RG_MAIN");
	token_set_y("CONFIG_RG_VENDOR");
    }
    
    if (token_get("CONFIG_RG_MTD_DEFAULT_PARTITION"))
    {
	if (!token_get("CONFIG_RG_MTD") ||
	    !token_get("CONFIG_MTD_PHYSMAP_START") ||
	    !token_get("CONFIG_MTD_PHYSMAP_LEN"))
	{
	    conf_err("ERROR: Incorrect MTD configuration\n");
	}
    }

    if (token_get("CONFIG_RG_SSL_ROOT_CERTIFICATES") && 
	token_get("CONFIG_RG_ADV_SERVICES_LEVEL") > 1)
    {
	token_set_y("CONFIG_RG_SSL_ROOT_CERTIFICATES_FULL_LIST");
    }

    if (token_get("CONFIG_RG_RADIUS_CLIENT") || 
	token_get("CONFIG_RG_RADIUS_SERVER"))
    {
	token_set_y("CONFIG_RG_RADIUS_COMMON");
    }

    if (token_get("CONFIG_RG_CLI"))
	token_set_y("CONFIG_RG_CMD");

    if (token_get("CONFIG_RG_OS_LINUX_26"))
    {
	token_set_y("CONFIG_GENERIC_HARDIRQS");

	if (!token_get("CONFIG_FEROCEON_COMMON"))
	    token_set_y("CONFIG_TINY_SHMEM");

	if (!strcmp(token_getz("ARCH"), "arm"))
	{
	    /* required for all ARM-based platforms */
	    token_set_y("CONFIG_UID16");
	    token_set_y("CONFIG_TRACE_IRQFLAGS_SUPPORT");
	    token_set_y("CONFIG_GENERIC_HWEIGHT");
	    token_set_y("CONFIG_RTC_LIB");
	    token_set("CONFIG_VECTORS_BASE", "0xffff0000");
	    token_set_y("CONFIG_ZONE_DMA");
	}

	if (!strcmp(token_getz("ARCH"), "mips"))
	{
	    token_set_y("CONFIG_TRACE_IRQFLAGS_SUPPORT");
	    token_set_y("CONFIG_ZONE_DMA");
	    token_set_y("CONFIG_GENERIC_TIME");
	    token_set_y("CONFIG_GENERIC_FIND_NEXT_BIT");
	    token_set_y("CONFIG_GENERIC_HWEIGHT");
	}
    }

    /* Don't use this feature until B53229 is fixed
     * if (token_get("CONFIG_RG_SKB_CACHE"))
     *	token_set_y("CONFIG_LINUX_SKB_REUSE");
     */

    /* XXX: A hack until B48079 will be fixed */
    if (!token_get("CONFIG_RG_DISK_EMULATION_COUNT"))
	token_set("CONFIG_RG_DISK_EMULATION_COUNT", "4");

    if (token_get("CONFIG_RG_JNLP"))
	token_set_y("CONFIG_RG_XML");

    if (token_get("CONFIG_RG_HA_X10"))
	token_set_y("CONFIG_RG_HOME_AUTOMATION");

    /* RG KLOG Crash logger */
    if (token_get("CONFIG_RG_KLOG"))
    {
	token_set_m("CONFIG_RG_KLOG_RAM_BE");
	token_set_y("CONFIG_RG_KLOG_EMAIL");
	token_set("CONFIG_RG_KLOG_RAMSIZE", "0x040000");

	if (token_get("CONFIG_RG_KLOG_EMAIL"))
	{
	    /* Email notification */
	    token_set_y("CONFIG_RG_ENTFY");
	}

	if (token_is_y("CONFIG_RG_KLOG_RAM_BE"))
	{
	    /* Core must be compiled staticly when backend is static */
	    token_set_y("CONFIG_RG_KLOG");
	}
    }

    if (token_get("CONFIG_RG_JQUERY"))
    {
	token_set_y("CONFIG_RG_JSLINT");
	token_set_y("CONFIG_RG_JSMIN");
    }

    if (!token_get("CONFIG_RG_JPKG") && token_get("CONFIG_RG_VOIP") && 
	token_get("CONFIG_RG_REDUCE_SUPPORT") &&
	!token_get("CONFIG_RG_HOME_PBX"))
    {
	conf_err("ERROR: Cannot use a non HOME_PBX VOIP configuration "
	    "with CONFIG_RG_REDUCE_SUPPORT\n");
    }

    token_set("CONFIG_UEVENT_HELPER_PATH", "%s", "");
    if (token_get("CONFIG_HOTPLUG"))
    {
        /* in linux 2.6, we enable netlink and disable helper
         * while in 2.4, we do exactly the opposite */
        if (token_get("CONFIG_RG_OS_LINUX_26"))
        {
            token_set_y("CONFIG_RG_NETLINK_HOTPLUG");
        }

        if (token_get("CONFIG_RG_OS_LINUX_24"))
        {
            token_set_y("CONFIG_RG_HOTPLUG");
            token_set("CONFIG_UEVENT_HELPER_PATH", "/sbin/hotplug");
        }
    }
    /* Off-the-shelf uses only free providers. */
    if (token_get("CONFIG_RG_DDNS"))
    {
	token_set_y("CONFIG_RG_DDNS_ZONEEDIT");
	token_set_y("CONFIG_RG_DDNS_DTDNS");
	token_set_y("CONFIG_RG_DDNS_DYNDNS");
	token_set_y("CONFIG_RG_DDNS_NOIP");
	token_set_y("CONFIG_RG_DDNS_CHANGEIP");
	if (!token_get("CONFIG_RG_INTEGRAL"))
	{
	    token_set_y("CONFIG_RG_DDNS_TZO");
	    token_set_y("CONFIG_RG_DDNS_ODS");
	    token_set_y("CONFIG_RG_DDNS_EASYDNS");
	}
    }

    if (token_get("CONFIG_CRYPTO_DANUBE"))
    {
	token_set_y("CONFIG_CRYPTO_DEV_DANUBE");
	token_set_y("CONFIG_CRYPTO_DEV_DANUBE_DES");
	token_set_y("CONFIG_CRYPTO_DEV_DANUBE_AES");

	/* as soon as B45763 is resolved
	   token_set_y("CONFIG_CRYPTO_DEV_DANUBE_SHA1");
	   token_set_y("CONFIG_CRYPTO_DEV_DANUBE_MD5");
	   */

	token_set_y("CONFIG_CRYPTO_DEV_DANUBE_DMA");

	if (token_get("CONFIG_IPSEC"))
	    token_set_y("CONFIG_IPSEC_USE_DANUBE_CRYPTO");
    }

    if (token_get("CONFIG_CRYPTO_DEV_DEU"))
    {
	token_set_y("CONFIG_CRYPTO");
        token_set_y("CONFIG_CRYPTO_HASH");
        token_set_y("CONFIG_CRYPTO_ALGAPI");
        token_set_y("CONFIG_CRYPTO_BLKCIPHER");
	if (token_get("CONFIG_IPSEC"))
	    token_set_y("CONFIG_IPSEC_USE_LANTIQ_DEU_CRYPTO");

	/* sigh... The BSP requires the set of configs to be declared
	 * for EACH platform.
	 */
#define CRYPTO_IFX(plat) do {\
    	token_set_y("CONFIG_CRYPTO_DEV_DES" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_AES" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_ARC4" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_SHA1" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_MD5" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_SHA1_HMAC" plat);\
	token_set_y("CONFIG_CRYPTO_DEV_MD5_HMAC" plat); \
} while(0)

	CRYPTO_IFX("");
	if (token_get("CONFIG_CRYPTO_DEV_AR9"))
	    CRYPTO_IFX("_AR9");
	if (token_get("CONFIG_CRYPTO_DEV_VR9"))
	    CRYPTO_IFX("_VR9");
        
#undef CRYPTO_IFX
    }

    if (token_get("CONFIG_BCM_ARL_VLAN_LEARNING") &&
	token_get("CONFIG_BCM_ETHSW_MDIO_VALIDATION"))
    {
	conf_err("ERROR: CONFIG_BCM_ARL_VLAN_LEARNING and "
	    "CONFIG_BCM_ETHSW_MDIO_VALIDATION are never to be enabled "
	    "together\n");
    }

    /* The CONFIG_RG_DBG_ULIBC_MEM_POISON depends on
     * CONFIG_RG_DBG_ULIBC_MALLOC */
    if (token_get("CONFIG_RG_DBG_ULIBC_MEM_POISON"))
	token_set_y("CONFIG_RG_DBG_ULIBC_MALLOC");

    if (token_get("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH") ||
        token_get("CONFIG_RG_MAINFS_IN_UBI"))
    {
	token_set_y("CONFIG_RG_MAINFS_IN_FLASH");
    }

    if (token_get("CONFIG_RG_IGMP_PROXY"))
    {
	token_set_m("CONFIG_RG_IGMP_SNOOPING");
	token_set_y("CONFIG_IP_MROUTE");
    }

    if (token_get("CONFIG_FEROCEON_WATCHDOG"))
	token_set_y("CONFIG_WATCHDOG");
}    

/* Features that are always defined */
void openrg_features(void)
{
    token_set_y("CONFIG_EXPORT_BINARIES"); /* Make distribution export objects
					    * and binaries in addition to src
					    */
    if (token_get("CONFIG_INSURE"))
	token_set_y("CONFIG_GLIBC");

    if (token_get("CONFIG_VALGRIND"))
    {
	token_set_y("OPENRG_DEBUG");
    }
   
    if (!token_get("CONFIG_GLIBC") && !token_get("CONFIG_RG_NOT_UNIX"))
	token_set_y("CONFIG_ULIBC");

    if (token_get("CONFIG_ULIBC") && !token_get_str("ULIBC_DIR"))
	token_set("ULIBC_DIR", "pkg/ulibc");

    if (!token_get("CONFIG_RG_NOT_UNIX") && !token_get("CONFIG_RG_RGLOADER"))
	token_set_y("CONFIG_UNIX");

    /* Config freepages limits manually - check CONFIG_FREEPAGES_HIGH for 
     * values.
     */
    token_set_y("CONFIG_FREEPAGES_THRESHOLD");

    token_set_y("CONFIG_RG_FACTORY_SETTINGS");

    token_set_y("CONFIG_SYN_COOKIES");

    /* OpenRG char device */
    token_set("OPENRG_CHRDEV_NAME", "rg_chrdev");
    token_set("OPENRG_CHRDEV_MAJOR", "240");

    /* Default languages */
    if (!token_get_str("CONFIG_RG_LANGUAGES"))
    {
	/* Defines which languages to be compiled. The languages name should 
	 * be separated with spaces, ex: "DEF es ja"
	 * If left out, all languages in csv files will be compiled.
	 * Currently we use only 'DEF' and 'fr', to reduce image size */
	token_set("CONFIG_RG_LANGUAGES", "DEF fr");
    }

    /* Add here right to left languages */
    if (strstr(token_get_str("CONFIG_RG_LANGUAGES"), "he"))
	token_set_y("CONFIG_RG_RTL_LANGUAGE");

    if (is_evaluation)
	token_set_y("CONFIG_LICENSE_AGREEMENT"); 
    
    if (!token_get_str("RG_PROD_STR"))
	token_set("RG_PROD_STR", "OpenRG");
    if (!token_get_str("DEVICE_MANUFACTURER_STR"))
	token_set("DEVICE_MANUFACTURER_STR", "Jungo");

    token_set_default("CONFIG_RG_LOCAL_RESOLVE_BY_SO_MARK", "y");
}

option_t *option_token_get_nofail(option_t *array, char *token)
{
    int i;
    for (i = 0; array[i].token && strcmp(array[i].token, token); i++);
    return array[i].token ? array+i : NULL;
}

option_t *option_token_get(option_t *array, char *token)
{
    option_t *opt = option_token_get_nofail(array, token);
    if (!opt)
	conf_err("ERROR: Can't set %s: No such option\n", token);
    return opt;
}

void _token_set_y(char *file, int line, char *token)
{
    _token_set(file, line, SET_PRIO_TOKEN_SET, token, "y");
}

void _token_set_m(char *file, int line, char *token)
{
    _token_set(file, line, SET_PRIO_TOKEN_SET, token, "m");
}

char *token_getz(char *token)
{
    return token_get_str(token) ?: "";
}

char *token_get_str(char *token)
{
    option_t *opt = option_token_get(openrg_config_options, token);
    return opt->value;
}

int token_is_y(char *token)
{
    char *val;

    if (!(val = token_get_str(token)))
	return 0;
    return !strcmp(val, "y");
}

/* handle both decimal and hex numbers */
/* We allow "", "1234", "0x1234" */
int str_is_number_value(char *val)
{
    int i;
    if (!*val)
	return 1;
    if (val[0]=='0' && val[1]=='x')
    {
	for (i=2; val[i] && isxdigit(val[i]); i++);
	return i>2 && !val[i];
    }
    for (i=0; val[i] && isxdigit(val[i]); i++);
    return i>0 && !val[i];
}

/* handle both decimal and hex numbers */
int str_to_number(char *val)
{
    int i = 0;
    if (!*val)
	return 0;
    if (val[0]=='0' && val[1]=='x')
	sscanf(val, "%x", &i);
    else
	sscanf(val, "%d", &i);
    return i;
}

int token_get(char *token)
{
    char *val;
    option_t *o = option_token_get(openrg_config_options, token);
    
    /* sanity check for internal errors - accessing strings as integer */
    if (o->type & (OPT_STR | OPT_C_STR))
    {
	conf_err("ERROR: token %s is a string type (expected integer)\n",
	    token);
    }
    if (!(val = token_get_str(token)))
	return 0;
    /* is it a number? */
    if (o->type & OPT_NUMBER)
	return str_to_number(val);
    /* otherwise its a y/m/n */
    return !strcmp(val, "y") || !strcmp(val, "m");
}

