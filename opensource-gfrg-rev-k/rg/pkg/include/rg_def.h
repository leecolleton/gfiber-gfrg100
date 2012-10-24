/****************************************************************************
 *
 * rg/pkg/include/rg_def.h
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

#ifndef _RG_DEF_H_
#define _RG_DEF_H_

#if USE_BASIC_HOST_CONFIG
#include <host_config.h>
#else
#include <rg_config.h>
#endif

#define OS_INCLUDE_STRING
#include <os_includes.h>

#include <util/util.h>
#include <rg_types.h>

#include <cc_config.h>

#define MACRO_TO_STR2(A) #A
#define MACRO_TO_STR(A) MACRO_TO_STR2(A)

#define MAX_ETH_RAW_FRAME_SIZE 4096

/* OpenRG common URLs and emails */
#define JUNGO_DOMAIN "jungo.com"
#define JUNGO_SITE "www." JUNGO_DOMAIN
#define JUNGO_URL "http://" JUNGO_SITE
#define JUNGO_URL_SSL "https://" JUNGO_SITE
#define JUNGO_HELP_URL JUNGO_URL "/openrg/help/openrg_%d.%d.html"
#define JUNGO_OPEN_SOURCE_URL JUNGO_URL "/openrg/opensource_ack.html"
#define RG_SUPP_EMAIL "rg_support@" JUNGO_DOMAIN
#define RG_SALES_EMAIL "sales_rg@" JUNGO_DOMAIN
#define RG_ANON_USERNAME "anonymous"
#define RG_ANON_PWD "OpenRG@" JUNGO_DOMAIN
#define RG_PHONE_NUMBER "1-800-GO-JUNGO"

#define SSID_NAME_SUFFIX "'s"
#define SSID_DEFAULT_SEPARATOR " - "
#define HOME_NETWORK_SSID_SUFFIX RG_PROD_STR " Home Network"
#define WPA_NETWORK_SSID_SUFFIX RG_PROD_STR " WPA Security"
#define WEP_NETWORK_SSID_FMT_PREF RG_PROD_STR " WEP Security"
#define HOME_NETWORK_SSID_FMT "%s" SSID_NAME_SUFFIX " " HOME_NETWORK_SSID_SUFFIX
#define HOME_NETWORK_SSID_UNIQUE_FMT HOME_NETWORK_SSID_SUFFIX " (%s)"
#define WPA_NETWORK_SSID_FMT WPA_NETWORK_SSID_SUFFIX SSID_DEFAULT_SEPARATOR "%s"
#define WEP_NETWORK_SSID_FMT WEP_NETWORK_SSID_FMT_PREF SSID_DEFAULT_SEPARATOR \
    "%s"
#define INSTALL_NETWORK_SSID_FMT RG_PROD_STR " " RG_SSID_STR " (%s)"
#define HELP_LINE_SSID_FMT "Help Line (%s)"
#define HELP_LINE_SSID "Help Line"

#define MAX_LINE_SIZE 1024
#define MAX_VAR_NAME 80
#define MAX_CMD_LEN 300
#define MAX_PATH_LEN 100
#define MAX_QUERY_LEN 1024
#define MAX_IP_LEN 20
#define MAX_MAC_LEN 18
#define DOTTED_IP_LEN 16 	/* length of: xxx.xxx.xxx.xxx\0 */
#define MAX_PORT_LEN 6		/* length of: xxxxx\0 */
#define LONG_NUM_LEN 16
#define MAX_PROC_CMD_FILENAME 32
#define MAX_DEV_FILENAME 32
#define MAX_DEV_NAME 16
#ifndef MAX_ERROR_MSG_LEN 
#define MAX_ERROR_MSG_LEN MAX_LINE_SIZE
#endif
#define MAX_8021X_KEY_CHARS 27 /* 104 bit WEP key */
#define MAX_IP_PREFIX_LEN 32
#define MAX_IPV6_PREFIX_LEN 128

