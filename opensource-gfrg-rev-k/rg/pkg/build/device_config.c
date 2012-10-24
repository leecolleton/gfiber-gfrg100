/****************************************************************************
 *
 * rg/pkg/build/device_config.c
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
#include <rg_conf_entries.h>
#include <util/conv.h>
#include <enums.h>
#undef _ENUMS_H_
#define ENUM_IMPLEMENTATION
#include <enums.h>

static void _dev_add_enabled(char *file, int line, char *name,
    dev_if_type_t type, logical_network_t net, int is_enabled);

/* XXX: Temporary fix after enum change. These configs should be removed and
 * dev_if's should be able to brought up in create config and information would
 * be retrived by IOCTLs. */
code2str_t dev_if_type_cfg_str[] = {
    { DEV_IF_79XX355_DSL, "DEV_IF_79XX355_DSL" },
    { DEV_IF_79XX355_ETH, "DEV_IF_79XX355_ETH" },
    { DEV_IF_PRISM2, "DEV_IF_PRISM2" },
    { DEV_IF_BCM43XX, "DEV_IF_BCM43XX" },
    { DEV_IF_BCM963XX_ETH, "DEV_IF_BCM963XX_ETH" },
    { DEV_IF_BCM963XX_ADSL, "DEV_IF_BCM963XX_ADSL" },
    { DEV_IF_BCM963XX_RNDIS, "DEV_IF_BCM963XX_RNDIS" },
    { DEV_IF_ATM_NULL, "DEV_IF_ATM_NULL" },
    { DEV_IF_BCM4710_ETH, "DEV_IF_BCM4710_ETH" },
    { DEV_IF_EEPRO100, "DEV_IF_EEPRO100" },
    { DEV_IF_EEPRO1000, "DEV_IF_EEPRO1000" },
    { DEV_IF_ETH1394, "DEV_IF_ETH1394" },
    { DEV_IF_ISL38XX, "DEV_IF_ISL38XX" },
    { DEV_IF_ISL_SOFTMAC, "DEV_IF_ISL_SOFTMAC" },
    { DEV_IF_NATSEMI, "DEV_IF_NATSEMI" },
    { DEV_IF_NE2000, "DEV_IF_NE2000" },
    { DEV_IF_RTL8139, "DEV_IF_RTL8139" },
    { DEV_IF_UML, "DEV_IF_UML" },
    { DEV_IF_UML_HW_SWITCH, "DEV_IF_UML_HW_SWITCH" },
    { DEV_IF_USB_RNDIS, "DEV_IF_USB_RNDIS" },
    { DEV_IF_VLAN, "DEV_IF_VLAN" },
    { DEV_IF_PPPOA, "DEV_IF_PPPOA" },
    { DEV_IF_PPPOE, "DEV_IF_PPPOE" },
    { DEV_IF_PPPOES_CONN, "DEV_IF_PPPOES_CONN" },
    { DEV_IF_PPPOH, "DEV_IF_PPPOH" },
    { DEV_IF_PPPOS_CONN, "DEV_IF_PPPOS_CONN" },
    { DEV_IF_ETHOA, "DEV_IF_ETHOA" },
    { DEV_IF_PPTPC, "DEV_IF_PPTPC" },
    { DEV_IF_PPTPS_CONN, "DEV_IF_PPTPS_CONN" },
    { DEV_IF_L2TPS_CONN, "DEV_IF_L2TPS_CONN" },
    { DEV_IF_IPSEC_DEV, "DEV_IF_IPSEC_DEV" },
    { DEV_IF_IPSEC_CONN, "DEV_IF_IPSEC_CONN" },
    { DEV_IF_IPSEC_TEMPL, "DEV_IF_IPSEC_TEMPL" },
    { DEV_IF_IPSEC_TEMPL_TRANSIENT, "DEV_IF_IPSEC_TEMPL_TRANSIENT" },
    { DEV_IF_IPSEC_TEMPL_CONN, "DEV_IF_IPSEC_TEMPL_CONN" },
    { DEV_IF_BRIDGE, "DEV_IF_BRIDGE" },
    { DEV_IF_CHWAN_MASTER, "DEV_IF_CHWAN_MASTER" },
    { DEV_IF_DOCSIS, "DEV_IF_DOCSIS" },
    { DEV_IF_ELCP, "DEV_IF_ELCP" },
    { DEV_IF_CAS, "DEV_IF_CAS" },
    { DEV_IF_CLIP, "DEV_IF_CLIP" },
    { DEV_IF_L2TPC, "DEV_IF_L2TPC" },
    { DEV_IF_USER_VLAN, "DEV_IF_USER_VLAN" },
    { DEV_IF_IPV6_OVER_IPV4_TUN, "DEV_IF_IPV6_OVER_IPV4_TUN" },
    { DEV_IF_IPOA, "DEV_IF_IPOA" },
    { DEV_IF_CX821XX_ETH, "DEV_IF_CX821XX_ETH" },
    { DEV_IF_SL2312_ETH, "DEV_IF_SL2312_ETH" },
    { DEV_IF_CX8620X_SWITCH, "DEV_IF_CX8620X_SWITCH" },
    { DEV_IF_WDS_CONN, "DEV_IF_WDS_CONN" },
    { DEV_IF_RT2560, "DEV_IF_RT2560" },
    { DEV_IF_RT2561, "DEV_IF_RT2561" },
    { DEV_IF_RT2860, "DEV_IF_RT2860" },
    { DEV_IF_RT2880, "DEV_IF_RT2880" },
    { DEV_IF_AGN100, "DEV_IF_AGN100" },
    { DEV_IF_WAVE300, "DEV_IF_WAVE300" },
    { DEV_IF_WAVE300_VAP, "DEV_IF_WAVE300_VAP" },
    { DEV_IF_UML_WLAN, "DEV_IF_UML_WLAN" },
    { DEV_IF_MPC82XX_ETH, "DEV_IF_MPC82XX_ETH" },
    { DEV_IF_AD6834_ETH, "DEV_IF_AD6834_ETH" },
    { DEV_IF_AD68XX_ADSL, "DEV_IF_AD68XX_ADSL" },
    { DEV_IF_KS8695_ETH, "DEV_IF_KS8695_ETH" },
    { DEV_IF_BCM5325A_HW_SWITCH, "DEV_IF_BCM5325A_HW_SWITCH" },
    { DEV_IF_BCM5325E_HW_SWITCH, "DEV_IF_BCM5325E_HW_SWITCH" },
    { DEV_IF_BRCM91125E_ETH, "DEV_IF_BRCM91125E_ETH" },
    { DEV_IF_RTL8305SC_HW_SWITCH, "DEV_IF_RTL8305SC_HW_SWITCH" },
    { DEV_IF_RTL8305SB_HW_SWITCH, "DEV_IF_RTL8305SB_HW_SWITCH" },
    { DEV_IF_COMCERTO_ETH, "DEV_IF_COMCERTO_ETH" },
    { DEV_IF_COMCERTO_ETH_VED, "DEV_IF_COMCERTO_ETH_VED" },
    { DEV_IF_SOLOS_WLAN, "DEV_IF_SOLOS_WLAN" },
    { DEV_IF_SOLOS_LAN, "DEV_IF_SOLOS_LAN" },
    { DEV_IF_SOLOS_DMZ, "DEV_IF_SOLOS_DMZ" },
    { DEV_IF_SOLOS_DSL, "DEV_IF_SOLOS_DSL" },
    { DEV_IF_WIFI, "DEV_IF_WIFI" },
    { DEV_IF_AR5212_VAP, "DEV_IF_AR5212_VAP" },
    { DEV_IF_AR5416_VAP, "DEV_IF_AR5416_VAP" },
    { DEV_IF_AR5212_VAP_SLAVE, "DEV_IF_AR5212_VAP_SLAVE" },
    { DEV_IF_AR5416_VAP_SLAVE, "DEV_IF_AR5416_VAP_SLAVE" },
    { DEV_IF_IKANOS_VDSL, "DEV_IF_IKANOS_VDSL" },
    { DEV_IF_IKANOS_ADSL, "DEV_IF_IKANOS_ADSL" },
    { DEV_IF_DANUBE_ETH, "DEV_IF_DANUBE_ETH" },
    { DEV_IF_DANUBE_ATM, "DEV_IF_DANUBE_ATM" },
    { DEV_IF_ADM6996_HW_SWITCH, "DEV_IF_ADM6996_HW_SWITCH" },
    { DEV_IF_PSB6973_HW_SWITCH, "DEV_IF_PSB6973_HW_SWITCH" },
    { DEV_IF_LANTIQ_VR9_HW_SWITCH, "DEV_IF_LANTIQ_VR9_HW_SWITCH" },
    { DEV_IF_VINAX_VDSL, "DEV_IF_VINAX_VDSL" },
    { DEV_IF_KS8995MA_ETH, "DEV_IF_KS8995MA_ETH" },
    { DEV_IF_P400_DSL, "DEV_IF_P400_DSL" },
    { DEV_IF_CN3XXX_ETH, "DEV_IF_CN3XXX_ETH" },
    { DEV_IF_RT2561_VAP, "DEV_IF_RT2561_VAP" },
    { DEV_IF_RT2860_VAP, "DEV_IF_RT2860_VAP" },    
    { DEV_IF_RT2880_VAP, "DEV_IF_RT2880_VAP" },    
    { DEV_IF_BCM43XX_VAP, "DEV_IF_BCM43XX_VAP" },
    { DEV_IF_MYDEV, "DEV_IF_MYDEV" },
    { DEV_IF_UML_WLAN_VAP, "DEV_IF_UML_WLAN_VAP" },
    { DEV_IF_AVALANCHE_CPGMAC_F_ETH, "DEV_IF_AVALANCHE_CPGMAC_F_ETH" },
    { DEV_IF_TI550_ETH_DOCSIS3, "DEV_IF_TI550_ETH_DOCSIS3" },
    { DEV_IF_TI550_CNI_DOCSIS3, "DEV_IF_TI550_CNI_DOCSIS3" },
    { DEV_IF_AR8316_HW_SWITCH, "DEV_IF_AR8316_HW_SWITCH" },
    { DEV_IF_HW_SWITCH_PORT, "DEV_IF_HW_SWITCH_PORT" },
    { DEV_IF_TI550_ETH, "DEV_IF_TI550_ETH" },
    { DEV_IF_LANTIQ_AR9_ETH, "DEV_IF_LANTIQ_AR9_ETH" },
    { DEV_IF_LANTIQ_VR9_ETH, "DEV_IF_LANTIQ_VR9_ETH" },
    { DEV_IF_LANTIQ_XWAY_DSL, "DEV_IF_LANTIQ_XWAY_DSL" },
    { DEV_IF_LANTIQ_XWAY_ATM, "DEV_IF_LANTIQ_XWAY_ATM" },
    { DEV_IF_LANTIQ_XWAY_PTM, "DEV_IF_LANTIQ_XWAY_PTM" },
    { DEV_IF_LANTIQ_ETH, "DEV_IF_LANTIQ_ETH" },
    { DEV_IF_VIRTUAL_WAN, "DEV_IF_VIRTUAL_WAN" },
    { DEV_IF_3G_USB_SERIAL, "DEV_IF_3G_USB_SERIAL" },
    { DEV_IF_BCM9636X_ETH, "DEV_IF_BCM9636X_ETH" },
    { DEV_IF_BCM9636X_ADSL, "DEV_IF_BCM9636X_ADSL" },
    { DEV_IF_C1000_ETH, "DEV_IF_C1000_ETH" },
    { DEV_IF_BCM53115M_ETH, "DEV_IF_BCM53115M_ETH" },
    { DEV_IF_BCM338X_VETH, "DEV_IF_BCM338X_VETH" },
    { DEV_IF_BCM338X_VETHSW, "DEV_IF_BCM338X_VETHSW" },
    { DEV_IF_BCM338X_VIPSTK, "DEV_IF_BCM338X_VIPSTK" },
    { DEV_IF_BCM338X_VWLAN_PHY, "DEV_IF_BCM338X_VWLAN_PHY" },
    { DEV_IF_BCM338X_VWLAN, "DEV_IF_BCM338X_VWLAN" },
    { DEV_IF_MV88F65XX_ETH, "DEV_IF_MV88F65XX_ETH" },
    { DEV_IF_FEROCEON_HW_SWITCH, "DEV_IF_FEROCEON_HW_SWITCH" },
    { DEV_IF_MV8764_VAP, "DEV_IF_MV8764_VAP" },
    { DEV_IF_MV88F65XX_PON, "DEV_IF_MV88F65XX_PON" },
    { DEV_IF_QHS71X_HOST, "DEV_IF_QHS71X_HOST" },
    { -1, NULL }
};

