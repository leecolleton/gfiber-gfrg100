/* liCENSE_CODE JGPL */

#include <string.h>
#include "config_opt.h"
#include "create_config.h"
#include <sys/stat.h>
#include <unistd.h>

static int stat_lic_file(char *path)
{
    struct stat s;
    int ret = stat(path, &s);

    printf("Searching for license file in %s: %sfound\n", path,
	ret ? "not " : "");
    return ret;
}

static void set_jnet_server_configs(void)
{
    token_set_y("CONFIG_JRMS_DEBUG");
    token_set_y("CONFIG_JRMS_LDAP");    
    token_set_y("CONFIG_RG_HTTPS");
    token_set_y("CONFIG_RG_SSL");
    token_set_y("CONFIG_RG_OPENSSL_MD5");
    token_set_y("CONFIG_RG_JNLP");
    token_set_y("CONFIG_RG_MGT");
    token_set_y("CONFIG_RG_DSLHOME");
    token_set_y("CONFIG_RG_WGET");
    token_set_y("CONFIG_LOCAL_WBM_LIB");
    token_set_y("CONFIG_RG_SESSION_MYSQL");
    token_set_y("HAVE_MYSQL");
    token_set_y("CONFIG_RG_JNET_SERVER");
    token_set_y("CONFIG_RG_AJAX_SERVER");
    token_set("CONFIG_RG_JPKG_DIST", "JPKG_LOCAL_I386");
    token_set("TARGET_MACHINE", "local_i386");
    token_set_y("CONFIG_RG_LANG");
    token_set_y("CONFIG_GLIBC");
    token_set_y("CONFIG_RG_LIBIMAGE_DIM");
    token_set_y("CONFIG_RG_SQL");
    token_set_y("CONFIG_RG_LOCAL_WPA_UTILS");
    token_set_y("CONFIG_RG_JRMS_REDUCE_SUPPORT");    
    token_set_y("CONFIG_RG_JRMS_USERS_SUPPORT");
    token_set_y("CONFIG_RG_CMD");
    token_set_y("CONFIG_RG_REDUCE_SUPPORT");
    token_set_y("CONFIG_RG_FIREWALL");
    token_set_y("CONFIG_RG_WEB"); 
    token_set_y("CONFIG_RG_WEB_UTIL_LIB");
    token_set_y("CONFIG_RG_WEB_GRAPHICS_LIB");
    token_set_y("CONFIG_RG_WEB_NETWORK_LIB");
    token_set_y("CONFIG_RG_WEB_RG_LIB");
    token_set_y("CONFIG_RG_SSL_ROOT_CERTIFICATES");
    token_set_y("CONFIG_RG_JQUERY");
    token_set_y("CONFIG_RG_JRMS_SYSLOG");
    token_set_y("CONFIG_RG_JRMS_MONITORING");
    token_set_y("CONFIG_RG_JRMS_PBX");
    token_set_y("CONFIG_RG_JRMS_DOCSIS");
    token_set_y("CONFIG_RG_JRMS_PPP_AUTHENTICATION");
    token_set_y("CONFIG_RG_JRMS_WEB_AUTH");
    token_set_y("CONFIG_RG_JRMS_HW_DSL_WAN");
    token_set_y("CONFIG_RG_JRMS_IPERF");
    token_set_y("CONFIG_RG_JRMS_LOG_VOIP_RTP_STATS");
    token_set_y("CONFIG_HEADEND_OSAP");
    token_set_y("CONFIG_RG_WEB_SERVICE");    
    token_set_y("CONFIG_RG_CURL");    
    token_set_y("CONFIG_HEADEND_OSAP_OPAQUE_IDS");
    token_set_y("CONFIG_HEADEND_OSAP_OPAQUE_URL");
    token_set_y("CONFIG_HEADEND_OSAP_STORAGE");
    token_set_y("CONFIG_HEADEND_OSAP_MEDIA");
    token_set_y("CONFIG_RG_FAST_CGI");    
    token_set("CONFIG_JRMS_APPS_LOCATION", "/usr/lib/jrms-jacs");
    token_set("CONFIG_JRMS_UI_APPS_LOCATION", "/usr/lib/jrms-ui");
    token_set("CONFIG_JRMS_UI_HTDOCS_LOCATION", "/usr/share/jrms-ui/htdocs");
    token_set("CONFIG_JRMS_UI_STYLES_LOCATION", "%s/styles",
	token_getz("CONFIG_JRMS_UI_HTDOCS_LOCATION"));
    token_set("CONFIG_JRMS_AJAX_HTDOCS_LOCATION", 
	"/usr/share/jrms-ajax/htdocs");
    token_set("CONFIG_JRMS_SCR_HTDOCS_LOCATION", "/usr/share/jrms-scr/htdocs");
    token_set("CONFIG_JRMS_VOIP_VOICEMAIL_PASSWORDS_LOCATION",
	"/var/spool/asterisk/voicemail_passwords");
    token_set("CONFIG_JRMS_VOIP_PHONE_NUMBERS_LOCATION",
	"/var/spool/asterisk/phone_numbers");
    token_set_y("CONFIG_JRMS_OSAP_WBM");
}

static void set_vas_configs(void)
{
    token_set_y("CONFIG_RG_VAS_PORTAL");
    token_set_y("CONFIG_RG_JQUERY");

    token_set_y("CONFIG_RG_HTTPS");
    token_set_y("CONFIG_RG_SSL");
    token_set_y("CONFIG_RG_OPENSSL_MD5");
    token_set_y("CONFIG_RG_JNLP");
    token_set_y("CONFIG_RG_MGT");
    token_set_y("CONFIG_RG_WGET");
    token_set_y("CONFIG_RG_SESSION_MYSQL");
    token_set_y("HAVE_MYSQL");
    token_set("CONFIG_RG_JPKG_DIST", "JPKG_LOCAL_I386");
    token_set("TARGET_MACHINE", "local_i386");
    token_set_y("CONFIG_RG_LANG");
    token_set_y("CONFIG_GLIBC");
    token_set_y("CONFIG_RG_LIBIMAGE_DIM");
    token_set_y("CONFIG_RG_SQL");
}

static void set_hosttools_configs(void)
{
    /***************************************************************************
    * this function created due to bug B32553 . until the bug will fix         *
    * we need to seperate the configs used by the HOSTTOOLS dist to 2 sets     *
    * 1 - contain CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY which can turn on by the  *
    * dist         but not by the jpkg                                         * 
    * 2 - contain configs which should also turn on by the JPKG                *
    ***************************************************************************/
    token_set_y("CONFIG_RG_ZLIB");
    token_set_y("CONFIG_RG_TOOLS");
    token_set("CONFIG_RG_DISK_EMULATION_COUNT", "4");
    token_set_y("CONFIG_RG_OPENSSL");
}

char *set_dist_license(void)
{
#define DEFAULT_LIC_DIR "pkg/license/licenses/"
#define DEFAULT_LIC_FILE "license.lic"
    char *lic = NULL;

    if (IS_DIST("RTA770W"))
	lic = DEFAULT_LIC_DIR "belkin.lic";
    else if (!stat_lic_file(DEFAULT_LIC_DIR DEFAULT_LIC_FILE))
	lic = DEFAULT_LIC_DIR DEFAULT_LIC_FILE;
    else if (!stat_lic_file(DEFAULT_LIC_FILE))
	lic = DEFAULT_LIC_FILE;

    if (lic)
	token_set("LIC", lic);
    return lic;
}

static void small_flash_default_dist(void)
{
    token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
    token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

    enable_module("MODULE_RG_FOUNDATION");
    enable_module("MODULE_RG_UPNP");
    enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    enable_module("MODULE_RG_ADVANCED_ROUTING");
    enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    enable_module("MODULE_RG_VLAN");
    enable_module("MODULE_RG_PPP");
    enable_module("MODULE_RG_PPTP");
    enable_module("MODULE_RG_L2TP");
    enable_module("MODULE_RG_QOS");
    enable_module("MODULE_RG_ROUTE_MULTIWAN");
    enable_module("MODULE_RG_URL_FILTERING");
    enable_module("MODULE_RG_DSLHOME");
    enable_module("MODULE_RG_TR_064");
    enable_module("MODULE_RG_DSL");
    enable_module("MODULE_RG_SSL_VPN");
    enable_module("MODULE_RG_REDUCE_SUPPORT");
    enable_module("MODULE_RG_OSAP_AGENT");

    token_set_y("CONFIG_ULIBC_SHARED");
}