/* The following are not including '\0' */
#define MAX_WORKGROUP_NAME_LEN 15
/* FQDN accodring to RFC */
#define MAX_DOMAIN_NAME_LEN 255
#define MAX_HOST_NAME_LEN 63
#define MAX_SSID_LEN 32

#define MAX_PHONE_NUMBER_LEN 32
#define COUNTRY_CODE_LEN 3
#define MAX_USERNAME_LEN 64
#define MAX_POLICY_NAME_LEN MAX_USERNAME_LEN
/* username "@" domain */
#define MAX_EMAIL_LEN (MAX_USERNAME_LEN+1+MAX_DOMAIN_NAME_LEN)
#define MAX_PASSWORD_LEN 64
#define MAX_ENC_KEY_LEN 128
#define MAX_FULLNAME_LEN 128
/* Windows allows use of upto 256 characters for PPTP client password */
#define MAX_SECRET_LEN 256

#define MAX_URL_LEN  \
    ( \
    sizeof("http://") - 1 + \
    MAX_USERNAME_LEN + \
    sizeof(":") - 1 + \
    MAX_PASSWORD_LEN + \
    sizeof("@") - 1 + \
    MAX_HOST_NAME_LEN + \
    sizeof(":65535/") - 1 + \
    MAX_PATH_LEN + \
    MAX_QUERY_LEN \
    )
#define URL_MAX_DISPLAY_LEN 30
    
#define MAX_PPP_USERNAME_LEN 100
#define MAX_PPP_PASSWORD_LEN 100

#define MIN_MGT_PASSWORD_LEN 4

#define MAX_DHCPS_LEASES 256

#define MAX_ENCRYPTION_KEY_LEN 20
#define MAX_WIRELESS_AP_KEY_MAX 26

#define MAX_CERT_SUBJECT_LEN 255
#define MAX_X509_NAME_LEN 64
#define X509_DEFAULT_NAME "new-certificate"
#define X509_OPENRG_NAME_PREFIX "ORname_"

/* limitation defined in rp-l2tp */
#define L2TP_MAX_SECRET_LEN 96

#define MAX_DESCR_LEN 64

#include <rg_os.h>

#define PPTP_CLIENT_SUFFIX	"pptp_cli"
#define LOCAL_HOST		"localhost"

#define SYSTEM_LOG		0x1 
#define SYSTEM_DAEMON		0x2
#define SYSTEM_CLOSE_FDS	0x4
/* Similar to SYSTEM_DAEMON, but the spawned process inherits its parent's
 * stdout and stderr file descriptors, making its output visible. */
#define SYSTEM_BACKGROUND	0x8
/* Redirect the spawned process input/output to /dev/null */
#define SYSTEM_NO_STDS		0x10

#define MAX_BAD_REBOOT 3

#define PPTPS_MAX_CONNS 10
#define L2TPS_MAX_CONNS 10

/* echo needed to keep the fw state alive, otherwise it is deleted after two
 * minutes idle time and the connection will be blocked.
 * The default values below will make ppp discover broken link in 30
 * seconds (5*6)
 */
#ifndef CONFIG_PPP_ECHO_INTERVAL
#define PPP_ECHO_INTERVAL 6
#else
#define PPP_ECHO_INTERVAL (CONFIG_PPP_ECHO_INTERVAL)
#endif

#ifndef CONFIG_PPP_ECHO_FAILURE
#define PPP_ECHO_FAILURE 5
#else 
#define PPP_ECHO_FAILURE (CONFIG_PPP_ECHO_FAILURE)
#endif

#ifndef CONFIG_PPPOE_RETRANSMIT
#define PPPOE_RETRANSMIT 10
#else
#define PPPOE_RETRANSMIT (CONFIG_PPPOE_RETRANSMIT)
#endif