static FILE *dev;

void _token_set_dev_type(char *file, int line, dev_if_type_t type)
{
    char config_dev[128];
    
    sprintf(config_dev, "CONFIG_RG_%s", code2str(dev_if_type_cfg_str, type));
    _token_set(file, line, SET_PRIO_TOKEN_SET, config_dev, "y");
}

/* Define a network interface for a hardware.
 * name - the name of the interface, as it appears in rg_conf and in
 *   ifconfig.
 * type - the type of the device. dev_if_type_t is defined in
 *   pkg/include/enums.h.
 * net - define the network this device will represent - wan/lan/dmz.
 */
void _dev_add(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net)
{
    _dev_add_enabled(file, line, name, type, net, 1);
}

static void _dev_add_enabled(char *file, int line, char *name,
    dev_if_type_t type, logical_network_t net, int is_enabled)
{
    int is_sync;

    _token_set_dev_type(file, line, type);

    is_sync = net == DEV_IF_NET_INT; /* internal are with static IP - sync */
    if (type == DEV_IF_ATM_NULL)
	is_sync = 1;

    fprintf(dev, "dev/%s/%s %s\n", name, Stype,
	code2str_ex(dev_if_type_t_str, type));
    fprintf(dev, "dev/%s/%s %d\n", name, Slogical_network, net);
    fprintf(dev, "dev/%s/%s %d\n", name, Sis_sync, is_sync);
    fprintf(dev, "dev/%s/%s %d\n", name, Senabled, is_enabled);
}