static void set_jpkg_dist_configs(char *jpkg_dist)
{
    int is_src = !strcmp(jpkg_dist, "JPKG_SRC");

    if (is_src || !strcmp(jpkg_dist, "JPKG_ARMV6_COMCERTO"))
    {
	jpkg_dist_add("C1KMFCNEVM");
	enable_module("MODULE_RG_MODULAR_UPGRADE");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_CHILDREN_NETWORK");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_UML"))
    {
	jpkg_dist_add("UML");
	jpkg_dist_add("UML_GLIBC");
	jpkg_dist_add("UML_26");
	jpkg_dist_add("RGLOADER_UML");
	jpkg_dist_add("UML_VALGRIND");

	set_hosttools_configs();
	enable_module("MODULE_RG_FULL_PBX");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB_INFINEON"))
    {
	jpkg_dist_add("DANUBE");
	jpkg_dist_add("TWINPASS");
	jpkg_dist_add("IAD303");
	jpkg_dist_add("IAD305");
	
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("CONFIG_RG_ATHEROS_HW_AR5212");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_VOIP_OSIP");
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_PSE");

	enable_module("CONFIG_HW_ENCRYPTION");
	token_set_y("CONFIG_CRYPTO_DANUBE");
	enable_module("MODULE_RG_MODULAR_UPGRADE");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_LX4189_ADI"))
    {
	jpkg_dist_add("AD6834");
        enable_module("CONFIG_HW_80211G_RALINK_RT2561");

	enable_module("MODULE_RG_VOIP_OSIP");
	enable_module("MODULE_RG_PSE");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_LX4189"))
    {
	jpkg_dist_add("HIMALAYA2");
	
	token_set_y("CONFIG_RG_OS_LINUX_26");
	/* List of modules that are currently not in the default image due to
	 * footprint issues */
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_VOIP_OSIP");
        enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("CONFIG_HW_USB_RNDIS");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_SSL_VPN");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB_IKANOS"))
    {
	jpkg_dist_add("CAMEROON-IFE6-VDSL");
	jpkg_dist_add("CAMEROON-IFE6-ADSL");
	
	token_set_y("CONFIG_RG_OS_LINUX_26");
	/* List of modules that are currently not in the default image due to
	 * footprint issues */
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("CONFIG_HW_USB_RNDIS");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_SSL_VPN");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_ARM_920T_LE"))
    {
	jpkg_dist_add("CENTROID");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
        enable_module("CONFIG_HW_80211G_RALINK_RT2561");
	/* XXX MODULE_RG_ADVANCED_MANAGEMENT Needs DYN_LINK 
	 * and Dyn link causes a crash on CENTROID.
	 * B30410
	 */
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_ARMV5L"))
    {
	jpkg_dist_add("SOLOS");
	
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_VLAN");
	/* Can't be included in JPKG_ARMV5L  because B37659
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	*/
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");	
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	/* Can't be included in JPKG_ARMV5L because B3774
	enable_module("MODULE_RG_VOIP_OSIP");
	*/
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
        enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	/* Needed for voip compilation */
	token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");

        enable_module("CONFIG_HW_80211G_RALINK_RT2561");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_ARMV4L"))
    {
	jpkg_dist_add("MALINDI");
	jpkg_dist_add("MALINDI2");
	
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	/* Can't be included in JPKG_ARMV4L because B37659
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	*/
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
        enable_module("CONFIG_HW_80211G_RALINK_RT2560");
        enable_module("CONFIG_HW_80211G_RALINK_RT2561");
	/* Can't be included in JPKG_ARMV4L because B3774
	enable_module("MODULE_RG_VOIP_OSIP");
	*/
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_FULL_PBX");
	token_set_y("CONFIG_RG_DSLHOME_VOUCHERS"); /* removed from MALINDI2 */
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_ARMV6J"))
    {
	//jpkg_dist_add("PACKET-IAD");
	//jpkg_dist_add("FE-ROUTER");
	jpkg_dist_add("BB-ROUTER");

	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	/* Can't be included in JPKG_ARMV4L because B37659
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	*/
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
        enable_module("CONFIG_HW_80211G_RALINK_RT2560");
        enable_module("CONFIG_HW_80211G_RALINK_RT2561");
	enable_module("CONFIG_HW_80211N_RALINK_RT2860");
	/* Can't be included in JPKG_ARMV4L because B3774
	enable_module("MODULE_RG_VOIP_OSIP");
	*/
	enable_module("MODULE_RG_ATA");
	token_set_y("CONFIG_RG_DSLHOME_VOUCHERS"); /* removed from MALINDI2 */
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_IPSEC");
        enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	token_set_m("CONFIG_IPSEC_ALG_JOINT");

	/* these are needed for VLAN */
	token_set_y("CONFIG_RG_IPROUTE2");
	token_set_m("CONFIG_RG_BRIDGE");
	enable_module("MODULE_RG_VLAN");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_KS8695"))
    {
	jpkg_dist_add("CENTAUR_VGW");
	jpkg_dist_add("CENTAUR");
	jpkg_dist_add("RGLOADER_CENTAUR");

	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	/* Needed for voip compilation */
	token_set("CONFIG_HW_NUMBER_OF_FXS_PORTS", "2");
    }    
    if (is_src || !strcmp(jpkg_dist, "JPKG_PPC"))
    {
	/* XXX Restore these dists in B36583 
	jpkg_dist_add("EP8248_26");
	token_set_y("CONFIG_HW_BUTTONS");
	*/
	jpkg_dist_add("MPC8272ADS");
	jpkg_dist_add("MPC8349ITX");

	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_FTP_SERVER");
        enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_JVM");
	/* Can't be included in JPKG_PPC because B37659
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	*/
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	/* Wireless */
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("CONFIG_RG_ATHEROS_HW_AR5212");
	enable_module("CONFIG_RG_ATHEROS_HW_AR5416");
	enable_module("CONFIG_HW_80211N_RALINK_RT2860");
	token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "0");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_SB1250"))
    {
	jpkg_dist_add("BCM91125E");
	jpkg_dist_add("BCM_SB1125");
	enable_module("MODULE_RG_PSE");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_OCTEON"))
    {
	jpkg_dist_add("CN3XXX");

	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB"))
    {
	jpkg_dist_add("DWV_96358");
	jpkg_dist_add("HH1620");
	jpkg_dist_add("CT6382D");

	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_IPSEC");
	/* Can't be included in JPKG_MIPSEB because B37659
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	*/
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");	
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	/* Can't be included in JPKG_MIPSEB because B3774
	enable_module("MODULE_RG_VOIP_OSIP");
	*/
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("CONFIG_HW_USB_RNDIS");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_TR_064");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");
	enable_module("MODULE_RG_VLAN");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEL"))
    {
	jpkg_dist_add("OLYMPIA_P402");

	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB_XWAY"))
    {
	enable_module("CONFIG_RG_ATHEROS_HW_AR5416");
	token_set_y("CONFIG_RG_ATHEROS_FUSION");
	token_set("CONFIG_ATHEROS_AR5008_PCI_SWAP", "1");

	jpkg_dist_add("ARX188");
	jpkg_dist_add("ARX188_ETH");
	jpkg_dist_add("VRX288");

	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_FULL_PBX");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_TR_064");
	enable_module("CONFIG_HW_ENCRYPTION");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");	
	enable_module("MODULE_RG_MODULAR_UPGRADE");
	enable_module("MODULE_RG_CHILDREN_NETWORK");
	token_set_y("CONFIG_RG_MTD_UTILS");

	/* JPKG needs all possible networking drivers */
	token_set_y("CONFIG_ATM");
	token_set_y("CONFIG_IFX_ATM");
	token_set_y("CONFIG_IFX_PTM");
	token_set_y("CONFIG_IFX_PPA_A5");
	token_set_y("CONFIG_IFX_PPA_D5");
	token_set_y("CONFIG_IFX_PPA_E5");
	token_set_y("CONFIG_IFX_PPA_DATAPATH");
	token_set_y("CONFIG_IFX_PPA_API_PROC");	
	token_set_y("CONFIG_IFX_3PORT_SWITCH");
	token_set_y("CONFIG_IFX_7PORT_SWITCH");
	token_set_y("CONFIG_IFX_ETH_FRAMEWORK");
	token_set_y("CONFIG_RG_LANTIQ_XWAY_DYN_DATAPATH");
	token_set_y("CONFIG_RG_LANTIQ_XWAY_STATIC_DATAPATH");	
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_BCM9634X"))
    {
	jpkg_dist_add("ASUS6020VI");
	jpkg_dist_add("WADB100G");
	jpkg_dist_add("WADB102GB");

	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB_BCM9636X"))
    {
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_MIPSEB_BCM9338X"))
    {
	jpkg_dist_add("BCM93383");

	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_MODULAR_UPGRADE");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_ARMV5L_FEROCEON"))
    {
	jpkg_dist_add("DB88F6560BP");
	jpkg_dist_add("FIBERTEC_FEROCEON");
	jpkg_dist_add("VERIZON_FEROCEON");
	jpkg_dist_add("NCS_FEROCEON");
	jpkg_dist_add("FIBERTEC_DB88F6560BP");
    }
    if (is_src || !strcmp(jpkg_dist, "JPKG_LOCAL_I386"))
    {
	/* Can't use JNET_SERVER in JPKG_SRC because it turns on
	 * CONFIG_RG_USE_LOCAL_TOOLCHAIN, which we don't want.
	 * Remove this when B32553 is fixed.
	 */
	/* do not create jpkgs for jnet/jrms for now
	 * When branch-dev-jrms-packages is merged, uncomment these lines  
	if (is_src)
	    set_jnet_server_configs();
	else
	    jpkg_dist_add("JNET_SERVER");
	*/
    }
    if (is_src)
    {
	token_set_y("CONFIG_RG_DOC_SDK");
	token_set_y("CONFIG_RG_SAMPLES_COMMON");	
	token_set_y("CONFIG_RG_TCPDUMP");
	token_set_y("CONFIG_RG_LIBPCAP");
	token_set_y("CONFIG_RG_IPROUTE2_UTILS");
        token_set_y("CONFIG_RG_JAVA");
        token_set_y("CONFIG_RG_JTA");
        token_set_y("CONFIG_RG_PROPER_JAVA_RDP");
        token_set_y("CONFIG_RG_JVFTP");
        token_set_y("CONFIG_RG_JCIFS");
        token_set_y("CONFIG_RG_SMB_EXPLORER");
        token_set_y("CONFIG_RG_TIGHT_VNC");
	token_set_y("GLIBC_IN_TOOLCHAIN");	
	token_set_y("CONFIG_RG_JNET_SERVER_TUTORIAL");
	token_set_y("CONFIG_RG_OSAP_AGENT");
    }
    else
    {
        token_set_y("CONFIG_RG_JPKG_BIN");
	/* JPKG consistency test require these CONFIGs to be set for all ARCHs 
	 */
	token_set_y("CONFIG_RG_GDBSERVER");
	token_set_y("CONFIG_RG_KERNEL");
	token_set_y("CONFIG_RG_SAMPLES_COMMON");
    }

    /* Common additional features: */
    token_set_y("CONFIG_RG_JPKG");
    if (strcmp(jpkg_dist, "JPKG_LOCAL_I386"))
    {
	/* These shouldn't be turned on in binary local jpkg */
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	token_set_y("CONFIG_RG_TOOLS");
	token_set_y("CONFIG_RG_CONF_INFLATE");
        token_set_y("CONFIG_OPENRG");
        enable_module("MODULE_RG_DSLHOME");
    }
    /* Add all event configs */
    token_set_y("CONFIG_TARGET_EVENT_EPOLL");
    token_set_y("CONFIG_TARGET_EVENT_POLL");
    token_set_y("CONFIG_TARGET_EVENT_SELECT");
    token_set_y("CONFIG_LOCAL_EVENT_EPOLL");
    token_set_y("CONFIG_LOCAL_EVENT_POLL");
    token_set_y("CONFIG_LOCAL_EVENT_SELECT");
}

void distribution_features()
{
    if (!dist)
	conf_err("ERROR: DIST is not defined\n");

    /* MIPS */
    if (IS_DIST("DANUBE_LSP") || IS_DIST("TWINPASS_LSP"))
    {
	hw = "DANUBE";
	os = "LINUX_24";
	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_HW_ETH_LAN");
        token_set_y("CONFIG_RG_VOIP_DEMO");
	enable_module("CONFIG_HW_DSP");	
    }
    else if (IS_DIST("DANUBE") || IS_DIST("TWINPASS") || IS_DIST("IAD303") ||
	IS_DIST("IAD305") || IS_DIST("IAD305_DECT") || IS_DIST("IAD303_OPENRG"))
    {
	if (IS_DIST("DANUBE"))
	    hw = "DANUBE";
	else if (IS_DIST("IAD303") || IS_DIST("IAD305") ||
	    IS_DIST("IAD305_DECT") || IS_DIST("IAD303_OPENRG"))
	{
	    hw = "IAD303";
	}
	else if (IS_DIST("TWINPASS"))
	    hw = "TWINPASS";

	os = "LINUX_24";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB_INFINEON");
	
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	if (IS_DIST("IAD303_OPENRG"))
	{
	    token_set_y("CONFIG_RG_INTEGRAL");
	    token_set("DEVICE_MANUFACTURER_STR", "VTech");
	}

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_UPNP");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("CONFIG_RG_WPS");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	if (IS_DIST("DANUBE") || IS_DIST("IAD303") || IS_DIST("IAD305") ||
	     IS_DIST("IAD305_DECT") || IS_DIST("IAD303_OPENRG"))
	{
	    enable_module("MODULE_RG_DSL");
	}
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	if (!IS_DIST("IAD303_OPENRG"))
	{
	    enable_module("MODULE_RG_SSL_VPN");
	    enable_module("MODULE_RG_WEB_SERVER");
	    enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	}
	enable_module("MODULE_RG_PSE");

	/*  RG Priority 2  */
	if (!IS_DIST("IAD303_OPENRG"))
	{
	    enable_module("MODULE_RG_PPTP");
	    enable_module("MODULE_RG_L2TP");
	    enable_module("MODULE_RG_PRINTSERVER");
	}
	/*  RG Priority 3  */
	if (!IS_DIST("IAD303_OPENRG"))
	    enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */ 
	if (!IS_DIST("IAD303_OPENRG"))
	{
	    enable_module("MODULE_RG_URL_FILTERING");
	    enable_module("MODULE_RG_TR_064");
	}
	/* Not enough space on flash
	enable_module("MODULE_RG_IPSEC");
	enable_module("CONFIG_HW_ENCRYPTION");
	token_set_y("CONFIG_IPSEC_USE_DANUBE_CRYPTO");
	enable_module("MODULE_RG_JVM");
	*/

	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");

	enable_module("CONFIG_HW_USB_STORAGE");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_DSP");
	if (IS_DIST("DANUBE") || IS_DIST("TWINPASS"))
	    enable_module("CONFIG_RG_ATHEROS_HW_AR5416");
	else if (IS_DIST("IAD303"))
	    enable_module("CONFIG_HW_80211N_RALINK_RT2860");
	else
	    enable_module("CONFIG_HW_80211N_RALINK_RT3062");
	if (IS_DIST("DANUBE") || IS_DIST("IAD303") || IS_DIST("IAD305") ||
	    IS_DIST("IAD303_OPENRG") || IS_DIST("IAD305_DECT"))
	{
	    if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	    {
		token_set_y("CONFIG_HW_ETH_WAN");
		switch_virtual_port_dev_add("eth1", "eth0_main", 
		    IS_HW("IAD303") || IS_DIST("IAD305") || 
		    IS_DIST("IAD305_DECT") ? 1 : 3, DEV_IF_NET_EXT);
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0_main", NULL);
	    }
	    else
		dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	    token_set_y("CONFIG_HW_DSL_WAN");
	    token_set_m("CONFIG_HW_LEDS");
	    token_set_m("CONFIG_HW_BUTTONS");

	    token_set_m("CONFIG_RG_KLOG");
	}
	else
	{
	    /* TWINPASS */
	    dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);
	    token_set_y("CONFIG_HW_VDSL_WAN");
	    if (token_get("MODULE_RG_PSE"))
		token_set_y("CONFIG_RG_PSE_HW");
	    token_set_y("CONFIG_RG_HW_QOS");
	}
	if (IS_DIST("IAD303") || IS_DIST("IAD303_OPENRG") ||
	    IS_DIST("IAD305_DECT"))
	{
	    token_set_y("CONFIG_HW_FXO");
	}
	if (IS_DIST("IAD303_OPENRG"))
	{
	    token_set_y("CONFIG_RG_VAP_SECURED");
	    if (token_get("MODULE_RG_REDUCE_SUPPORT"))
		token_set_y("CONFIG_RG_VAP_HELPLINE");
#ifdef CONFIG_RG_DO_DEVICES
	    token_set("CONFIG_RG_DEFAULT_80211N_MODE", "%d", DOT11_MODE_NG);
#endif
	}
	token_set_y("CONFIG_DYN_LINK");

	if (IS_DIST("IAD305_DECT"))
	{
	    token_set_y("CONFIG_HW_DECT");
	    token_set_y("CONFIG_RG_DECT");
	    token_set_y("CONFIG_VTECH_DECT");
	    token_set_y("CONFIG_RG_VOIP_CALL_LOG");
	}

	/* Debugging tools */
#if 0	/* Enable those features only for development compilations */
	token_set_y("CONFIG_RG_DBG_ULIBC_MALLOC");
	/* Redirect malloc error messages from stdout to /dev/console */
	token_set_y("CONFIG_RG_DBG_MALLOC_2_CON");
	/* Crash task in case of found memory error */
	token_set_y("CONFIG_RG_DBG_ULIBC_MALLOC_CRASH");
	/* This feature fills freed memory with 0xeb value. If you found case of
	 * usage released memory - use is_mem_valid(ptr) for memory validation.
	 * Impairs board performance severely!!! */ 
	token_set("CONFIG_RG_DBG_ULIBC_MEM_POISON", "256");
#endif
    }
    else if (IS_DIST("ARX188_LSP"))
    {
	hw = "ARX188";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set("LIBC_IN_TOOLCHAIN", "y");
	
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");
    }
    else if (IS_DIST("ARX188") || IS_DIST("ARX188_ETH"))
    {
	hw = "ARX188";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB_XWAY");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");
	
	/*  RG Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	/* Not tested
	 * enable_module("MODULE_RG_REDUCE_SUPPORT");
	 */
	enable_module("CONFIG_HW_80211N_LANTIQ_WAVE300");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("CONFIG_RG_WPS");
	if (!IS_DIST("ARX188_ETH"))
	    enable_module("MODULE_RG_DSL");

	/*  RG Priority 2  */
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("CONFIG_HW_DSP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("CONFIG_HW_USB_STORAGE");	
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_PSE");
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");

#if 0 /* Not enough space in reference board flash (8MB) */
	/*  RG Priority 3  */
	enable_module("MODULE_RG_IPSEC");
	enable_module("CONFIG_HW_ENCRYPTION");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");

	/*  RG Priority 4  */
	enable_module("MODULE_RG_PRINTSERVER");
#endif

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	/* Ethernet WAN is available in ADSL distribution as
	 * well, however, it cannot be accelerated.
	 */
	token_set_y("CONFIG_HW_ETH_WAN");
        if (token_get("MODULE_RG_DSL"))
            token_set_y("CONFIG_HW_DSL_WAN");

	/* this enslaves all LAN devices, if no other device is enslaved
	 * explicitly */
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	/* build options */
	//token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");
    }
    else if (IS_DIST("VRX288_LSP") || IS_DIST("GRX288_LSP"))
    {
	if (IS_DIST("VRX288_LSP"))
	    hw = "VRX288";
	else
	    hw = "GRX288";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set("LIBC_IN_TOOLCHAIN", "y");
	
	token_set_y("CONFIG_HW_ETH_LAN");
	if (IS_DIST("VRX288_LSP"))
	    token_set_y("CONFIG_HW_VDSL_WAN");
    }
    else if (IS_DIST("VRX288") || IS_DIST("GRX288"))
    {
	if (IS_DIST("VRX288"))
	    hw = "VRX288";
	else
	    hw = "GRX288";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB_XWAY");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");
	
	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");
	if (IS_DIST("VRX288"))
	{
	    token_set_y("CONFIG_HW_VDSL_WAN");
	    token_set_y("CONFIG_HW_DSL_WAN");
	    enable_module("MODULE_RG_DSL");
	}

	/*  RG Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	/* Not tested
	 * enable_module("MODULE_RG_REDUCE_SUPPORT");
	 */
	enable_module("CONFIG_HW_80211N_LANTIQ_WAVE300");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("CONFIG_RG_WPS");

	/*  RG Priority 2  */
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("CONFIG_HW_DSP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("CONFIG_HW_USB_STORAGE");	
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_PSE");
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");

	/*  RG Priority 3  */
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_IPSEC");
	enable_module("CONFIG_HW_ENCRYPTION");
	enable_module("MODULE_RG_CHILDREN_NETWORK");

	/*  RG Priority 4  */
	enable_module("MODULE_RG_PRINTSERVER");

	/* this enslaves all LAN devices, if no other device is enslaved
	 * explicitly */
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);
        
	/* build options */
	//token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");

	if (IS_DIST("VRX288"))
	{
	    token_set_y("CONFIG_RG_PERM_STORAGE_UBI");

	    /* Disable CONFIG_HW_FLASH_NOR to avoid having 2 OpenRG volumes */
	    token_set("CONFIG_HW_FLASH_NOR", "n");

	    /* Volume size for permst - OpenRG Flash layout */
	    token_set("CONFIG_RG_PERM_STORAGE_UBI_VOLUME_SIZE", "0x33ae000");
	    /* Partion Size ncludes the volume size, 4 PEBs for UBI and 80 
	     * possible bab-blocks (worst case scenario).
	     */
	    token_set("CONFIG_RG_UBI_PARTITION_SIZE", "0x3f00000");
	    /* 1MiB for u-boot and it's data at the beginning of the flash */
	    token_set("CONFIG_RG_UBI_PARTITION_OFFSET", "0x100000");
	}
    }
    else if (IS_DIST("BCM91125E") || IS_DIST("BCM_SB1125"))
    {
    	if (IS_DIST("BCM91125E"))
	    hw = "BCM91125E";
	else if (IS_DIST("BCM_SB1125"))
	    hw = "COLORADO";
	os = "LINUX_26";
	
	token_set_y("CONFIG_RG_SMB");
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_SB1250");

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");

	token_set_y("CONFIG_DYN_LINK");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_PSE");

	/*  SMB Priority 2  */
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");

	/*  SMB Priority 3  */
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");

	/*  SMB Priority 4  */
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	/* Can't add JVM to BCM1125E see B38742    	
	enable_module("MODULE_RG_JVM");*/

	/*  SMB Priority 5  */
	/* Can't add H323 to BCM1125 see B38603    	
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");*/
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	
	/* Only Ethernet HW for now */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");

	if (IS_DIST("BCM_SB1125"))
	{
	    /* Disk (USB) dependent modules */
	    enable_module("MODULE_RG_BLUETOOTH");
	    enable_module("MODULE_RG_FILESERVER");
	    enable_module("MODULE_RG_ACCESS_DLNA");
	    enable_module("MODULE_RG_MEDIA_SERVER");
	    enable_module("MODULE_RG_MAIL_SERVER");
	    enable_module("MODULE_RG_WEB_SERVER");
	    enable_module("MODULE_RG_FTP_SERVER");
	    enable_module("MODULE_RG_PRINTSERVER");
	    enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	    enable_module("MODULE_RG_FULL_PBX");
	    enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	}

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }
    else if (IS_DIST("CN3XXX_LSP"))
    {
	hw = "CN3XXX";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");
	
	token_set("LIBC_IN_TOOLCHAIN", "y");
	token_set_y("CONFIG_GLIBC");

	token_set_y("CONFIG_HW_ETH_LAN");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }
    else if (IS_DIST("CN3XXX"))
    {
	hw = "CN3XXX";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_OCTEON");

	token_set_y("CONFIG_RG_SMB");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_WEB_SERVER");

	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	if (token_get("MODULE_RG_REDUCE_SUPPORT"))
	    enable_module("MODULE_RG_HOME_PBX");
	else
	    enable_module("MODULE_RG_FULL_PBX");

	enable_module("MODULE_RG_VOIP_ASTERISK_SIP"); 

	enable_module("MODULE_RG_PSE");

	/*  SMB Priority 2  */
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	
	/*  SMB Priority 3  */
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	
	/*  SMB Priority 4  */
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	if (!token_get("MODULE_RG_REDUCE_SUPPORT"))
	    enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_FTP_SERVER");
	/* pkg/kaffe doesn't support 64bit */
	//enable_module("MODULE_RG_JVM");

	/*  SMB Priority 5  */
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	token_set_y("CONFIG_DYN_LINK");
	token_set_y("CONFIG_RG_IMAGE_ELF");
	
	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");

	/* HW Configuration Section */

	enable_module("CONFIG_HW_ENCRYPTION");

	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_USB_STORAGE");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	/* Fast Path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

	/* Currently we use only 1 8M flash and boot from network. Ignore the
	 * image size
	 */
	token_set_y("CONFIG_RG_IGNORE_IMAGE_SECTION_SIZE");
    }
    else if (IS_DIST("BCM94702"))
    {
	hw = "BCM94702";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("BCM94704"))
    {
	hw = "BCM94704";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_SNMP");
    	token_set_y("CONFIG_RG_EVENT_LOGGING"); /* Event Logging */
	token_set_y("CONFIG_RG_ENTFY");	/* Email notification */

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", "eth0", NULL);
    }
    else if (IS_DIST("USI_BCM94712"))
    {
	hw = "BCM94712";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0.0", "eth0", NULL);

	token_set_y("CONFIG_DYN_LINK");
    }
    else if (IS_DIST("SRI_USI_BCM94712"))
    {
	hw = "BCM94712";

	token_set_y("CONFIG_RG_SMB");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");

	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_SNMP");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	
	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0.0", "eth0", NULL);

	/* The Broadcom nas application is dynamically linked */
	token_set_y("CONFIG_DYN_LINK");

	/* Wireless GUI options */
	/* Do NOT show Radius icon in advanced */
	token_set_y("CONFIG_RG_RADIUS_WBM_IN_CONN");
    }
    else if (IS_DIST("RTA770W"))
    {
	hw = "RTA770W";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	/* broadcom nas (wpa application) needs ulibc.so so we need to compile
	 * openrg dynamically */
	token_set_y("CONFIG_DYN_LINK");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	token_set_y("CONFIG_RG_IGD_XBOX");
	enable_module("MODULE_RG_DSL");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("CONFIG_HW_USB_RNDIS");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", "usb0", "wl0", NULL);

	token_set("RG_PROD_STR", "Prodigy infinitum");
	token_set_y("CONFIG_RG_DSL_CH");
	token_set_y("CONFIG_RG_PPP_ON_DEMAND_AS_DEFAULT");
	/* Belkin's requirement - 3 hours of idle time */
	token_set("CONFIG_RG_PPP_ON_DEMAND_DEFAULT_MAX_IDLE_TIME", "10800");
	/* from include/enums.h PPP_COMP_ALLOW is 1 */
	token_set("CONFIG_RG_PPP_DEFAULT_BSD_COMPRESSION", "1");
	/* from include/enums.h PPP_COMP_ALLOW is 1 */
	token_set("CONFIG_RG_PPP_DEFAULT_DEFLATE_COMPRESSION", "1");
	/* Download image to memory before flashing
	 * Only one image section in flash, enough memory */
	token_set_y("CONFIG_RG_RMT_UPGRADE_IMG_IN_MEM");
	/* For autotest and development purposes, Spanish is the default
	 * language allowing an override */
	if (!token_get_str("CONFIG_RG_DIST_LANG"))
	    token_set("CONFIG_RG_DIST_LANG", "spanish_belkin");
	token_set_y("CONFIG_RG_CFG_SERVER");
	token_set_y("CONFIG_RG_OSS_RMT");
	token_set_y("CONFIG_RG_RMT_MNG");
	token_set_y("CONFIG_RG_NON_ROUTABLE_LAN_DEVICE_IP");
    }
    else if (IS_DIST("RTA770W_EVAL"))
    {
	hw = "RTA770W";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_DSL");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("CONFIG_HW_USB_RNDIS");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", "usb0", "wl0", NULL);

	/* Download image to memory before flashing
	 * Only one image section in flash, enough memory */
	token_set_y("CONFIG_RG_RMT_UPGRADE_IMG_IN_MEM");
	/* broadcom nas (wpa application) needs ulibc.so so we need to compile
	 * openrg dynamically */
	token_set_y("CONFIG_DYN_LINK");
    }
    else if (IS_DIST("RGLOADER_RTA770W"))
    {
	hw = "RTA770W";

	token_set_y("CONFIG_RG_RGLOADER");
	token_set_y("CONFIG_BCM963XX_BOOTSTRAP");
	token_set_m("CONFIG_RG_KRGLDR");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_LAN2");
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("GTWX5803"))
    {
	hw = "GTWX5803";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_PSE");
	
	/*  RG Priority 2  */
    	enable_module("MODULE_RG_PPTP");
    	enable_module("MODULE_RG_L2TP");
	
	/*  RG Priority 3  */
    	enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */
	/*Not enough space on flash 
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_JVM");
	*/

	/*  RG Priority 7  */
	/*Not enough space on flash 
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	*/

	token_set_y("CONFIG_ULIBC_SHARED");

	token_set_y("CONFIG_RG_MTD");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("CONFIG_HW_USB_RNDIS");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", NULL);
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");
    }
    else if (IS_DIST("RGLOADER_GTWX5803"))
    {
	hw = "GTWX5803";

	token_set_y("CONFIG_RG_RGLOADER");
	token_set_y("CONFIG_RG_TELNETS");
	token_set_y("CONFIG_BCM963XX_BOOTSTRAP");
	token_set_m("CONFIG_RG_KRGLDR");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");
	token_set_y("CONFIG_RG_MTD");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("WRT54G"))
    {
	hw = "WRT54G";

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_SNMP");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	token_set_m("CONFIG_HW_BUTTONS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0.2", "eth0", NULL);

	token_set_y("CONFIG_ARCH_BCM947_CYBERTAN");
	token_set_y("CONFIG_RG_BCM947_NVRAM_CONVERT");
	token_set_y("CONFIG_DYN_LINK");
    }
    else if (IS_DIST("SOLOS_LSP") || IS_DIST("SOLOS"))
    {
	hw = "CX9451X";
	os = "LINUX_26";
	token_set_y("CONFIG_ARCH_SOLOS");
	
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	if (IS_DIST("SOLOS_LSP"))
	{
	    token_set_y("CONFIG_RG_KGDB");
	    token_set_y("CONFIG_LSP_DIST");
	}
	else
	{

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_ATA");
	enable_module("MODULE_RG_PSE");
	
	/*  RG Priority 2  */
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_PRINTSERVER");

	/*  RG Priority 3  */
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */
	enable_module("MODULE_RG_IPSEC");
           token_set_y("CONFIG_IPSEC_USE_SOLOS_CRYPTO");
	/* Can't add H323 to SOLOS see B38603    	
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");*/	
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_MAIL_SERVER");	
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");

	/*  RG Priority 7  */
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SNMP");
	/* Can't add IPV6 to SOLOS see B38590 
	enable_module("MODULE_RG_IPV6");  */

	    /* HW configuration */
	    enable_module("CONFIG_HW_DSP");
	    enable_module("CONFIG_HW_USB_STORAGE");
	    token_set_y("CONFIG_HW_DSL_WAN");
	    token_set_y("CONFIG_HW_ETH_WAN");
	    token_set_y("CONFIG_HW_ETH_LAN");

	    dev_add_bridge("br0", DEV_IF_NET_INT, "lan0", NULL);
	}

	token_set_y("CONFIG_DYN_LINK");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L");
	/* XXX remove after resolving B36422 */
	token_set("CONFIG_RG_EXTERNAL_TOOLS_PATH",
	    "/usr/local/virata/tools_v10.1c/redhat-9-x86");
    }
    else if (IS_DIST("CX82100_SCHMID"))
    {
	hw = "CX82100";
	os = "LINUX_22";
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_USB_RNDIS");

	token_set_y("CONFIG_RG_TODC");
    }
    else if (IS_DIST("RGLOADER_CENTROID"))
    {
	hw = "CENTROID";

	token_set_y("CONFIG_RG_RGLOADER");
	token_set_y("CONFIG_SL2312_COMMON");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_LAN2");
    }
    else if (IS_DIST("CENTROID"))
    {
	hw = "CENTROID";
	
	token_set_y("CONFIG_RG_SMB");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	enable_module("MODULE_RG_IPSEC");
	
	token_set_y("CONFIG_RG_MTD");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
 	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	enable_module("CONFIG_HW_USB_STORAGE");
	
	token_set_y("CONFIG_SL2312_COMMON_RG");
	token_set("CONFIG_RG_FLASH_LAYOUT_SIZE", "8"); 

	dev_add_bridge("br0", DEV_IF_NET_INT, "sl0", NULL);
    }
    else if (IS_DIST("CX8620XR_LSP") || IS_DIST("CX8620XD_LSP"))
    {
	if (IS_DIST("CX8620XR_LSP"))
	    hw = "CX8620XR";
	else
	    hw = "CX8620XD";

	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_IPTABLES");
	token_set_m("CONFIG_BRIDGE");
	token_set_y("CONFIG_BRIDGE_UTILS");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	token_set_m("CONFIG_ISL_SOFTMAC");
	token_set_y("CONFIG_RG_NETKIT");
	
	token_set_y("CONFIG_CX8620X_COMMON");

	enable_module("CONFIG_HW_USB_HOST_EHCI");
    	enable_module("CONFIG_HW_USB_STORAGE");
    }
    else if (IS_DIST("CX8620XR"))
    {
	hw = "CX8620XR";
	
	token_set_y("CONFIG_CX8620X_COMMON");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
  	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	
	dev_add_bridge("br0", DEV_IF_NET_INT, "cnx0", NULL);
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L");
    }
    else if (IS_DIST("CX8620XD_FILESERVER"))
    {
	hw = "CX8620XD";

	token_set_y("CONFIG_CX8620X_COMMON");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");

	/* HW Configuration Section */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
    	enable_module("CONFIG_HW_USB_STORAGE");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
 	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	
	dev_add_bridge("br0", DEV_IF_NET_INT, "cnx0", NULL);
    }
    else if (IS_DIST("CX8620XD_SOHO"))
    {
	hw = "CX8620XD";

	token_set_y("CONFIG_CX8620X_COMMON");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");

    	token_set_y("CONFIG_RG_SMB");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_L2TP");
        enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
        enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	/* HW Configuration Section */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
    	enable_module("CONFIG_HW_USB_STORAGE");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
 	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L");

	dev_add_bridge("br0", DEV_IF_NET_INT, "cnx0", NULL);
    }
    else if (IS_DIST("MALINDI") || IS_DIST("MALINDI2"))
    {
	hw = "COMCERTO";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV4L");
	
	/* Comcerto chipset */
	token_set_y("CONFIG_COMCERTO_COMMON");

	/* Board */
	if (IS_DIST("MALINDI"))
	    token_set_y("CONFIG_COMCERTO_MALINDI");
	else if (IS_DIST("MALINDI2"))
	    token_set_y("CONFIG_COMCERTO_NAIROBI");

	token_set_y("CONFIG_RG_SMB");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");

	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_QOS");

	/* CONFIG_RG_DSLHOME_VOUCHERS is disabled in feature_config */
	enable_module("MODULE_RG_DSLHOME"); 
	
	enable_module("MODULE_RG_SSL_VPN");
        enable_module("MODULE_RG_FULL_PBX");

	/*Not enough space on flash
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_L2TP");
	*/
	enable_module("MODULE_RG_PSE");
	
	/*  SMB Priority 2  */
	/*Not enough space on flash
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	*/
	
	/*  SMB Priority 3  */
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	/*Not enough space on flash
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	*/
	
	/*  SMB Priority 4  */
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	/*Not enough space on flash
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	*/
	
	/*  SMB Priority 5  */
	/*Not enough space on flash
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	*/

	/* Cryptographic hardware accelerator */
	token_set_y("CONFIG_CADENCE_IPSEC2");

	/* HW Configuration Section */
	enable_module("CONFIG_HW_USB_HOST_OHCI");
	enable_module("CONFIG_HW_USB_STORAGE");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");
	enable_module("CONFIG_HW_ENCRYPTION");
	enable_module("CONFIG_HW_DSP");

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	/* Ralink Wi-Fi card */
	enable_module("CONFIG_HW_80211G_RALINK_RT2561");

	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

    }
    else if ( IS_DIST("PACKET-IAD") || IS_DIST("BB-ROUTER") ||
	IS_DIST("FE-ROUTER"))
    {	
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV6J");

	/* board */
	if (IS_DIST("PACKET-IAD"))
	    hw = "PACKET-IAD";
	else if (IS_DIST("BB-ROUTER"))
	    hw = "BB-ROUTER";
	else if (IS_DIST("FE-ROUTER"))
	    hw = "FE-ROUTER";

	token_set_y("CONFIG_RG_SMB");
	if (IS_DIST("BB-ROUTER"))
	    token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_HOME_PBX");
	/* CONFIG_RG_DSLHOME_VOUCHERS is disabled in feature_config */
	enable_module("MODULE_RG_DSLHOME"); 
	enable_module("MODULE_RG_UPNP"); 
	enable_module("MODULE_RG_PSE"); 
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_PPTP");
	/*Not enough space on flash
	enable_module("MODULE_RG_SSL_VPN");	
	enable_module("MODULE_RG_L2TP");
	*/
	
	/*  SMB Priority 2  */
	enable_module("MODULE_RG_VLAN");
	/*Not enough space on flash
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	*/
	
	/*  SMB Priority 3  */
	/*Not enough space on flash
	enable_module("MODULE_RG_BLUETOOTH");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE"); 
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	*/
	
	/*  SMB Priority 4  */
	/*Not enough space on flash
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
	*/
	enable_module("MODULE_RG_REDUCE_SUPPORT"); 
	enable_module("MODULE_RG_OSAP_AGENT"); 
	
	/*  SMB Priority 5  */
	/*Not enough space on flash
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	*/
	
	enable_module("CONFIG_HW_80211N_RALINK_RT2860");

	/* Comcerto chipset */
	token_set_y("CONFIG_COMCERTO_COMMON_821XX"); /* do not want to re-use CONFIG_COMCERTO_COMMON */

	/* HW Configuration Section */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_STORAGE");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");

        enable_module("CONFIG_HW_DSP");
 
	/* Fast Path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

 	/* Cryptographic hardware accelerator */
 	if (token_get("MODULE_RG_IPSEC"))
	    enable_module("CONFIG_HW_ENCRYPTION");
 
	if (IS_DIST("PACKET-IAD") || IS_DIST("BB-ROUTER"))
	    token_set_y("CONFIG_COMCERTO_VITESSE");

	if ( IS_DIST("FE-ROUTER") )
	{
	    if (token_get("CONFIG_HW_DSP"))
	    {
		/* FXS support */
		token_set_y("CONFIG_SPI_SI3215");
		/* FXO support */
		token_set_y("CONFIG_SPI_SI3050");
	    }
	    token_set_m("CONFIG_HW_BUTTONS");
	    token_set_m("CONFIG_HW_LEDS");
	}
	

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");

	dev_add_bridge("br0", DEV_IF_NET_INT, "eth2", NULL);
    }
    else if (IS_DIST("RGLOADER_CX8620XD"))
    {
	hw = "CX8620XD";

	token_set_y("CONFIG_RG_RGLOADER");
	token_set_y("CONFIG_CX8620X_COMMON_RGLOADER");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L");
    }
    else if (IS_DIST("RGLOADER_WADB100G"))
    {
    	hw = "WADB100G";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");
	token_set_y("CONFIG_RG_RGLOADER");
	token_set_y("CONFIG_BCM963XX_BOOTSTRAP");
	token_set_m("CONFIG_RG_KRGLDR");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_LAN2");
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("WADB100G"))
    {
    	hw = "WADB100G";
	os = "LINUX_24";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	
	/*  RG Priority 2  */
    	enable_module("MODULE_RG_PPTP");
    	enable_module("MODULE_RG_L2TP");
	
	/*  RG Priority 3  */
    	enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */
	/*Not enough space on flash 
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	*/

	/*  RG Priority 7  */
	/*Not enough space on flash 
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	*/
	
	token_set_y("CONFIG_ULIBC_SHARED");

	token_set_y("CONFIG_RG_MTD");

	/* OpenRG HW support */
	
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("CONFIG_HW_USB_RNDIS");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", "bcm1", NULL);
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");
    }
    else if (IS_DIST("WADB100G_26"))
    {
    	hw = "WADB100G";
	os = "LINUX_26";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	
	/*  RG Priority 2  */
    	enable_module("MODULE_RG_PPTP");
    	enable_module("MODULE_RG_L2TP");
	
	/*  RG Priority 3  */
    	enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	/*Not enough space on flash 
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_JVM");
	*/

	/*  RG Priority 7  */
	/*Not enough space on flash 
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	*/
	

	token_set_y("CONFIG_ULIBC_SHARED");

	token_set_y("CONFIG_RG_MTD");

	/* OpenRG HW support */
	
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("CONFIG_HW_USB_RNDIS");

	/* B40399: HW led/button support is required for ASUS kernel 2.6
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");
	*/

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm0", "bcm1", NULL);
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }
    else if (IS_DIST("ASUS6020VI"))
    {
    	hw = "ASUS6020VI";
	os = "LINUX_24";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	
	/*  RG Priority 2  */
 	enable_module("MODULE_RG_PPTP");
  	enable_module("MODULE_RG_L2TP");
	
	/*  RG Priority 3  */
  	enable_module("MODULE_RG_VLAN");

	/*  RG Priority 4  */
	/* Not enough RAM on board for IPSEC
	enable_module("MODULE_RG_IPSEC");
	*/
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	/*Not enough space on flash 
	enable_module("MODULE_RG_JVM");
	*/

	/*  RG Priority 7  */
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	/*Not enough space on flash
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	*/
	
	/* OpenRG HW support */
	
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	token_set_y("CONFIG_ULIBC_SHARED");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm1", NULL);
    }
    else if (IS_DIST("ASUS6020VI_26"))
    {
    	hw = "ASUS6020VI";
	os = "LINUX_26";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB");

	token_set_y("CONFIG_BOOTLDR_CFE");
	token_set_y("CONFIG_BOOTLDR_CFE_DUAL_IMAGE");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
    	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
    	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
    	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_PSE");
	
	/*  RG Priority 2  */
	/* Not enough space on flash 
 	enable_module("MODULE_RG_PPTP");
  	enable_module("MODULE_RG_L2TP");
	*/
	
	/*  RG Priority 3  */
	/* Not enough space on flash 
  	enable_module("MODULE_RG_VLAN");
	*/

	/*  RG Priority 4  */
	/* Not enough RAM on board for IPSEC
	enable_module("MODULE_RG_IPSEC");
	*/
	/* Not enough space on flash 
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_JVM");
	*/

	/*  RG Priority 7  */
	/* Not enough space on flash
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	*/
	
	/* OpenRG HW support */
	
	token_set_y("CONFIG_HW_DSL_WAN");
	/*
	token_set_y("CONFIG_HW_SWITCH_LAN");
	*/
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");

	/* B40399: HW led/button support is required for ASUS kernel 2.6
	token_set_m("CONFIG_HW_BUTTONS");
	token_set_y("CONFIG_HW_LEDS");
	*/

	token_set_y("CONFIG_ULIBC_SHARED");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm1", NULL);
    }
    else if (IS_DIST("RGLOADER_WADB102GB"))
    {
    	hw = "WADB102GB";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");
	token_set_y("CONFIG_RG_RGLOADER");
	token_set_m("CONFIG_RG_KRGLDR");
	token_set_y("CONFIG_RG_TELNETS");
	
	token_set_y("CONFIG_RG_MTD");

	token_set("CONFIG_SDRAM_SIZE", "16");
	token_set_y("CONFIG_BCM963XX_BOOTSTRAP");

	/* OpenRG HW support */
	
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("WADB102GB"))
    {
    	hw = "WADB102GB";
	os = "LINUX_24";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");
	
	small_flash_default_dist();

	/* OpenRG HW support */

	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	enable_module("CONFIG_HW_80211G_BCM43XX");
	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm1", NULL);
    }
    else if (IS_DIST("MPC8272ADS_LSP") || IS_DIST("MPC8272ADS_LSP_26"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	/* Hardware */
	
	hw = "MPC8272ADS";
	token_set_y("CONFIG_HW_ETH_FEC");

	if (IS_DIST("MPC8272ADS_LSP_26"))
	    os = "LINUX_26";
	else
	    os = "LINUX_24";
	
	/* Software */

	token_set_y("CONFIG_LSP_DIST");
    }
    else if (IS_DIST("MPC8272ADS"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	hw = "MPC8272ADS";
	os = "LINUX_26";

	token_set_y("CONFIG_RG_SMB");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_SSL_VPN");
        enable_module("MODULE_RG_FULL_PBX"); 
	enable_module("MODULE_RG_PSE");

	/*  SMB Priority 2  */
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");

	/*  SMB Priority 3  */
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	/*Not enough space on flash
	enable_module("MODULE_RG_BLUETOOTH");
	*/

	/*  SMB Priority 4  */
	/*Not enough space on flash
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");	
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	*/

	/*  SMB Priority 5  */
	/*Not enough space on flash
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
        */
	
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	/* Include automatic daylight saving time calculation */
	
	token_set_y("CONFIG_RG_DATE");
	token_set_y("CONFIG_RG_TZ_FULL");
	token_set("CONFIG_RG_TZ_YEARS", "5");

	/* HW Configuration Section */
	
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");

	token_set_y("CONFIG_HW_ETH_FEC");

	enable_module("CONFIG_HW_USB_STORAGE");

	/* Ralink RT2560 */

	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");

	enable_module("CONFIG_HW_ENCRYPTION");
	
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth1", NULL);

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
    }
    else if (IS_DIST("EP8248_LSP_26"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	/* Hardware */
	
	hw = "EP8248";
	os = "LINUX_26";
	
	token_set_y("CONFIG_HW_ETH_FEC");

	/* Software */

	token_set_y("CONFIG_LSP_DIST");
    }
    else if (IS_DIST("EP8248_26"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	hw = "EP8248";
	os = "LINUX_26";
	
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_URL_FILTERING");
#if 0
	if (!IS_DIST("MPC8272ADS_26")) /* Temporary */
	{
	    enable_module("MODULE_RG_MAIL_SERVER");
	    enable_module("MODULE_RG_WEB_SERVER");
	    enable_module("MODULE_RG_FTP_SERVER");

	    enable_module("MODULE_RG_PRINTSERVER");
	}

	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
#endif	
	token_set_y("CONFIG_RG_NETTOOLS_ARP");

	/* Include automatic daylight saving time calculation */
	
	token_set_y("CONFIG_RG_DATE");
	token_set_y("CONFIG_RG_TZ_FULL");
	token_set("CONFIG_RG_TZ_YEARS", "5");

	/* HW Configuration Section */
	
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");

	token_set_y("CONFIG_HW_ETH_FEC");

//	enable_module("CONFIG_HW_USB_STORAGE");

	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);
    }
    else if (IS_DIST("MPC8349ITX_LSP"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	/* Hardware */
	
	os = "LINUX_26";
	hw = "MPC8349ITX";

	/* Software */
	token_set_y("CONFIG_GIANFAR");
	token_set_y("CONFIG_CICADA_PHY");
	token_set_y("CONFIG_PHYLIB");
	token_set_y("CONFIG_LSP_DIST");

	token_set_y("CONFIG_USB_GADGET");
	token_set_y("CONFIG_USB_ETH");
	token_set_y("CONFIG_USB_MPC");
	token_set_y("CONFIG_USB_GADGET_MPC");
	token_set_y("CONFIG_USB_GADGET_DUALSPEED");
	token_set_y("CONFIG_USB_ETH_RNDIS");

	token_set_y("CONFIG_SCSI");
	token_set_y("CONFIG_SCSI_SATA");
	token_set_y("CONFIG_SCSI_SATA_SIL");

	token_set_y("CONFIG_USB");
	token_set_y("CONFIG_USB_EHCI_HCD");
	token_set_y("CONFIG_USB_EHCI_ROOT_HUB_TT");
	token_set_y("CONFIG_USB_DEVICEFS");
	token_set_y("CONFIG_FSL_USB20");
	token_set_y("CONFIG_MPH_USB_SUPPORT");
	token_set_y("CONFIG_MPH0_USB_ENABLE");
	token_set_y("CONFIG_MPH0_ULPI");
    }
    else if (IS_DIST("MPC8349ITX"))
    {
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_PPC");

	os = "LINUX_26";
	hw = "MPC8349ITX";

	token_set_y("CONFIG_RG_SMB");

	/*  SMB Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_WEB_SERVER");
	enable_module("MODULE_RG_SSL_VPN");
        enable_module("MODULE_RG_FULL_PBX"); 
	enable_module("MODULE_RG_PSE");

	/*  SMB Priority 2  */
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	/*Not enough space on flash
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	*/

	/*  SMB Priority 3  */
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	/*Not enough space on flash
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_RADIUS_SERVER");
	enable_module("MODULE_RG_BLUETOOTH");
	*/
	
	
	/*  SMB Priority 4  */
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	/*Not enough space on flash
	enable_module("MODULE_RG_FTP_SERVER");
	enable_module("MODULE_RG_SNMP");	
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_MAIL_SERVER");
	enable_module("MODULE_RG_JVM");
	*/
	
	/*  SMB Priority 5  */
	/*Not enough space on flash
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	*/

	token_set_y("CONFIG_RG_NETTOOLS_ARP");

	/* Include automatic daylight saving time calculation */
	token_set_y("CONFIG_RG_DATE");
	token_set_y("CONFIG_RG_TZ_FULL");
	token_set("CONFIG_RG_TZ_YEARS", "5");

	/* HW Configuration Section */
	/* Remove RNDIS until  B38111 is resolved  
	token_set_y("CONFIG_HW_USB_ETH");
	*/
	token_set_y("CONFIG_HW_IDE");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_USB_STORAGE");
	if (token_get("MODULE_RG_IPSEC"))
	    enable_module("CONFIG_HW_ENCRYPTION");

	dev_add_bridge("br0", DEV_IF_NET_INT, "eth1", NULL);

	/* Don't use this feature until B53229 is fixed
	 * token_set_y("CONFIG_RG_SKB_CACHE");
	 */
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
    }
    else if (IS_DIST("CENTAUR_LSP"))
    {
	hw = "CENTAUR";
 	os = "LINUX_24";
 
 	token_set_y("CONFIG_KS8695_COMMON");
 
 	/* Software */
 	token_set_y("CONFIG_LSP_DIST");
 	token_set_y("CONFIG_SIMPLE_RAMDISK");
 	token_set_y("CONFIG_LSP_FLASH_LAYOUT");
 	token_set("CONFIG_RAMDISK_SIZE", "1024");
 	token_set_y("CONFIG_HW_ETH_WAN");
 	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("CENTAUR") || IS_DIST("CENTAUR_VGW"))
    {
 	hw = "CENTAUR";
 	os = "LINUX_24";
 
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_KS8695");
	enable_module("MODULE_RG_FOUNDATION");
 	enable_module("MODULE_RG_ADVANCED_ROUTING");
 	enable_module("MODULE_RG_VLAN");
 	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
   	enable_module("MODULE_RG_PPP");
 	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
 	enable_module("MODULE_RG_URL_FILTERING");
 	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
 	enable_module("MODULE_RG_SNMP");
 	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
 
 	/* HW Configuration Section */
 	token_set_y("CONFIG_HW_ETH_WAN");
 	token_set_y("CONFIG_HW_ETH_LAN");
 	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
 
 	token_set("CONFIG_RG_SSID_NAME", "Centaur");
 	dev_add_bridge("br0", DEV_IF_NET_INT, "eth1", NULL);

	if (IS_DIST("CENTAUR_VGW"))
	{
	    enable_module("MODULE_RG_VOIP_OSIP");
	    enable_module("MODULE_RG_ATA");
	    enable_module("CONFIG_HW_DSP");
	}
 
 	/* Software */
 	token_set_y("CONFIG_KS8695_COMMON");
    }
    else if (IS_DIST("RGLOADER_CENTAUR"))
    {
 	hw = "CENTAUR";
 	os = "LINUX_24";
 
 	token_set_y("CONFIG_RG_RGLOADER");
 
 	token_set_y("CONFIG_KS8695_COMMON");
 	token_set_y("CONFIG_SIMPLE_RAMDISK");
 	token_set("CONFIG_RAMDISK_SIZE", "4096");
 	token_set_y("CONFIG_PROC_FS");
 	token_set_y("CONFIG_EXT2_FS");
 	token_set_y("CONFIG_HW_ETH_WAN");
 	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("AD6834"))
    {
	hw = "AD6834";

	/* Modules */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_PPP");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	token_set_y("CONFIG_RG_PERM_STORAGE_VENDOR_HEADER");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_DYN_LINK");

	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("CONFIG_HW_DSP");
	enable_module("MODULE_RG_HOME_PBX");

	/* OpenRG HW support */
 	enable_module("CONFIG_HW_80211G_RALINK_RT2560");
	token_set_y("CONFIG_RG_VAP_SECURED");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_DSL_WAN");
        enable_module("CONFIG_HW_USB_RNDIS");
        enable_module("CONFIG_HW_ENCRYPTION");
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_LX4189_ADI");
    }
    else if (IS_DIST("AD6834_26") || IS_DIST("AD6834_26_FULL"))
    {
	hw = "AD6834";
	os = "LINUX_26";

	/* Modules */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_PPP");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	
	token_set_y("CONFIG_RG_PERM_STORAGE_VENDOR_HEADER");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");
	
	token_set_y("CONFIG_DYN_LINK");

	enable_module("MODULE_RG_BLUETOOTH");

	/* OpenRG HW support */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_DSL_WAN");
        enable_module("CONFIG_HW_ENCRYPTION");
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_LX4189_ADI");

	/* Ramdisk */
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }
    else if (IS_DIST("AD6834_26_LSP"))
    {
	hw = "AD6834";
	os = "LINUX_26";
	token_set_y("CONFIG_DYN_LINK");

	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_RG_PERM_STORAGE");

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");

	/* Ramdisk */
	//token_set_y("CONFIG_SIMPLE_RAMDISK");
	token_set_y("CONFIG_LSP_FLASH_LAYOUT");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	//token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_SIMPLE_RAMDISK");

    }
    else if (IS_DIST("ALASKA") || IS_DIST("HIMALAYA2"))
    {
	hw = IS_DIST("ALASKA") ? "ALASKA" : "HIMALAYA2";
	os = "LINUX_26";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_LX4189");

	token_set_y("CONFIG_BOOTLDR_UBOOT");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
        /* Some features were removed. They should be returned after
	 * mainfs_in_flash will work. See B57442 */

	/* RG Priority 1 */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_PSE");
        /* Disable this feature to free memory space and reduce CPU load.
	 * enable_module("MODULE_RG_REDUCE_SUPPORT");
	 */
	enable_module("MODULE_RG_DSL");

	/*  RG Priority 2  */
	//enable_module("MODULE_RG_FILESERVER");
	//enable_module("MODULE_RG_IPSEC");

	/*  RG Priority 3  */
	//enable_module("MODULE_RG_DSLHOME");
	//enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");

	/*  RG Priority 4  */
	//enable_module("MODULE_RG_FTP_SERVER");
	//enable_module("MODULE_RG_MAIL_SERVER");
	//enable_module("MODULE_RG_JVM"); /* doesn't work see B45361 */

	/* General */
	token_set_y("CONFIG_DYN_LINK");

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	
	if (IS_DIST("ALASKA"))
	    token_set_y("CONFIG_HW_VDSL_WAN");
	else
	{
	    token_set_y("CONFIG_HW_DSL_WAN");
	    token_set_y("CONFIG_HW_ETH_LAN2");
	}

	/* this enslaves all LAN devices, if no other device is enslaved
	 * explicitly */
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	/* Ramdisk */
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	if (IS_DIST("ALASKA"))
	    token_set_y("CONFIG_HW_LEDS");

	token_set_m("CONFIG_RG_KLOG");
	
	/* Fast Path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

       /* HW QoS */
       if (token_get("MODULE_RG_QOS"))
       {
	   token_set_y("CONFIG_RG_HW_QOS");
	   token_set_m("CONFIG_RG_HW_QOS_PLAT_IKANOS");
       }

       /* build options */
       token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
       token_set("LIBC_IN_TOOLCHAIN", "n");

       /* Additional LAN Interface: Wireless LAN, IEEE 802.11a/b/g (Atheros) */
       enable_module("CONFIG_RG_ATHEROS_HW_AR5212");

       /* USB */
       enable_module("CONFIG_HW_USB_HOST_EHCI");
       enable_module("CONFIG_HW_USB_HOST_OHCI");
       enable_module("CONFIG_HW_USB_STORAGE");
       //enable_module("CONFIG_HW_USB_RNDIS"); /* doesn't work */

       /* DSP */
       enable_module("CONFIG_HW_DSP");

       /* VOIP */
       enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
       enable_module("MODULE_RG_HOME_PBX");
    }
    else if (IS_DIST("ALASKA_LSP") || IS_DIST("CAMEROON_LSP"))
    {
	hw = IS_DIST("ALASKA_LSP") ? "ALASKA" : "CAMEROON";
	os = "LINUX_26";
	
	token_set_y("CONFIG_DYN_LINK");
	token_set_y("CONFIG_LSP_DIST");
	token_set_y("CONFIG_KALLSYMS");	

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
#if 0
	/* Not tested yet... */
	token_set_y("CONFIG_HW_VDSL_WAN");
#endif

	/* Ramdisk */
	token_set_y("CONFIG_LSP_FLASH_LAYOUT");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_SIMPLE_RAMDISK");

	/* VoIP */
	enable_module("CONFIG_HW_DSP");
	token_set_y("CONFIG_RG_VOIP_DEMO");
    }
    else if (IS_DIST("CAMEROON") || IS_DIST("CAMEROON-IFE6-VDSL") ||
	IS_DIST("CAMEROON-IFE6-ADSL"))
    {
	hw = IS_DIST("CAMEROON") ? "CAMEROON" : "CAMEROON-IFE6";
	os = "LINUX_26";

	token_set_y("CONFIG_BOOTLDR_UBOOT");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

        /* Board */
        if (IS_DIST("CAMEROON"))
            token_set_y("CONFIG_FUSIV_VX180_IFE5");
        else 
            token_set_y("CONFIG_FUSIV_VX180_IFE6");

	if (IS_DIST("CAMEROON-IFE6-VDSL"))
	{
	    token_set_y("CONFIG_FUSIV_VX180_IFE6_VDSL");
	    token_set_y("CONFIG_HW_VDSL_WAN");
	}
	else if (IS_DIST("CAMEROON-IFE6-ADSL"))
	{
	    token_set_y("CONFIG_FUSIV_VX180_IFE6_ADSL");    
	    enable_module("MODULE_RG_DSL");
	    token_set_y("CONFIG_HW_DSL_WAN");
	}
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB_IKANOS");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");

	/*  RG Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FILESERVER");

	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
        enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	
	/* General */
	token_set_y("CONFIG_DYN_LINK");

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_LAN2");

	/* this enslaves all LAN devices, if no other device is enslaved
	 * explicitly */
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	/* Fast Path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

	/* HW QoS */
       if (token_get("MODULE_RG_QOS"))
       {
	   token_set_y("CONFIG_RG_HW_QOS");
	   token_set_m("CONFIG_RG_HW_QOS_PLAT_IKANOS");
       }

	/* build options */
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_HW_LEDS");

	token_set_m("CONFIG_RG_KLOG");

	/* Additional LAN Interface: Wireless LAN, IEEE 802.11a/b/g (Atheros) */
	enable_module("CONFIG_RG_ATHEROS_HW_AR5416");

	/* USB */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_HOST_OHCI");
	enable_module("CONFIG_HW_USB_STORAGE");

	/* DSP */
	enable_module("CONFIG_HW_DSP");
    }
    else if (IS_DIST("JNET_SERVER"))
    {
	set_jnet_server_configs();
	token_set_y("CONFIG_DEBIAN_DPKG_VER_CHECK");
	token_set_y("CONFIG_RG_USE_LOCAL_TOOLCHAIN");
	token_set_y("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY");
	token_set_y("CONFIG_RG_ZLIB");
	token_set_y("CONFIG_LOCAL_EVENT_EPOLL");
	token_set("CONFIG_RG_LANGUAGES", "DEF fr ru es ko zh_TW ja de it he "
	    "zh_CN");
    }
    else if (IS_DIST("VAS_PORTAL"))
    {
	set_vas_configs();
	token_set_y("CONFIG_RG_USE_LOCAL_TOOLCHAIN");
	token_set_y("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY");
	token_set("CONFIG_RG_LANGUAGES", "DEF fr ru es ko zh_TW ja de it he "
	    "zh_CN");
    }
    else if (IS_DIST("HOSTTOOLS"))
    {
	set_hosttools_configs();
	/* we want to use objects from JPKG_UML (local_*.o.i386-jungo-linux-gnu)
	 * despite the fact that we use the host local compiler */
	token_set_y("CONFIG_RG_USE_LOCAL_TOOLCHAIN");
	token_set("I386_TARGET_MACHINE", "i386-jungo-linux-gnu");
	token_set("TARGET_MACHINE", token_get_str("I386_TARGET_MACHINE"));

	token_set_y("CONFIG_GLIBC");
	token_set_y("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_UML");
    }
    else if (IS_DIST("MAKELICRG"))
    {
	token_set_y("CONFIG_RG_USE_LOCAL_TOOLCHAIN");
	token_set_y("CONFIG_GLIBC");
	token_set_y("CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY");
    }
    else if (IS_DIST("BCM93380_LSP"))
    {
	os = "LINUX_26";
	hw = "BCM93380";

	token_set_y("CONFIG_LSP_DIST");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
    }
    else if (IS_DIST("BCM93383_LSP"))
    {
	os = "LINUX_26";
	hw = "BCM93383";

	token_set_y("CONFIG_LSP_DIST");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
    }
    else if (IS_DIST("BCM93383"))
    {
	os = "LINUX_26";
	hw = "BCM93383";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB_BCM9338X");

	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set("CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION", "linux");

    	enable_module("MODULE_RG_FOUNDATION");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_80211N_BCM338X");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_SNMP_CLIENT");
	enable_module("MODULE_RG_DSLHOME");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_IPV6");
#if 0
	enable_module("CONFIG_RG_WPS");
#endif
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_PSE");
    }
    else if (IS_DIST("BCM96362_LSP"))
    {
	os = "LINUX_26";
	hw = "BCM96362";

	token_set_y("CONFIG_LSP_DIST");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_DSL_WAN");
    }
    else if (IS_DIST("BCM96362"))
    {
	os = "LINUX_26";
	hw = "BCM96362";

	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");	

    	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_VLAN");	
	enable_module("MODULE_RG_DSL");	
	enable_module("MODULE_RG_PSE");

        enable_module("CONFIG_HW_USB_STORAGE");
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_HOST_OHCI");

	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_DSL_WAN");

	/* VOIP */
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("CONFIG_HW_DSP");

	token_set_y("CONFIG_RG_PERM_STORAGE_UBI");
	token_set("CONFIG_RG_UBI_PARTITION_SIZE", "0x02800000");

        token_set_y("CONFIG_RG_HW_WATCHDOG");
    }
    else if (IS_DIST("RGLOADER_BCM96362"))
    {        
	os = "LINUX_26";
	hw = "BCM96362";
        
	token_set_y("CONFIG_RG_RGLOADER");
        token_set_m("CONFIG_RG_KRGLDR");

 	token_set_y("CONFIG_PROC_FS");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	token_set_y("CONFIG_RG_PERM_STORAGE_UBI");
	token_set("CONFIG_SDRAM_SIZE", "32");
	token_set_y("CONFIG_BRCM_MEMORY_RESTRICTION_32M");

	token_set("CONFIG_RG_CRAMFS_COMP_METHOD", "none");
    }
    else if (IS_DIST("BCM96358_LSP"))
    {
	hw = "BCM96358";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");

	/* Devices */
	/* XXX: Add all devices when drivers are ready.
	 */
	token_set_y("CONFIG_HW_ETH_LAN");
	
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
    }
    /* Two bcm96358 boards exist, bcm96358GW and bcm96358M, for now their
     * features are the same, but in the future if different features will be 
     * implemented, must configure two dists */
    else if (IS_DIST("BCM96358"))
    {
	hw = "BCM96358";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB");
	
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");

	/* Small flash */
	token_set("CONFIG_RG_STANDARD_SIZE_CONF_SECTION", "n");
	token_set("CONFIG_RG_STANDARD_SIZE_IMAGE_SECTION", "n");

	/*  RG Priority 1  */
    	enable_module("MODULE_RG_FOUNDATION");
    	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
        enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_SSL_VPN");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	if (token_get("CONFIG_RG_NETWORK_BOOT_IMAGE"))
	{
	    enable_module("CONFIG_HW_DSP");
	    enable_module("MODULE_RG_HOME_PBX");
	    enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	    enable_module("MODULE_RG_WEB_SERVER");
	    enable_module("MODULE_RG_FILESERVER");
	    enable_module("MODULE_RG_ACCESS_DLNA");
	    enable_module("MODULE_RG_MEDIA_SERVER");
	}
	enable_module("MODULE_RG_PSE");

	/*  RG Priority 2  */
	if (token_get("CONFIG_RG_NETWORK_BOOT_IMAGE"))
	{
	    enable_module("MODULE_RG_L2TP");
	    enable_module("MODULE_RG_PPTP");
	    enable_module("MODULE_RG_PRINTSERVER");
	}
	token_set_y("CONFIG_RG_PSE_SW_DRIVER_GIVE_UNALIGNED_IP");

	/*  RG Priority 3  */
	/*Not enough space on flash
	 * XXX: wrap with CONFIG_RG_NETWORK_BOOT_IMAGE
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_BLUETOOTH");
        */

	/*  RG Priority 4  */
	/*Not enough space on flash
	 * XXX: wrap with CONFIG_RG_NETWORK_BOOT_IMAGE
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_VOIP_ASTERISK_H323");	
	enable_module("MODULE_RG_VOIP_ASTERISK_MGCP_CALL_AGENT");
	enable_module("MODULE_RG_MAIL_SERVER");	
	enable_module("MODULE_RG_JVM");
	enable_module("MODULE_RG_FTP_SERVER");
        */	
	
	/*  RG Priority 7  */
	/*Not enough space on flash
	 * XXX: wrap with CONFIG_RG_NETWORK_BOOT_IMAGE
	enable_module("MODULE_RG_ROUTE_MULTIWAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
        */
    
    	token_set_y("CONFIG_ULIBC_SHARED");
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_SWITCH_LAN");
	token_set_y("CONFIG_HW_DSL_WAN");
	token_set_y("CONFIG_RG_NETTOOLS_ARP");

	if (token_get("CONFIG_RG_NETWORK_BOOT_IMAGE"))
	{
	    /* USB Host */
	    enable_module("CONFIG_HW_USB_HOST_EHCI");
	    enable_module("CONFIG_HW_USB_HOST_OHCI");
	    enable_module("CONFIG_HW_USB_STORAGE");
	}

	enable_module("CONFIG_HW_80211G_BCM43XX");
      	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm1", NULL);

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	/* USB device */
	/* RNDIS is disabled due to lack of flash.
	 * XXX: wrap with CONFIG_RG_NETWORK_BOOT_IMAGE
	enable_module("CONFIG_HW_USB_RNDIS");
	*/
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");
    }
    else if (IS_DIST("DWV_96358") || IS_DIST("HH1620") || IS_DIST("CT6382D"))
    {
	if (IS_DIST("HH1620"))
	    hw = "HH1620";
	else if (IS_DIST("DWV_96358"))
	    hw = "DWV_BCM96358";
	else if (IS_DIST("CT6382D"))
	    hw = "CT6382D";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEB");

	token_set_y("CONFIG_BOOTLDR_CFE");
	if (!IS_DIST("CT6382D"))
	    token_set_y("CONFIG_BOOTLDR_CFE_DUAL_IMAGE");
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_VAS_CLIENT");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	/* Additional LAN Interface - See below */
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
        enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN"); /* WLAN */
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");

	/* Additional LAN Interface */
	enable_module("CONFIG_HW_USB_RNDIS"); /* USB Slave */

	/* VOIP */
	enable_module("MODULE_RG_HOME_PBX");
	token_set_y("CONFIG_RG_VOIP_CALL_LOG");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("CONFIG_HW_DSP");
	if (IS_DIST("DWV_96358") || IS_DIST("HH1620"))
	    token_set_y("CONFIG_HW_FXO");

	/* WLAN */
	if (!token_get("CONFIG_RG_ATHEROS_HW_AR5416") &&
	    !token_get("CONFIG_RG_ATHEROS_HW_AR5212"))
	{
	    enable_module("CONFIG_HW_80211G_BCM43XX");
	}
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");

	/* HW switch */
	token_set_y("CONFIG_HW_SWITCH_LAN");
	/* DSL */
	if (token_get("MODULE_RG_DSL"))
	    token_set_y("CONFIG_HW_DSL_WAN");
	/* USB Host */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_HOST_OHCI");
	enable_module("CONFIG_HW_USB_STORAGE");	
	
	dev_add_bridge("br0", DEV_IF_NET_INT, NULL);
	dev_add_to_bridge_if_opt("br0", "wl0", "CONFIG_HW_80211G_BCM43XX");
	dev_add_to_bridge_if_opt("br0", "usb0", "CONFIG_HW_USB_RNDIS");

	if (token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	{
	    token_set_y("CONFIG_HW_ETH_WAN");
	    switch_virtual_port_dev_add("eth0", "bcm1_main", 3, DEV_IF_NET_EXT);
	    dev_add_to_bridge("br0", "bcm1_main");
	}
	else
	    dev_add_to_bridge("br0", "bcm1");


	/* build */
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");
	token_set_y("CONFIG_RG_MAINFS_IN_PHYS_MAPPED_FLASH");
        
	token_set_y("CONFIG_HW_BUTTONS");

	token_set_m("CONFIG_RG_KLOG"); /* Availale only for the 8MB flash */

	if (IS_DIST("DWV_96358"))
	    token_set_y("CONFIG_HW_LEDS");
#if 0
	/* B37386: Footprint Reduction cause crashes in our dist... */
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
#endif
    }
    else if (IS_DIST("DWV_96348"))
    {
    	hw = "DWV_BCM96348";
	os = "LINUX_26";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_BCM9634X");
	token_set_y("CONFIG_HW_DWV_96348");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");

	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_TR_064");

	/* Additional LAN Interface */
	enable_module("CONFIG_HW_USB_RNDIS"); /* USB Slave */

	/* WLAN */
	enable_module("CONFIG_HW_80211G_BCM43XX");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");

	/* HW switch */
#if 0
	/* B38935: HW switch does not work */
	token_set_y("CONFIG_HW_SWITCH_LAN");
#else
	token_set_y("CONFIG_HW_ETH_LAN");
#endif

	/* ETH WAN. TODO: Set as DMZ? */
	token_set_y("CONFIG_HW_ETH_WAN");

	/* DSL */
	enable_module("MODULE_RG_DSL");
	token_set_y("CONFIG_HW_DSL_WAN");

	/* USB Host */
#if 0
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_HOST_OHCI");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("CONFIG_HW_USB_STORAGE");   
#endif

	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "bcm1", "wl0", "usb0", NULL);

	/* build */
	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");
    }
    else if (IS_DIST("RGLOADER_UML"))
    {
	hw = "UML";
	os = "LINUX_26";

	token_set_y("CONFIG_RG_RGLOADER");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_RG_TELNETS");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_UML");
    }
    else if (IS_DIST("JPKG_SRC"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"src\"");
	token_set_y("CONFIG_RG_JPKG_SRC");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_UML"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"uml\"");
        token_set_y("CONFIG_RG_JPKG_UML");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV5B"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv5b\"");
	token_set_y("CONFIG_RG_JPKG_ARMV5B");
	    
	token_set_y("CONFIG_GLIBC");
	token_set_y("GLIBC_IN_TOOLCHAIN");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV5L"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv5l\"");
        token_set_y("CONFIG_RG_JPKG_ARMV5L");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV4L"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv4l\"");
        token_set_y("CONFIG_RG_JPKG_ARMV4L");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV6J"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv6j\"");
        token_set_y("CONFIG_RG_JPKG_ARMV6J");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV6_COMCERTO"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv6-comcerto\"");
        token_set_y("CONFIG_RG_JPKG_ARMV6_COMCERTO");

	set_jpkg_dist_configs("JPKG_ARMV6_COMCERTO");
    }
    else if (IS_DIST("JPKG_KS8695"))
    {
	hw = "JPKG";
	token_set("JPKG_ARCH", "\"ks8695\"");
	token_set_y("CONFIG_RG_JPKG_KS8695");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_PPC"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"ppc\"");
        token_set_y("CONFIG_RG_JPKG_PPC");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEB");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEL"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipsel\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEL");

	set_jpkg_dist_configs(dist);
    }    
    else if (IS_DIST("JPKG_BCM9634X"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"bcm9634x\"");
        token_set_y("CONFIG_RG_JPKG_BCM9634X");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_SB1250"))
    {
	hw = "JPKG";
	token_set("JPKG_ARCH", "\"sb1250\"");

	token_set_y("CONFIG_RG_JPKG_SB1250");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_OCTEON"))
    {
	hw = "JPKG";
	token_set("JPKG_ARCH", "\"octeon\"");

	token_set_y("CONFIG_RG_JPKG_OCTEON");

	set_jpkg_dist_configs(dist);
    }    
    else if (IS_DIST("JPKG_ARM_920T_LE"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"arm-920t-le\"");
        token_set_y("CONFIG_RG_JPKG_ARM_920T_LE");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB_INFINEON"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb-infineon\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEB_INFINEON");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB_XWAY"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb-xway\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEB_XWAY");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_LX4189_ADI"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"lx4189-adi\"");
	token_set_y("CONFIG_RG_JPKG_LX4189");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_LX4189"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"lx4189\"");
	token_set_y("CONFIG_RG_JPKG_LX4189");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB_IKANOS"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb-ikanos\"");
	token_set_y("CONFIG_RG_JPKG_MIPSEB_IKANOS");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB_BCM9636X"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb-bcm9636x\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEB_BCM9636X");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_MIPSEB_BCM9338X"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"mipseb-bcm9338x\"");
        token_set_y("CONFIG_RG_JPKG_MIPSEB_BCM9338X");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_ARMV5L_FEROCEON"))
    {
	hw = "JPKG";
        token_set("JPKG_ARCH", "\"armv5l-feroceon\"");
	token_set_y("CONFIG_RG_JPKG_ARMV5L_FEROCEON");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("JPKG_LOCAL_I386"))
    {
	hw = NULL;
        token_set("JPKG_ARCH", "\"local-i386\"");
	token_set_y("CONFIG_RG_JPKG_LOCAL_I386");

	set_jpkg_dist_configs(dist);
    }
    else if (IS_DIST("UML_LSP"))
    {
	hw = "UML";
	os = "LINUX_24";

	token_set_y("CONFIG_RG_UML");

	token_set_y("CONFIG_LSP_DIST");
    }
    else if (IS_DIST("UML_LSP_26"))
    {
	hw = "UML";
	os = "LINUX_26";

	token_set_y("CONFIG_RG_UML");
	
	token_set_y("CONFIG_GLIBC");

	token_set_y("CONFIG_LSP_DIST");
    }
    else if (IS_DIST("UML") || IS_DIST("UML_GLIBC") || IS_DIST("UML_DUAL_WAN")
	|| IS_DIST("UML_ATM") || IS_DIST("UML_26") || IS_DIST("UML_VALGRIND")
	|| IS_DIST("UML_OPENRG"))
    {
	hw = "UML";

	if (IS_DIST("UML_26") || IS_DIST("UML_VALGRIND")
	    || IS_DIST("UML_OPENRG"))
	{
	    os = "LINUX_26";
	}

	if (IS_DIST("UML_GLIBC"))
	{
	    token_set_y("CONFIG_GLIBC");
	    token_set_y("GLIBC_IN_TOOLCHAIN");
	}

	if (IS_DIST("UML_VALGRIND"))
	    token_set_y("CONFIG_VALGRIND");

	if (IS_DIST("UML_OPENRG"))
	    token_set_y("CONFIG_RG_INTEGRAL");
	else
	{
	    token_set_y("CONFIG_RG_SMB");
	    token_set_y("CONFIG_RG_SPEED_TEST");
	    enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	    enable_module("MODULE_RG_IPSEC");
	    enable_module("MODULE_RG_PPTP");
	    enable_module("MODULE_RG_SNMP");
	    enable_module("MODULE_RG_IPV6");
	    /* XXX Removed until compilation with IPv6 is fixed */
	    if (!token_get("MODULE_RG_IPV6"))
	    {
		enable_module("MODULE_RG_IPV6_OVER_IPV4_TUN");
		enable_module("MODULE_RG_BLUETOOTH");
	    }
	    enable_module("MODULE_RG_ADVANCED_ROUTING");
	    enable_module("MODULE_RG_L2TP");
	    enable_module("MODULE_RG_URL_FILTERING");
	    enable_module("MODULE_RG_ROUTE_MULTIWAN");
	    enable_module("MODULE_RG_JVM");
	    enable_module("MODULE_RG_WEB_SERVER");
	    enable_module("MODULE_RG_PRINTSERVER");
	    enable_module("MODULE_RG_MAIL_SERVER");
	    enable_module("MODULE_RG_FTP_SERVER");
	    enable_module("MODULE_RG_SSL_VPN");
	    enable_module("MODULE_RG_ANTIVIRUS_NAC");
	    enable_module("MODULE_RG_ANTIVIRUS_LAN_PROXY");
	    enable_module("MODULE_RG_RADIUS_SERVER");
	}

	enable_module("MODULE_RG_PSE");

	/* ALL OpenRG Available Modules - ALLMODS */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	/* XXX Removed until compilation with IPv6 is fixed */
	if (!token_get("MODULE_RG_IPV6"))
	    enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
        enable_module("CONFIG_HW_80211G_UML_WLAN");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_UML_LOOP_STORAGE"); /* UML Disk Emulation */
	token_set_y("CONFIG_HW_ETH_WAN");
	enable_module("CONFIG_HW_DSP");
	if (IS_DIST("UML_DUAL_WAN"))
	    token_set_y("CONFIG_HW_ETH_WAN2");
	if (IS_DIST("UML_ATM"))
	{
	    enable_module("MODULE_RG_DSL");
	    token_set_y("CONFIG_HW_DSL_WAN");
	}
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_LAN2");
	
	/* When B50873 is fixed then the if (MODULE_RG_VLAN) should be
	 * removed.*/
	if (token_get("MODULE_RG_VLAN"))
	    token_set_y("CONFIG_HW_SWITCH_LAN");
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_UML");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");
	token_set_y("CONFIG_RG_MT_PROFILING_FULL_INFO");
	token_set_y("CONFIG_RG_SSL_ROOT_CERTIFICATES");

	/* All devices will be added according to the CONFIGS in HW config.
	 * Note: We assume that we have a lan bridge and at least one device
	 * will be enslaved to it...
	 */
	dev_add("br0", DEV_IF_BRIDGE, DEV_IF_NET_INT);
	token_set_y("CONFIG_DEF_BRIDGE_LANS");
    }
    else if (IS_DIST("UML_IPPHONE"))
    {
	hw = "UML";

	/* ALL OpenRG Available Modules - ALLMODS */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_SNMP");
	/* For Customer distributions only:
	 * When removing IPV6 you must replace in feature_config.c the line 
	 * if(token_get("MODULE_RG_IPV6")) with if(1) */
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_VOIP_RV_SIP");
	enable_module("MODULE_RG_VOIP_RV_MGCP");
	enable_module("MODULE_RG_VOIP_RV_H323");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_DSP");

	/* VoIP */
	token_set_y("CONFIG_RG_IPPHONE");
    }
    else if (IS_DIST("UML_IPPHONE_VALGRIND"))
    {
	hw = "UML";

	token_set_y("CONFIG_RG_SMB");

	/* ALL OpenRG Available Modules - ALLMODS */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_SNMP");
	/* For Customer distributions only:
	 * When removing IPV6 you must replace in feature_config.c the line 
	 * if(token_get("MODULE_RG_IPV6")) with if(1) */
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_VOIP_RV_SIP");
	enable_module("MODULE_RG_VOIP_RV_MGCP");
	enable_module("MODULE_RG_VOIP_RV_H323");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_DSP");

	token_set_y("CONFIG_VALGRIND");

	/* VoIP */
	token_set_y("CONFIG_RG_IPPHONE");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");
    }
    else if (IS_DIST("UML_ATA_OSIP"))
    {
	hw = "UML";

	token_set_y("CONFIG_RG_SMB");
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_UML");
        
	/* ALL OpenRG Available Modules - ALLMODS */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_SNMP");
	/* For Customer distributions only:
	 * When removing IPV6 you must replace in feature_config.c the line 
	 * if(token_get("MODULE_RG_IPV6")) with if(1) */
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_VOIP_OSIP");

	/* HW Configuration Section */
	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	enable_module("CONFIG_HW_DSP");

	/* VoIP */
	enable_module("MODULE_RG_ATA");
	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");
	token_set_y("CONFIG_RG_MT_PROFILING_FULL_INFO");
    }
    else if (IS_DIST("FULL"))
    {
	token_set_y("CONFIG_RG_SMB");

	/* All OpenRG available Modules */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_IPSEC");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_SNMP");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_VOIP_RV_SIP");
	enable_module("MODULE_RG_VOIP_RV_MGCP");
	enable_module("MODULE_RG_VOIP_RV_H323");
        enable_module("MODULE_RG_DSL");
        enable_module("MODULE_RG_FILESERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
        enable_module("MODULE_RG_PRINTSERVER");
        enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
        enable_module("MODULE_RG_CABLEHOME");
        enable_module("MODULE_RG_VODSL");
    }
    else if (IS_DIST("OLYMPIA_P402_LSP"))
    {
	hw = "OLYMPIA_P40X";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");

	/* Devices */
	/* XXX: Add all devices when drivers are ready.
	 */
	token_set_y("CONFIG_HW_ETH_LAN");
	
	token_set_y("CONFIG_ULIBC");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_SIMPLE_RAMDISK");
 	
	token_set_y("CONFIG_LSP_FLASH_LAYOUT");

	token_set_y("CONFIG_PROC_FS");
 	token_set_y("CONFIG_EXT2_FS");
    }
    else if (IS_DIST("OLYMPIA_P402"))
    {
	hw = "OLYMPIA_P40X";
	os = "LINUX_26";

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_MIPSEL");

	token_set_y("CONFIG_DYN_LINK");

	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_VIDEO_SURVEILLANCE");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_IPSEC");
        enable_module("CONFIG_HW_ENCRYPTION");
	enable_module("MODULE_RG_DSL");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_TR_064");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");

	/* VOIP */
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("CONFIG_HW_DSP");
        enable_module("MODULE_RG_VOIP_ASTERISK_SIP");

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	dev_add_bridge("br0", DEV_IF_NET_INT, "eth0", NULL);

	/* RT2661 uses RT2561 driver */
	enable_module("CONFIG_HW_80211G_RALINK_RT2561");

	token_set_y("CONFIG_HW_DSL_WAN");
	
	token_set_y("CONFIG_ULIBC");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
        token_set_y("CONFIG_RG_MODFS_CRAMFS");

	token_set_y("CONFIG_RG_DYN_FLASH_LAYOUT");

	token_set_y("CONFIG_PROC_FS");
 	token_set_y("CONFIG_EXT2_FS");

	token_set_y("CONFIG_LINUX_SKB_REUSE");

	token_set_m("CONFIG_HW_BUTTONS");
 
	/* XXX Dependency of api and spi modules on crypto should be resolved
	 */
        token_set_m("CONFIG_P400_CRYPTO");

	enable_module("MODULE_RG_PSE");
    }
    else if (IS_DIST("TNETC550W_LSP") || IS_DIST("BVW3653_LSP"))
    {
	if (IS_DIST("TNETC550W_LSP"))
	    hw = "TNETC550W";
	else if (IS_DIST("BVW3653_LSP"))
	    hw = "BVW3653";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");
	token_set("LIBC_IN_TOOLCHAIN", "n");

	token_set_y("CONFIG_HW_ETH_LAN");

	token_set_y("CONFIG_DYN_LINK");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
    }
    else if (IS_DIST("TNETC550W"))
    {
	hw = "TNETC550W";
	os = "LINUX_26";

	token_set_y("CONFIG_TNETC550W_CM");

	token_set("LIBC_IN_TOOLCHAIN", "n");

	enable_module("MODULE_RG_FOUNDATION");
	/* QOS Conflicts with DOCSIS */
	/* enable_module("MODULE_RG_QOS"); */
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_TR_064");

	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_CABLE_WAN");

	token_set_y("CONFIG_RG_DOCSIS");
	token_set_y("CONFIG_RG_PACKET_CABLE");
	token_set_y("CONFIG_DYN_LINK");
    }
    else if (IS_DIST("MVG3420N") || IS_DIST("BVW3653") ||
	IS_DIST("BVW3653_OPENRG") || IS_DIST("BVW3653A1_OPENRG"))
    {
	hw = IS_DIST("MVG3420N") ? "MVG3420N" : "BVW3653";
	os = "LINUX_26";
	int is_integral = IS_DIST("BVW3653_OPENRG") ||
	    IS_DIST("BVW3653A1_OPENRG");

	token_set_y("CONFIG_TNETC550W_CM");
	if (!IS_DIST("MVG3420N"))
	    token_set_y("CONFIG_HITRON_BOARD");

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_RG_FOOTPRINT_REDUCTION");

	if (is_integral)
	{
	    token_set_y("CONFIG_RG_INTEGRAL");
	    token_set("DEVICE_MANUFACTURER_STR", "Hitron Technologies");
	}

	/*  RG Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	/* QOS Conflicts with DOCSIS */
	/* enable_module("MODULE_RG_QOS"); */
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("CONFIG_RG_WPS");
	enable_module("MODULE_RG_REDUCE_SUPPORT");
	enable_module("MODULE_RG_OSAP_AGENT");
	enable_module("MODULE_RG_PSE");
	if (!is_integral)
	    enable_module("MODULE_RG_VAS_CLIENT");

	/*  RG Priority 2  */
	enable_module("MODULE_RG_UPNP");
	if (is_integral)
	    enable_module("MODULE_RG_FILESERVER");
	
	/*  RG Priority 3  */
	//enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	if (!is_integral)
	{
	    enable_module("MODULE_RG_L2TP");
	    enable_module("MODULE_RG_PPTP");
	    token_set_y("CONFIG_RG_SPEED_TEST");
	}

	/* USB Host */
	enable_module("CONFIG_HW_USB_STORAGE");	

	/* General */
	token_set_y("CONFIG_HW_SWITCH_LAN");
	if (IS_DIST("BVW3653A1_OPENRG"))
	    enable_module("CONFIG_HW_80211N_RALINK_RT3052");
	else
	    enable_module("CONFIG_HW_80211N_RALINK_RT2880");
	if (is_integral)
	{
#ifdef CONFIG_RG_DO_DEVICES
	    token_set("CONFIG_RG_DEFAULT_80211N_MODE", "%d", DOT11_MODE_NG);
#endif
	    token_set_y("CONFIG_RG_VAP_SECURED");
	    if (token_get("MODULE_RG_REDUCE_SUPPORT"))
		token_set_y("CONFIG_RG_VAP_HELPLINE");
	}
	token_set_y("CONFIG_HW_CABLE_WAN");
	enable_module("MODULE_RG_VLAN");

	/* Docsis */
	token_set_y("CONFIG_RG_DOCSIS");
	token_set_y("CONFIG_RG_PACKET_CABLE");
	token_set_y("CONFIG_DYN_LINK");

        /* Crash logger */
	token_set_m("CONFIG_RG_KLOG");
	token_set_y("CONFIG_RG_KLOG_RAM_BE");
	token_set_y("CONFIG_RG_KLOG_EMAIL");
	token_set("CONFIG_RG_KLOG_RAMSIZE", "0x100000");

	token_set_m("CONFIG_HW_BUTTONS");
	if (IS_DIST("MVG3420N"))
	    token_set_m("CONFIG_HW_LEDS");
	else
	{
	    token_set_y("CONFIG_INOTIFY");
	    token_set_y("CONFIG_INOTIFY_USER");
	}

	/* Fast path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

	if (!is_integral && token_get("CONFIG_HW_SWITCH_PORT_SEPARATION"))
	{
	    /* Switch port 4 is a WAN interface. */
	    switch_virtual_port_dev_add("eth1", "eth0_main", 4, DEV_IF_NET_EXT);
	}
    }
    else if (IS_DIST("C1KMFCNEVM_LSP"))
    {
	hw = "C1KMFCNEVM";
	os = "LINUX_26";

	token_set_y("CONFIG_LSP_DIST");
	
	token_set("LIBC_IN_TOOLCHAIN", "y");
	token_set_y("CONFIG_ULIBC");

	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
    }
    else if (IS_DIST("C1KMFCNEVM"))
    {
	hw = "C1KMFCNEVM";
	os = "LINUX_26";
	
	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV6_COMCERTO");

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");
	token_set_y("CONFIG_DYN_LINK");

	/* Devices */
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_HW_ETH_WAN");

	/*  RG Priority 1  */
	enable_module("MODULE_RG_FOUNDATION");
	enable_module("MODULE_RG_IPV6");
	enable_module("MODULE_RG_QOS");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	/* Not tested
	 * enable_module("MODULE_RG_REDUCE_SUPPORT");
	 */
	enable_module("CONFIG_RG_ATHEROS_HW_AR5416");
	token_set_y("CONFIG_RG_ATHEROS_FUSION");
	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_PPP");
	enable_module("CONFIG_RG_WPS");

	/*  RG Priority 2  */
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_HOME_PBX");
	enable_module("MODULE_RG_VOIP_ASTERISK_SIP");
	enable_module("CONFIG_HW_DSP");
	enable_module("MODULE_RG_FILESERVER");
	enable_module("CONFIG_HW_USB_STORAGE");	
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_PSE");
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");

	/*  RG Priority 3  */
	enable_module("MODULE_RG_ACCESS_DLNA");
	enable_module("MODULE_RG_MEDIA_SERVER");
	enable_module("MODULE_RG_PPTP");
	enable_module("MODULE_RG_L2TP");
	enable_module("MODULE_RG_IPSEC");

	/*  RG Priority 4  */
	enable_module("MODULE_RG_PRINTSERVER");
	enable_module("MODULE_RG_ADVANCED_ROUTING");

	token_set_y("CONFIG_RG_ZLIB");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
	token_set_y("CONFIG_RG_MAINFS_CRAMFS");
	token_set_y("CONFIG_RG_MODFS_CRAMFS");

	dev_add_bridge("br0", DEV_IF_NET_INT, "eth2", NULL);
    }
    else if (IS_DIST("DB88F6560BP_LSP"))
    {
	/* Marvell SoC */
	hw = "DB88F6560BP";
	os = "LINUX_26";

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");
	token_set_y("CONFIG_DYN_LINK");

	token_set_y("CONFIG_LSP_DIST");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
 	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");
	token_set_y("CONFIG_RG_MTD_UTILS");
    }
    else if (IS_DIST("DB88F6560BP"))
    {
	/* Marvell SoC */
	hw = "DB88F6560BP";
	os = "LINUX_26";

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");
	token_set_y("CONFIG_DYN_LINK");

	enable_module("MODULE_RG_FOUNDATION");
        enable_module("MODULE_RG_PSE");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
 	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");

	token_set_y("CONFIG_RG_PERM_STORAGE_UBI");
	/* 106.5MiB at offset 1.5MiB (after u-boot) */
	token_set("CONFIG_RG_UBI_PARTITION_SIZE", "0x6a80000");
	/* OpenRG Permst UBI Volume ~50MiB */	
	token_set("CONFIG_RG_PERM_STORAGE_UBI_VOLUME_SIZE", "0x31f5000");

	if (token_get("CONFIG_RG_PERM_STORAGE_UBI"))
	    token_set_y("CONFIG_RG_MTD_UTILS");

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L_FEROCEON");
    }
    else if (IS_DIST("VERIZON_FEROCEON") || IS_DIST("FIBERTEC_FEROCEON") ||
	IS_DIST("NCS_FEROCEON") || IS_DIST("FIBERTEC_DB88F6560BP"))
    {
	/* Marvell SoC */
	os = "LINUX_26";

	if (IS_DIST("VERIZON_FEROCEON"))
	{
	    hw = "MI424WR-FEROCEON";
            token_set_y("CONFIG_RG_VERIZON");
	    token_set_y("CONFIG_RG_ATHEROS_FUSION");
	}
	else if (IS_DIST("NCS_FEROCEON"))
	{
	    hw = "MI424WR-FEROCEON";
            token_set_y("CONFIG_RG_NCS");
	}
	else
	{
	    hw = "DB88F6560BP";
            token_set_y("CONFIG_RG_FIBERTEC");
	}

	token_set("LIBC_IN_TOOLCHAIN", "n");
	token_set_y("CONFIG_ULIBC");
	token_set_y("CONFIG_DYN_LINK");

	enable_module("MODULE_RG_FOUNDATION");
        enable_module("MODULE_RG_PSE");
	enable_module("MODULE_RG_VLAN");
	enable_module("MODULE_RG_ADVANCED_ROUTING");
	enable_module("MODULE_RG_ADVANCED_MANAGEMENT");
	token_set_y("CONFIG_RG_EXTERNAL_NTP_LAN_ADVERTISE");
	//enable_module("MODULE_RG_URL_FILTERING");
	enable_module("MODULE_RG_PPP");
	enable_module("MODULE_RG_FIREWALL_AND_SECURITY");
	enable_module("MODULE_RG_UPNP");
	enable_module("MODULE_RG_DSLHOME");
	enable_module("MODULE_RG_QOS");

	/* Default radio is Marvell 8764 3x3 802.11n */
	if (!token_get("CONFIG_QUANTENNA_QHS_71X") &&
	    !token_get("CONFIG_MV_WIFI_8864") &&
	    !token_get("CONFIG_RG_ATHEROS_FUSION"))
	{
	    token_set_m("CONFIG_MV_WIFI_8764");
	}

	if (token_get("CONFIG_RG_ATHEROS_FUSION"))
	    enable_module("CONFIG_RG_ATHEROS_HW_AR5416");

	enable_module("MODULE_RG_WLAN_AND_ADVANCED_WLAN");
	if (token_get("CONFIG_RG_VOIP_MEDIA5"))
	{
	    token_set_m("CONFIG_MARVELL_MMP");
	    enable_module("MODULE_RG_HOME_PBX");
	    token_set_y("CONFIG_RG_MEDIA5_AVANTA");
	}
	enable_module("MODULE_RG_IPV6");
	token_set_y("CONFIG_RG_IPROUTE2_UTILS");

	token_set_y("CONFIG_RG_INITFS_RAMFS");
 	token_set_y("CONFIG_RG_MAINFS_CRAMFS");

	token_set_y("CONFIG_HW_ETH_WAN");
	token_set_y("CONFIG_HW_ETH_LAN");

	/* Fast path */
	if (token_get("MODULE_RG_PSE"))
	    token_set_y("CONFIG_RG_PSE_HW");

	token_set_y("CONFIG_RG_PERM_STORAGE_UBI");
	/* 62Mb (after u-boot) */
	token_set("CONFIG_RG_UBI_PARTITION_SIZE", "0x3e00000");
	token_set("CONFIG_RG_PERM_STORAGE_UBI_VOLUME_SIZE", "0x39d5000");

	if (token_get("CONFIG_RG_PERM_STORAGE_UBI"))
	    token_set_y("CONFIG_RG_MTD_UTILS");

	/* Storage */
#if 0
        /* USB */
	enable_module("CONFIG_HW_USB_HOST_EHCI");
	enable_module("CONFIG_HW_USB_STORAGE");
	/* eSATA Support */
	token_set_y("CONFIG_HW_SATA");
	token_set_y("CONFIG_RG_DISK_MNG");
#endif

	token_set("CONFIG_RG_JPKG_DIST", "JPKG_ARMV5L_FEROCEON");

	/* WAN swap must be present to configure TPM/Fastpath */
	token_set_y("CONFIG_RG_WAN_SWAP");

        if (token_get("CONFIG_RG_FIBERTEC"))
	{
	    token_set_y("CONFIG_GUI_GOOGLE");
	    token_set("CONFIG_RG_DEF_THEME", "google");
	    enable_module("MODULE_RG_IPSEC");
            enable_module("CONFIG_HW_ENCRYPTION");
	    token_set_y("CONFIG_FT_PUBLICWIFI");
	    token_set_y("CONFIG_HW_PON_WAN");
	    token_set("CONFIG_RG_INSTALLATION_WIZARD", "n");
	    token_set("CONFIG_RG_WBM_CONN_TROUBLESHOOT", "n");

	    token_set("CONFIG_RG_HELP_URL", "http://www.actiontec.com/gfiber/help.html");
	    token_set("RMT_UPG_SITE", "update.googlefiber.com");
	    token_set("RG_PROD_STR", "GFRG");
	    token_set("DEVICE_MANUFACTURER_STR", "Actiontec");

	    if (token_get("CONFIG_FIBERTEC_DOGFOOD"))
	    {
		token_set("CONFIG_FT_ACS_URL", "https://acs.canary.gfsvc.com/cwmp");
	    }
	    else
	    {
		token_set("CONFIG_FT_ACS_URL", "https://acs.prod.gfsvc.com/default/cwmp");
		token_set_y("CONFIG_RG_PRODUCTION_LOGIN_HOOK");
	    }

	    token_set("CONFIG_RG_IGD_TITLE", "Google Fiber Network Box");
	    token_set("CONFIG_RG_IGD_MANUFACTURER", "Google Fiber");
	    token_set("CONFIG_RG_IGD_MANUFACTURER_URL", "http://www.google.com/fiber");
	    token_set("CONFIG_RG_IGD_PREFIX", "GFiber");

	    token_set_y("CONFIG_RG_KNET_MIB2_IF_COUNTERS");
	}
        else if (token_get("CONFIG_RG_VERIZON"))
	{
	    token_set_y("CONFIG_GUI_VERIZON");
	    token_set("CONFIG_RG_DEF_THEME", "verizon");
	    token_set_y("CONFIG_RG_VAP_SECURED");
	    token_set("CONFIG_RG_INSTALLATION_WIZARD", "n");

	    token_set("CONFIG_RG_HELP_URL", "http://www.actiontec.com/mi424wr/help.html");
	    token_set("RMT_UPG_SITE", "update.actiontec.com");
	    token_set("DEVICE_MANUFACTURER_STR", "Actiontec");

	    token_set("CONFIG_RG_IGD_TITLE", "Verizon FiOS Router");
	    token_set("CONFIG_RG_IGD_MANUFACTURER", "Verizon");
	    token_set("CONFIG_RG_IGD_MANUFACTURER_URL", "http://www.verizon.com");
	    token_set("CONFIG_RG_IGD_PREFIX", "VerizonFiOS");
	}
	else if (token_get("CONFIG_RG_NCS"))
	{
	    token_set_y("CONFIG_RG_VAP_SECURED");
	    token_set("CONFIG_RG_INSTALLATION_WIZARD", "n");

	    token_set("CONFIG_RG_HELP_URL", "http://www.actiontec.com/mi424wr/help.html");
	    token_set("RMT_UPG_SITE", "update.actiontec.com");
	    token_set("DEVICE_MANUFACTURER_STR", "Actiontec");

	    token_set("CONFIG_RG_IGD_TITLE", "Actiontec Router");
	    token_set("CONFIG_RG_IGD_MANUFACTURER", "Actiontec");
	    token_set("CONFIG_RG_IGD_MANUFACTURER_URL", "http://www.actiontec.com");
	    token_set("CONFIG_RG_IGD_PREFIX", "Actiontec");
	}

	token_set_m("CONFIG_HW_BUTTONS");
        token_set_y("CONFIG_HW_LEDS");
        token_set_y("CONFIG_HW_SENSORS");
    }
    else
	conf_err("invalid DIST=%s\n", dist);

    if (hw && strcmp(hw, "JPKG") && !(*os))
	os = "LINUX_24";

    token_set("CONFIG_RG_DIST", dist);

    /* Event loop method: We use 'poll' by default since we have some linux-2.4
     * platforms that doesn't support epoll */
    if (!token_get("CONFIG_TARGET_EVENT_EPOLL") &&
	!token_get("CONFIG_TARGET_EVENT_SELECT") &&
	!token_get("CONFIG_TARGET_EVENT_POLL"))
    {
	token_set_y("CONFIG_TARGET_EVENT_POLL");
    }
    /* Currently /usr/local/openrg/i386 doesn't support epoll, and will not
     * support it as long as we support compiling RG on kernel-2.4 Hosts */
    if (!token_get("CONFIG_LOCAL_EVENT_EPOLL") &&
	!token_get("CONFIG_LOCAL_EVENT_POLL") &&
	!token_get("CONFIG_LOCAL_EVENT_SELECT"))
    {
	token_set_y("CONFIG_LOCAL_EVENT_POLL");
    }

    if (token_get("CONFIG_HW_DSL_WAN") && token_get("CONFIG_HW_ETH_WAN"))
	token_set_y("CONFIG_RG_MULTIWAN");

    if (token_get("CONFIG_RG_INSTALLTIME_WPA") &&
	!token_get("CONFIG_RG_VAP_INSTALLTIME"))
    {
	conf_err("Can't set CONFIG_RG_INSTALLTIME_WPA without "
	    "CONFIG_RG_VAP_INSTALLTIME enabled\n");
    }
}