#ifndef CONFIG_PPPOE_MAX_RETRANSMIT_TIMEOUT
#define PPPOE_MAX_RETRANSMIT_TIMEOUT ULONG_MAX
#else
#define PPPOE_MAX_RETRANSMIT_TIMEOUT (CONFIG_PPPOE_MAX_RETRANSMIT_TIMEOUT)
#endif

#define M2_IF_STATUS_UP 1
#define M2_IF_STATUS_DOWN 2

#define ITF_ANY_INTERFACE ""
#define ITF_LOOPBACK "lo"

#ifdef CONFIG_RG_BLUETOOTH_NAME
    #define BLUETOOTH_DEFAULT_NAME CONFIG_RG_BLUETOOTH_NAME
#else
    #define BLUETOOTH_DEFAULT_NAME "openrg"
#endif

#define FW_DEFAULT_POLICY "default"
#define FW_DEFAULT_POLICY_DESC "Default Policy"
#define FW_CH_DEFAULT_POLICY "ch_default"
#define FW_CH_DEFAULT_POLICY_DESC "CableHome Default Policy"
#define FW_CH_DISABLED_POLICY "ch_disabled"
#define FW_CH_DISABLED_POLICY_DESC "CableHome disabled-filtering policy"

#define INITIAL_RULES_DESCR "Initial Rules"
#define FINAL_RULES_DESCR "Final Rules"
#define ACC_CTRL_BLOCK_RULES_DESCR "Access Control - Block"
#define ACC_CTRL_ALLOW_RULES_DESCR "Access Control - Allow"
#define STATIC_HOST_CONF_DESCR "Static Host Configuration Rules"
#define INTERCEPTION_NAT_DESCR "Interception Address Translations"

#define CH_ALWAYS_DESCR "Never-disabled rules"
#define CH_CONF_DESCR "Configured ruleset"
#define CH_FACTORY_DESCR "Factory default ruleset"
#define CH_DEFAULT_DESCR "General behavior rules"

/* Rule group definitions
 * Important Note:
 * ipfilter 3.x doesn't support creating more then one head for the same
 * group. 
 * when a group number is specified, ipfw will add 1 to the number if the
 * direction of the rule is outbound. 
 * Bidir rules will be added once to the defined group number, and again to the
 * incremented group number.
 */
/* Security rules: IP Spoofing, IP options, IP fragments. */
#define GRP_SECURITY_IN_IPV4 	"security_in_ipv4" 
#define GRP_SECURITY_IN_IPV6 	"security_in_ipv6" 
#define GRP_SECURITY_OUT_IPV4	"security_out_ipv4"
#define GRP_SECURITY_OUT_IPV6	"security_out_ipv6"
/* OpenRG Support: RIP, IGMP, DHCP, PPTP, IPSec etc... */
#define GRP_OPENRG_IN_IPV4	"openrg_in_ipv4"
#define GRP_OPENRG_IN_IPV6	"openrg_in_ipv6"
#define GRP_OPENRG_OUT_IPV4	"openrg_out_ipv4"
#define GRP_OPENRG_OUT_IPV6	"openrg_out_ipv6"
#define GRP_RG_TASKS_IN		"rg_tasks_in"	
#define GRP_RG_TASKS_OUT	"rg_tasks_out"
#define GRP_FASTPATH_FLAGS	"fastpath_flags"
/* PacketCable*/
#define GRP_PKTCBL		"pktcbl"
#define GRP_PKTCBL_OUT		"pktcbl_out"
#define GRP_STATIC_RNAT      	"static_rnat"
/* User-defined rules (grouped by svc_rule_type_t) */
#define GRP_SVC_RMT_MAN_WAN_TRUSTED "svc_rmt_man_wan_trusted"
#define GRP_SVC_RMT_MAN_WAN	"svc_rmt_man_wan"
#define GRP_SVC_RMT_MAN_LAN	"svc_rmt_man_lan"
#define GRP_SVC_LOCAL_MAN	"svc_local_man"
#define GRP_SVC_LOCAL_SRV	"svc_local_srv"
#define GRP_DMZ			"dmz"
#define GRP_MAC_FILT		"mac_filt"
#define GRP_SVC_PARENT_CTRL	"svc_parent_ctrl"
#define GRP_SVC_PARENT_CTRL_RULE "svc_parent_ctrl_rule"
#define GRP_SVC_ADVANCED	"svc_advanced"
#define GRP_SVC_ADVANCED_OUT	"svc_advanced_out"
#define GRP_IPSEC		"ipsec"	
#define GRP_INITIAL_INBOUND	"initial_inbound"
#define GRP_INITIAL_OUTBOUND    "initial_outbound"
#define GRP_SVC_DMZ_HOST        "svc_dmz_host"
#define GRP_FINAL_INBOUND	"final_inbound"
#define GRP_FINAL_OUTBOUND    	"final_outbound"
#define GRP_STATIC_HOST_CONF_INBOUND "static_host_conf_inbound"
#define GRP_INTERCEPTION_NAT_IN "interception_nat_in"
#define GRP_PINHOLES 		Spinholes