void _dev_add_disabled(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net)
{
    _dev_add_enabled(file, line, name, type, net, 0);
}

void _dev_set_static_ip(char *file, int line, char *name,
    char *ip, char *netmask)
{
    fprintf(dev, "dev/%s/%s %s\n", name, Sstatic "/" Sip, ip);
    fprintf(dev, "dev/%s/%s %s\n", name, Sstatic "/" Snetmask, netmask);
}

void _dev_set_id(char *file, int line, char *name, int id)
{
    fprintf(dev, "dev/%s/%s %d\n", name, Sid, id);
}

void _dsl_atm_dev_add(char *file, int line, char *name, dev_if_type_t type, 
    char *dsl_dev)
{
    _dev_add(file, line, name, type, DEV_IF_NET_EXT);
    token_set("CONFIG_ATM_DEV_NAME", name);
    if (dsl_dev)
	dev_set_dependency(name, dsl_dev);
}

void _dsl_ptm_dev_add(char *file, int line, char *name, dev_if_type_t type,
    char *dsl_dev)
{
    _dev_add(file, line, name, type, DEV_IF_NET_EXT);
    token_set("CONFIG_PTM_DEV_NAME", name);
    if (dsl_dev)
	dev_set_dependency(name, dsl_dev);
}

void _dev_add_desc(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net, char *d)
{
    _dev_add(file, line, name, type, net);
    fprintf(dev, "dev/%s/%s %s\n", name, Sdescription, d);
}