/* CableHome chains */
#define GRP_CH_IN_CONF		"ch_in_conf"
#define GRP_CH_IN_FACTORY	"ch_in_factory"
#define GRP_CH_IN_ALWAYS	"ch_in_always"
#define GRP_CH_IN_DEFAULT	"ch_in_default"
#define GRP_CH_OUT_CONF		"ch_out_conf"
#define GRP_CH_OUT_FACTORY	"ch_out_factory"
#define GRP_CH_OUT_ALWAYS	"ch_out_always"
#define GRP_CH_OUT_DEFAULT	"ch_out_default"

#define GRP_TRANS_PXY           "trans_pxy"
/* HTTP proxy group */
#define GRP_HTTP_PXY		"http_pxy"
#define GRP_PURESIGHT_PXY	"puresight_pxy"

#define GRP_PORT_TRIGGERING	"port_triggering"
#define GRP_ALG_IN		"alg_in"
#define GRP_ALG_OUT		"alg_out"
#define NAT_CHAIN		"nat_chain"
#define CONN_GROUP_CHAIN	"conn_group_chain"

/* access control */
#define GRP_ACCESS_CTRL_BLOCK	"access_ctrl_block"
#define GRP_ACCESS_CTRL_ALLOW	"access_ctrl_allow"

/* hybrid model chain */
#define BRIDGE_FILTER_CHAIN 	"bridge_filter_chain"

/* QoS chains*/
#define QOS_CHAIN_OUT "qos_chain_out"
#define QOS_CHAIN_IN  "qos_chain_in"
#define QOS_CHAIN_PREFIX_CLASSLESS "qos_classless"
#define QOS_CHAIN_PREFIX_MAIN "qos_main"
#define QOS_CHAIN_APP "qos_chain_app"
#define QOS_CHAIN_FLOW "qos_chain_flow"
#define	QOS_CHAIN_DEFAULTS_IN_ALL "qos_defaults_in_all"
#define	QOS_CHAIN_CONN_UTIL_HOST_TX "qos_conn_util_host_tx"
#define	QOS_CHAIN_CONN_UTIL_APP_TX "qos_conn_util_app_tx"
#define	QOS_CHAIN_CONN_UTIL_APP_HOST_TX "qos_conn_util_app_host_tx"
#define	QOS_CHAIN_CONN_UTIL_HOST_RX "qos_conn_util_host_rx"
#define	QOS_CHAIN_CONN_UTIL_APP_RX "qos_conn_util_app_rx"
#define	QOS_CHAIN_CONN_UTIL_APP_HOST_RX "qos_conn_util_app_host_rx"

/* DNS interception chain */
#define DNS_INTERCEPTION_CHAIN "dns_intercept_chain"