void _switch_dev_add(char *file, int line, char *name, char *depend_on,
    dev_if_type_t type, logical_network_t net, int cpu_port)
{
    _dev_add(file, line, name, type, net);
    if (depend_on)
    {
	dev_set_dependency(name, depend_on);
	fprintf(dev, "dev/%s/%s %d\n", depend_on, Sis_invisible, 1);
    }

    fprintf(dev, "dev/%s/%s %d\n", name, Scpu_port, cpu_port);
}

void _switch_virtual_port_dev_add(char *file, int line, char *name, char *sw,
    int port, logical_network_t net)
{
    _dev_add(file, line, name, DEV_IF_HW_SWITCH_PORT, net);
    dev_set_dependency(name, sw);
    fprintf(dev, "dev/%s/%s/%s/%d/%s %s\n", sw, Shw_switch, Sports, port,
	Svirtual_dev, name);
}

void _gsm_dev_add(char *file, int line, char *name, dev_if_type_t type)
{
    _dev_add(file, line, name, type, DEV_IF_NET_EXT);
    token_set("CONFIG_GSM_MODEM_DEV_NAME", name);
}

void dev_add_uml_hw_switch(char *name, logical_network_t net, int cpu_port, ...)
{
    int port = 0;
    va_list ap;
    char *enslaved;

    switch_dev_add(name, NULL, DEV_IF_UML_HW_SWITCH, net, cpu_port);

    va_start(ap, cpu_port);

    while ((enslaved = va_arg(ap, char *)))
    {
	fprintf(dev, "dev/%s/%s/%s/%d/dev %s\n", name, Shw_switch, Sports,
	    port++, enslaved);
    }

    va_end(ap);
}

/* Make 'dev_name' depend on 'depend_on' */
void dev_set_dependency(char *dev_name, char *depend_on)
{
    fprintf(dev, "dev/%s/%s %s\n", dev_name,  Sdepend_on_name, depend_on);
}

void dev_set_logical_dependency(char *dev_name, char *depend_on)
{
    fprintf(dev, "dev/%s/%s %s\n", dev_name, Slogical_depend_on, depend_on);
}

void dev_can_be_missing(char *dev_name)
{
    fprintf(dev, "dev/%s/%s 1\n", dev_name, Scan_be_missing);
}

void dev_add_to_bridge_if_opt(char *name, char *enslaved, char *opt_verify)
{
    if (!opt_verify)
	return;

    if (!token_get(opt_verify))
	return;

    dev_add_to_bridge(name, enslaved);
}

void dev_add_to_bridge(char *name, char *enslaved)
{
    fprintf(dev, "dev/%s/%s/%s/%s %d\n", name, Senslaved, enslaved, Sstp, 0);
}

void dev_wl_net_type(char *dev_name, wl_net_type_t net_type)
{
    fprintf(dev, "%s/%s/%s %s\n", Sdev, dev_name, Swl_net_type,
	code2str_ex(wl_net_type_t_str, net_type));
}

void dev_set_web_auth(char *name)
{
    fprintf(dev, "%s/%s/%s %d\n", Sdev, name, Sweb_auth, 1);
}

void dev_set_ap_vlan_id(char *name, int vid)
{
    fprintf(dev, "%s/%s/%s %d\n", Sdev, name, Sap_vlan_id, vid);
}

/** Create a network bridge.
 * name - The bridge interface name. Usually "br0".
 * net - Define the network this device will represent - wan/lan/dmz.
 * ... - A list of char * enslaved devices, ending with a NULL.
 * If the device list is empty, the enslaved devices are set automatically to
 * all ethernet devices with the same logical network as bridge. 
 */
void dev_add_bridge(char *name, logical_network_t net, ...)
{
    va_list ap;
    char *enslaved;

    dev_add(name, DEV_IF_BRIDGE, net);

    va_start(ap, net);
    /* STP field is updated in gen_rg_def and is defaulted to 0 */
    while ((enslaved = va_arg(ap, char *)))
	dev_add_to_bridge(name, enslaved);

    va_end(ap);

    if (net == DEV_IF_NET_INT)
	token_set_y("CONFIG_DEF_BRIDGE_LANS");
}

void dev_set_route_group(char *name, dev_route_group_t grp)
{
    fprintf(dev, "%s/%s/%s %s\n", Sdev, name, Sroute_group_id,
	code2str_ex(dev_route_group_t_str, grp));
}

void dev_add_virtual_wans(char *underlying, char *main_vwan, ...)
{
    va_list ap;
    char *vwan;

    va_start(ap, main_vwan);

    for (vwan = main_vwan; vwan; vwan = va_arg(ap, char *))
    {
	dev_add(vwan, DEV_IF_VIRTUAL_WAN, DEV_IF_NET_EXT);
	dev_set_dependency(vwan, underlying);
    }

    va_end(ap);

    dev_set_logical_dependency(main_vwan, underlying);
}

void dev_open_conf_file(char *filename)
{
    if (!(dev = fopen(filename, "w")))
	conf_err("Can't open devices output file:%s\n", filename);
}

void dev_close_conf_file(void)
{
    fclose(dev);
}