/* Internet Connection Utilization classes*/
#define QOS_CONN_UTIL_CLASS_HIGH "conn_util_high"
#define QOS_CONN_UTIL_CLASS_MEDIUM "conn_util_medium"
#define QOS_CONN_UTIL_CLASS_LOW "conn_util_low"
#define QOS_CONN_UTIL_CLASS_LOW_INGRESS "conn_util_low_ingress"

/* Management Colors */
#define CLR_WORKING 0x008000
#define CLR_FAILURE 0xFF0000
#define CLR_DEFAULT 0x000000
#define CLR_INACTIVE 0x808080
#define CLR_PROGRESS 0xFF8000
#define CLR_WARNING 0xFF8000

/* upload/download rg_conf SSI defines */
#define SAVE_RG_CONF_CGI "save_rg_conf.cgi"
#define REPLACE_RG_CONF_CGI "replace_rg_conf.cgi"
#define NEW_RG_CONF "new_rg_conf"

/* HTTP variable names */
#define HTTP_VAR_ORG_URL "org_url"
#define HTTP_VAR_HOST_MAC "host_mac"
#define HTTP_VAR_OPENRG_URL "openrg_url"
#define HTTP_VAR_HTTP_INTERCEPT_PRIORITY "http_intercept_priority"
#define HTTP_VAR_INTERCEPT_ID "intercept_id"
#define HTTP_VAR_FIND_NEXT_INTERCEPTION "find_next_interception"
#define HTTP_VAR_NO_DNS "no_dns"

/* Disk management definitions */
#define DISK_MAX_DEV_NAME_LEN	 (sizeof("_dev_sdz99")) - 1
#define DISK_MOUNT_POINT_BASE	 "/mnt/fs"
#define DISK_MAX_MOUNT_POINT_LEN \
    (sizeof(DISK_MOUNT_POINT_BASE) + DISK_MAX_DEV_NAME_LEN)

/* UPnP */
#define UPNP_MAX_RULES		256
#define UPNP_CHECK_INTERVAL	5

/* DNS */
#define INTERCEPTION_IP 0x01010101
#define INTERCEPTION_MASK 0xfffffffc
#define DNS_ALIASING_POOL_START_IP "1.1.1.2"
#define DNS_ALIASING_POOL_END_IP "1.1.1.255"

/* Interception */
#define INTERCEPTION_PORT_BASE 8890
#define INTERCEPTION_PRIORITY_WEB_AUTH 10
#define INTERCEPTION_PRIORITY_NATION_ZONE 20
#define INTERCEPTION_PRIORITY_UNCONFIGURED_NETWORK 30
#define INTERCEPTION_PRIORITY_WAN_DOWN_NO_ISP 40
#define INTERCEPTION_PRIORITY_ZC_IP 50
#define INTERCEPTION_PRIORITY_VOIP 60
#define INTERCEPTION_PRIORITY_CABLE 70

/* ZC IP interception */
#define HTTP_VAR_BAD_IP "bad_ip"

/* Default HTTP directory */
#ifdef CONFIG_RG_EFSS
#define HTTP_DEFAULT_DIRECTORY "."
#elif defined(CONFIG_RG_JNET_SERVER)
#define HTTP_DEFAULT_DIRECTORY "/usr/local/jnet"
#else
#define HTTP_DEFAULT_DIRECTORY "/home/httpd/html"
#endif

#define RG_HTTP_QUERY_VAR "u"
#define WBM_HTTP_QUERY_VAR "wbm"
#define NET_HTTP_QUERY_VAR "net"
#define SCR_HTTP_QUERY_VAR "scr"

/* ISP specific stuff */
#define ISP_NAME "Jungo"
#define ISP_PHONE "1800-800-800"
#define ISP_PHONE_NORMAL_HOURS "1800-800-300"
#define ISP_BILLING_URL "http://www.jungo.com"
/* Link to guide user to set up new email */
#define ISP_EMAIL_BROWSING_URL "http://www.jungo.com"

#endif
