JMK_ROOT=../../../..
include $(JMK_ROOT)/envir.mak

JMK_CFLAGS += -MMD -O2 -Wall -g

JMK_CFLAGS += -I$(JMKE_BUILDDIR)/pkg/hostapd/hostapd-0.8.x/src/
JMK_CFLAGS += -I$(JMKE_BUILDDIR)/pkg/hostapd/hostapd-0.8.x/src/utils/

# Uncomment following line and set the path to your kernel tree include
# directory if your C library does not include all header files.
# JMK_CFLAGS += -DUSE_KERNEL_HEADERS -I/usr/src/linux/include

###############################################################################
#-include .config
###############################################################################
# Driver interface for marvell driver
JMK_CFLAGS += -DMRVL_WSC -DGENERIC_GETIE -DMRVL_WPS2
ifdef CONFIG_MV_WIFI_8864
  JMK_CFLAGS += -I$(JMKE_BUILDDIR)/vendor/marvell/wifi8864/
else
  JMK_CFLAGS += -I$(JMKE_BUILDDIR)/vendor/marvell/wifi8764/
endif

# IEEE 802.11F/IAPP
#CONFIG_IAPP=y

# WPA2/IEEE 802.11i RSN pre-authentication
CONFIG_RSN_PREAUTH=y

# PeerKey handshake for Station to Station Link (IEEE 802.11e DLS)
CONFIG_PEERKEY=y

# IEEE 802.11w (management frame protection)
# This version is an experimental implementation based on IEEE 802.11w/D1.0
# draft and is subject to change since the standard has not yet been finalized.
# Driver support is also needed for IEEE 802.11w.
#CONFIG_IEEE80211W=y

# Integrated EAP server
CONFIG_EAP=y

# EAP-MD5 for the integrated EAP server
CONFIG_EAP_MD5=y

# EAP-TLS for the integrated EAP server
#CONFIG_EAP_TLS=y

# EAP-MSCHAPv2 for the integrated EAP server
#CONFIG_EAP_MSCHAPV2=y

# EAP-PEAP for the integrated EAP server
#CONFIG_EAP_PEAP=y

# EAP-GTC for the integrated EAP server
#CONFIG_EAP_GTC=y

# EAP-TTLS for the integrated EAP server
#CONFIG_EAP_TTLS=y

# EAP-SIM for the integrated EAP server
#CONFIG_EAP_SIM=y

# EAP-AKA for the integrated EAP server
#CONFIG_EAP_AKA=y

# EAP-AKA' for the integrated EAP server
# This requires CONFIG_EAP_AKA to be enabled, too.
#CONFIG_EAP_AKA_PRIME=y

# EAP-PAX for the integrated EAP server
#CONFIG_EAP_PAX=y

# EAP-PSK for the integrated EAP server (this is _not_ needed for WPA-PSK)
#CONFIG_EAP_PSK=y

# EAP-SAKE for the integrated EAP server
#CONFIG_EAP_SAKE=y

# EAP-GPSK for the integrated EAP server
#CONFIG_EAP_GPSK=y
# Include support for optional SHA256 cipher suite in EAP-GPSK
#CONFIG_EAP_GPSK_SHA256=y

# EAP-FAST for the integrated EAP server
# Note: Default OpenSSL package does not include support for all the
# functionality needed for EAP-FAST. If EAP-FAST is enabled with OpenSSL,
# the OpenSSL library must be patched (openssl-0.9.9-session-ticket.patch)
# to add the needed functions.
#CONFIG_EAP_FAST=y

# Wi-Fi Protected Setup (WPS)
CONFIG_WPS=y
# Enable WSC 2.0 support
CONFIG_WPS2=y
# Enable UPnP support for external WPS Registrars
CONFIG_WPS_UPNP=y
CONFIG_WPS_TESTING=y

# EAP-IKEv2
#CONFIG_EAP_IKEV2=y

# Trusted Network Connect (EAP-TNC)
#CONFIG_EAP_TNC=y

# PKCS#12 (PFX) support (used to read private key and certificate file from
# a file that usually has extension .p12 or .pfx)
#CONFIG_PKCS12=y

# RADIUS authentication server. This provides access to the integrated EAP
# server from external hosts using RADIUS.
#CONFIG_RADIUS_SERVER=y

# Build IPv6 support for RADIUS operations
CONFIG_IPV6=y

# IEEE Std 802.11r-2008 (Fast BSS Transition)
#CONFIG_IEEE80211R=y

# Use the hostapd's IEEE 802.11 authentication (ACL), but without
# the IEEE 802.11 Management capability (e.g., madwifi or FreeBSD/net80211)
#CONFIG_DRIVER_RADIUS_ACL=y

# IEEE 802.11n (High Throughput) support
#CONFIG_IEEE80211N=y

# Remove debugging code that is printing out debug messages to stdout.
# This can be used to reduce the size of the hostapd considerably if debugging
# code is not needed.
#CONFIG_NO_STDOUT_DEBUG=y

# Remove support for RADIUS accounting
#CONFIG_NO_ACCOUNTING=y

# Remove support for RADIUS
#CONFIG_NO_RADIUS=y

# Remove support for VLANs
#CONFIG_NO_VLAN=y

# Remove support for dumping state into a file on SIGUSR1 signal
# This can be used to reduce binary size at the cost of disabling a debugging
# option.
#CONFIG_NO_DUMP_STATE=y
###############################################################################

CONFIG_OS=unix

OBJS += main.o
OBJS += config_file.o
OBJS += ../src/ap/hostapd.o
OBJS += ../src/ap/wpa_auth_glue.o
OBJS += ../src/ap/drv_callbacks.o
OBJS += ../src/ap/ap_drv_ops.o
OBJS += ../src/ap/utils.o
OBJS += ../src/ap/authsrv.o
OBJS += ../src/ap/ieee802_1x.o
OBJS += ../src/ap/ap_config.o
OBJS += ../src/ap/ieee802_11_auth.o
OBJS += ../src/ap/sta_info.o
OBJS += ../src/ap/wpa_auth.o
OBJS += ../src/ap/tkip_countermeasures.o
OBJS += ../src/ap/ap_mlme.o
OBJS += ../src/ap/wpa_auth_ie.o
OBJS += ../src/ap/preauth_auth.o
OBJS += ../src/ap/pmksa_cache_auth.o

NEED_RC4=y
NEED_AES=y
NEED_MD5=y
NEED_SHA1=y

OBJS += ../src/drivers/drivers.o
JMK_CFLAGS += -DHOSTAPD

ifdef CONFIG_WPA_TRACE
JMK_CFLAGS += -DWPA_TRACE
OBJS += ../src/utils/trace.o
JMK_LDFLAGS += -rdynamic
JMK_CFLAGS += -funwind-tables
ifdef CONFIG_WPA_TRACE_BFD
JMK_CFLAGS += -DWPA_TRACE_BFD
JMK_LIBS += -lbfd
endif
endif

OBJS += ../src/utils/eloop.o
OBJS += ../src/utils/common.o
OBJS += ../src/utils/wpa_debug.o
OBJS += ../src/utils/wpabuf.o
OBJS += ../src/utils/os_$(CONFIG_OS).o
OBJS += ../src/utils/ip_addr.o

OBJS += ../src/common/ieee802_11_common.o
OBJS += ../src/common/wpa_common.o

OBJS += ../src/eapol_auth/eapol_auth_sm.o


ifndef CONFIG_NO_DUMP_STATE
# define HOSTAPD_DUMP_STATE to include SIGUSR1 handler for dumping state to
# a file (undefine it, if you want to save in binary size)
JMK_CFLAGS += -DHOSTAPD_DUMP_STATE
OBJS += dump_state.o
OBJS += ../src/eapol_auth/eapol_auth_dump.o
endif

ifdef CONFIG_NO_RADIUS
JMK_CFLAGS += -DCONFIG_NO_RADIUS
CONFIG_NO_ACCOUNTING=y
else
OBJS += ../src/radius/radius.o
OBJS += ../src/radius/radius_client.o
endif

ifdef CONFIG_NO_ACCOUNTING
JMK_CFLAGS += -DCONFIG_NO_ACCOUNTING
else
OBJS += ../src/ap/accounting.o
endif

ifdef CONFIG_NO_VLAN
JMK_CFLAGS += -DCONFIG_NO_VLAN
else
OBJS += ../src/ap/vlan_init.o
endif

ifdef CONFIG_NO_CTRL_IFACE
JMK_CFLAGS += -DCONFIG_NO_CTRL_IFACE
else
OBJS += ctrl_iface.o
OBJS += ../src/ap/ctrl_iface_ap.o
endif

OBJS += ../src/crypto/md5.o

JMK_CFLAGS += -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX

ifdef CONFIG_IAPP
JMK_CFLAGS += -DCONFIG_IAPP
OBJS += ../src/ap/iapp.o
endif

ifdef CONFIG_RSN_PREAUTH
JMK_CFLAGS += -DCONFIG_RSN_PREAUTH
CONFIG_L2_PACKET=y
endif

ifdef CONFIG_PEERKEY
JMK_CFLAGS += -DCONFIG_PEERKEY
OBJS += ../src/ap/peerkey_auth.o
endif

ifdef CONFIG_IEEE80211W
JMK_CFLAGS += -DCONFIG_IEEE80211W
NEED_SHA256=y
NEED_AES_OMAC1=y
endif

ifdef CONFIG_IEEE80211R
JMK_CFLAGS += -DCONFIG_IEEE80211R
OBJS += ../src/ap/wpa_auth_ft.o
NEED_SHA256=y
NEED_AES_OMAC1=y
NEED_AES_UNWRAP=y
endif

ifdef CONFIG_IEEE80211N
JMK_CFLAGS += -DCONFIG_IEEE80211N
endif

###############################################################################
#include ../src/drivers/drivers.mak
# Original .config defined CONFIG_DRIVER_MARVELL=y
###############################################################################
ifdef CONFIG_HOSTAPD_DRIVER_MARVELL
JMK_CFLAGS += -DCONFIG_DRIVER_MARVELL
OBJS += ../src/drivers/driver_marvell.o
CONFIG_L2_PACKET=linux
endif

OBJS += ../src/drivers/netlink.o

OBJS += ../src/drivers/linux_ioctl.o
###############################################################################

ifdef CONFIG_L2_PACKET
ifdef CONFIG_DNET_PCAP
ifdef CONFIG_L2_FREEBSD
JMK_LIBS += -lpcap
OBJS += ../src/l2_packet/l2_packet_freebsd.o
else
JMK_LIBS += -ldnet -lpcap
OBJS += ../src/l2_packet/l2_packet_pcap.o
endif
else
OBJS += ../src/l2_packet/l2_packet_linux.o
endif
else
OBJS += ../src/l2_packet/l2_packet_none.o
endif


ifdef CONFIG_EAP_MD5
JMK_CFLAGS += -DEAP_SERVER_MD5
OBJS += ../src/eap_server/eap_server_md5.o
CHAP=y
endif

ifdef CONFIG_EAP_TLS
JMK_CFLAGS += -DEAP_SERVER_TLS
OBJS += ../src/eap_server/eap_server_tls.o
TLS_FUNCS=y
endif

ifdef CONFIG_EAP_PEAP
JMK_CFLAGS += -DEAP_SERVER_PEAP
OBJS += ../src/eap_server/eap_server_peap.o
OBJS += ../src/eap_common/eap_peap_common.o
TLS_FUNCS=y
CONFIG_EAP_MSCHAPV2=y
endif

ifdef CONFIG_EAP_TTLS
JMK_CFLAGS += -DEAP_SERVER_TTLS
OBJS += ../src/eap_server/eap_server_ttls.o
TLS_FUNCS=y
CHAP=y
endif

ifdef CONFIG_EAP_MSCHAPV2
JMK_CFLAGS += -DEAP_SERVER_MSCHAPV2
OBJS += ../src/eap_server/eap_server_mschapv2.o
MS_FUNCS=y
endif

ifdef CONFIG_EAP_GTC
JMK_CFLAGS += -DEAP_SERVER_GTC
OBJS += ../src/eap_server/eap_server_gtc.o
endif

ifdef CONFIG_EAP_SIM
JMK_CFLAGS += -DEAP_SERVER_SIM
OBJS += ../src/eap_server/eap_server_sim.o
CONFIG_EAP_SIM_COMMON=y
NEED_AES_CBC=y
endif

ifdef CONFIG_EAP_AKA
JMK_CFLAGS += -DEAP_SERVER_AKA
OBJS += ../src/eap_server/eap_server_aka.o
CONFIG_EAP_SIM_COMMON=y
NEED_SHA256=y
NEED_AES_CBC=y
endif

ifdef CONFIG_EAP_AKA_PRIME
JMK_CFLAGS += -DEAP_SERVER_AKA_PRIME
endif

ifdef CONFIG_EAP_SIM_COMMON
OBJS += ../src/eap_common/eap_sim_common.o
# Example EAP-SIM/AKA interface for GSM/UMTS authentication. This can be
# replaced with another file implementating the interface specified in
# eap_sim_db.h.
OBJS += ../src/eap_server/eap_sim_db.o
NEED_FIPS186_2_PRF=y
endif

ifdef CONFIG_EAP_PAX
JMK_CFLAGS += -DEAP_SERVER_PAX
OBJS += ../src/eap_server/eap_server_pax.o ../src/eap_common/eap_pax_common.o
endif

ifdef CONFIG_EAP_PSK
JMK_CFLAGS += -DEAP_SERVER_PSK
OBJS += ../src/eap_server/eap_server_psk.o ../src/eap_common/eap_psk_common.o
NEED_AES_OMAC1=y
NEED_AES_ENCBLOCK=y
NEED_AES_EAX=y
endif

ifdef CONFIG_EAP_SAKE
JMK_CFLAGS += -DEAP_SERVER_SAKE
OBJS += ../src/eap_server/eap_server_sake.o ../src/eap_common/eap_sake_common.o
endif

ifdef CONFIG_EAP_GPSK
JMK_CFLAGS += -DEAP_SERVER_GPSK
OBJS += ../src/eap_server/eap_server_gpsk.o ../src/eap_common/eap_gpsk_common.o
ifdef CONFIG_EAP_GPSK_SHA256
JMK_CFLAGS += -DEAP_SERVER_GPSK_SHA256
endif
NEED_SHA256=y
NEED_AES_OMAC1=y
endif

ifdef CONFIG_EAP_PWD
JMK_CFLAGS += -DEAP_SERVER_PWD
OBJS += ../src/eap_server/eap_server_pwd.o ../src/eap_common/eap_pwd_common.o
NEED_SHA256=y
endif

ifdef CONFIG_EAP_VENDOR_TEST
JMK_CFLAGS += -DEAP_SERVER_VENDOR_TEST
OBJS += ../src/eap_server/eap_server_vendor_test.o
endif

ifdef CONFIG_EAP_FAST
JMK_CFLAGS += -DEAP_SERVER_FAST
OBJS += ../src/eap_server/eap_server_fast.o
OBJS += ../src/eap_common/eap_fast_common.o
TLS_FUNCS=y
NEED_T_PRF=y
NEED_AES_UNWRAP=y
endif

ifdef CONFIG_WPS
ifdef CONFIG_WPS2
JMK_CFLAGS += -DCONFIG_WPS2
endif

JMK_CFLAGS += -DCONFIG_WPS -DEAP_SERVER_WSC
OBJS += ../src/utils/uuid.o
OBJS += ../src/ap/wps_hostapd.o
OBJS += ../src/eap_server/eap_server_wsc.o ../src/eap_common/eap_wsc_common.o
OBJS += ../src/wps/wps.o
OBJS += ../src/wps/wps_common.o
OBJS += ../src/wps/wps_attr_parse.o
OBJS += ../src/wps/wps_attr_build.o
OBJS += ../src/wps/wps_attr_process.o
OBJS += ../src/wps/wps_dev_attr.o
OBJS += ../src/wps/wps_enrollee.o
OBJS += ../src/wps/wps_registrar.o
NEED_DH_GROUPS=y
NEED_SHA256=y
NEED_BASE64=y
NEED_AES_CBC=y
NEED_MODEXP=y
CONFIG_EAP=y

ifdef CONFIG_WPS_UFD
JMK_CFLAGS += -DCONFIG_WPS_UFD
OBJS += ../src/wps/wps_ufd.o
NEED_WPS_OOB=y
endif

ifdef CONFIG_WPS_NFC
JMK_CFLAGS += -DCONFIG_WPS_NFC
OBJS += ../src/wps/ndef.o
OBJS += ../src/wps/wps_nfc.o
NEED_WPS_OOB=y
ifdef CONFIG_WPS_NFC_PN531
PN531_PATH ?= /usr/local/../src/nfc
JMK_CFLAGS += -DCONFIG_WPS_NFC_PN531
JMK_CFLAGS += -I${PN531_PATH}/inc
OBJS += ../src/wps/wps_nfc_pn531.o
LIBS += ${PN531_PATH}/lib/wpsnfc.dll
LIBS += ${PN531_PATH}/lib/libnfc_mapping_pn53x.dll
endif
endif

ifdef NEED_WPS_OOB
JMK_CFLAGS += -DCONFIG_WPS_OOB
endif

ifdef CONFIG_WPS_UPNP
JMK_CFLAGS += -DCONFIG_WPS_UPNP
OBJS += ../src/wps/wps_upnp.o
OBJS += ../src/wps/wps_upnp_ssdp.o
OBJS += ../src/wps/wps_upnp_web.o
OBJS += ../src/wps/wps_upnp_event.o
OBJS += ../src/wps/wps_upnp_ap.o
OBJS += ../src/wps/upnp_xml.o
OBJS += ../src/wps/httpread.o
OBJS += ../src/wps/http_client.o
OBJS += ../src/wps/http_server.o
endif

ifdef CONFIG_WPS_STRICT
JMK_CFLAGS += -DCONFIG_WPS_STRICT
OBJS += ../src/wps/wps_validate.o
endif

ifdef CONFIG_WPS_TESTING
JMK_CFLAGS += -DCONFIG_WPS_TESTING
endif

endif

ifdef CONFIG_EAP_IKEV2
JMK_CFLAGS += -DEAP_SERVER_IKEV2
OBJS += ../src/eap_server/eap_server_ikev2.o ../src/eap_server/ikev2.o
OBJS += ../src/eap_common/eap_ikev2_common.o ../src/eap_common/ikev2_common.o
NEED_DH_GROUPS=y
NEED_DH_GROUPS_ALL=y
NEED_MODEXP=y
NEED_CIPHER=y
endif

ifdef CONFIG_EAP_TNC
JMK_CFLAGS += -DEAP_SERVER_TNC
OBJS += ../src/eap_server/eap_server_tnc.o
OBJS += ../src/eap_server/tncs.o
NEED_BASE64=y
ifndef CONFIG_DRIVER_BSD
LIBS += -ldl
endif
endif

# Basic EAP functionality is needed for EAPOL
OBJS += eap_register.o
OBJS += ../src/eap_server/eap_server.o
OBJS += ../src/eap_common/eap_common.o
OBJS += ../src/eap_server/eap_server_methods.o
OBJS += ../src/eap_server/eap_server_identity.o
JMK_CFLAGS += -DEAP_SERVER_IDENTITY

ifdef CONFIG_EAP
JMK_CFLAGS += -DEAP_SERVER
endif

ifdef CONFIG_PKCS12
JMK_CFLAGS += -DPKCS12_FUNCS
endif

ifdef MS_FUNCS
OBJS += ../src/crypto/ms_funcs.o
NEED_DES=y
NEED_MD4=y
endif

ifdef CHAP
OBJS += ../src/eap_common/chap.o
endif

ifdef TLS_FUNCS
NEED_DES=y
# Shared TLS functions (needed for EAP_TLS, EAP_PEAP, and EAP_TTLS)
JMK_CFLAGS += -DEAP_TLS_FUNCS
OBJS += ../src/eap_server/eap_server_tls_common.o
NEED_TLS_PRF=y
endif

ifndef CONFIG_TLS
CONFIG_TLS=openssl
endif

ifeq ($(CONFIG_TLS), openssl)
ifdef TLS_FUNCS
OBJS += ../src/crypto/tls_openssl.o
LIBS += -lssl
endif
OBJS += ../src/crypto/crypto_openssl.o
HOBJS += ../src/crypto/crypto_openssl.o
ifdef NEED_FIPS186_2_PRF
OBJS += ../src/crypto/fips_prf_openssl.o
endif
LIBS += -lcrypto
LIBS_h += -lcrypto
endif

ifeq ($(CONFIG_TLS), gnutls)
ifdef TLS_FUNCS
OBJS += ../src/crypto/tls_gnutls.o
LIBS += -lgnutls -lgpg-error
ifdef CONFIG_GNUTLS_EXTRA
JMK_CFLAGS += -DCONFIG_GNUTLS_EXTRA
LIBS += -lgnutls-extra
endif
endif
OBJS += ../src/crypto/crypto_gnutls.o
HOBJS += ../src/crypto/crypto_gnutls.o
ifdef NEED_FIPS186_2_PRF
OBJS += ../src/crypto/fips_prf_gnutls.o
endif
LIBS += -lgcrypt
LIBS_h += -lgcrypt
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), schannel)
ifdef TLS_FUNCS
OBJS += ../src/crypto/tls_schannel.o
endif
OBJS += ../src/crypto/crypto_cryptoapi.o
OBJS_p += ../src/crypto/crypto_cryptoapi.o
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), nss)
ifdef TLS_FUNCS
OBJS += ../src/crypto/tls_nss.o
LIBS += -lssl3
endif
OBJS += ../src/crypto/crypto_nss.o
ifdef NEED_FIPS186_2_PRF
OBJS += ../src/crypto/fips_prf_nss.o
endif
LIBS += -lnss3
LIBS_h += -lnss3
CONFIG_INTERNAL_MD4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), internal)
ifndef CONFIG_CRYPTO
CONFIG_CRYPTO=internal
endif
ifdef TLS_FUNCS
OBJS += ../src/crypto/crypto_internal-rsa.o
OBJS += ../src/crypto/tls_internal.o
OBJS += ../src/tls/tlsv1_common.o
OBJS += ../src/tls/tlsv1_record.o
OBJS += ../src/tls/tlsv1_cred.o
OBJS += ../src/tls/tlsv1_server.o
OBJS += ../src/tls/tlsv1_server_write.o
OBJS += ../src/tls/tlsv1_server_read.o
OBJS += ../src/tls/asn1.o
OBJS += ../src/tls/rsa.o
OBJS += ../src/tls/x509v3.o
OBJS += ../src/tls/pkcs1.o
OBJS += ../src/tls/pkcs5.o
OBJS += ../src/tls/pkcs8.o
NEED_SHA256=y
NEED_BASE64=y
NEED_TLS_PRF=y
NEED_MODEXP=y
NEED_CIPHER=y
JMK_CFLAGS += -DCONFIG_TLS_INTERNAL
JMK_CFLAGS += -DCONFIG_TLS_INTERNAL_SERVER
endif
ifdef NEED_CIPHER
NEED_DES=y
OBJS += ../src/crypto/crypto_internal-cipher.o
endif
ifdef NEED_MODEXP
OBJS += ../src/crypto/crypto_internal-modexp.o
OBJS += ../src/tls/bignum.o
endif
ifeq ($(CONFIG_CRYPTO), libtomcrypt)
OBJS += ../src/crypto/crypto_libtomcrypt.o
LIBS += -ltomcrypt -ltfm
LIBS_h += -ltomcrypt -ltfm
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif
ifeq ($(CONFIG_CRYPTO), internal)
OBJS += ../src/crypto/crypto_internal.o
NEED_AES_DEC=y
JMK_CFLAGS += -DCONFIG_CRYPTO_INTERNAL
ifdef CONFIG_INTERNAL_LIBTOMMATH
JMK_CFLAGS += -DCONFIG_INTERNAL_LIBTOMMATH
ifdef CONFIG_INTERNAL_LIBTOMMATH_FAST
JMK_CFLAGS += -DLTM_FAST
endif
else
LIBS += -ltommath
LIBS_h += -ltommath
endif
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_DES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD4=y
CONFIG_INTERNAL_MD5=y
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif
ifeq ($(CONFIG_CRYPTO), cryptoapi)
OBJS += ../src/crypto/crypto_cryptoapi.o
OBJS_p += ../src/crypto/crypto_cryptoapi.o
JMK_CFLAGS += -DCONFIG_CRYPTO_CRYPTOAPI
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
endif
endif

ifeq ($(CONFIG_TLS), none)
ifdef TLS_FUNCS
OBJS += ../src/crypto/tls_none.o
JMK_CFLAGS += -DEAP_TLS_NONE
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD5=y
endif
OBJS += ../src/crypto/crypto_none.o
OBJS_p += ../src/crypto/crypto_none.o
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
endif

ifndef TLS_FUNCS
OBJS += ../src/crypto/tls_none.o
ifeq ($(CONFIG_TLS), internal)
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD5=y
CONFIG_INTERNAL_RC4=y
endif
endif

AESOBJS = # none so far
ifdef CONFIG_INTERNAL_AES
AESOBJS += ../src/crypto/aes-internal.o ../src/crypto/aes-internal-enc.o
endif

AESOBJS += ../src/crypto/aes-wrap.o
ifdef NEED_AES_EAX
AESOBJS += ../src/crypto/aes-eax.o
NEED_AES_CTR=y
endif
ifdef NEED_AES_CTR
AESOBJS += ../src/crypto/aes-ctr.o
endif
ifdef NEED_AES_ENCBLOCK
AESOBJS += ../src/crypto/aes-encblock.o
endif
ifdef NEED_AES_OMAC1
AESOBJS += ../src/crypto/aes-omac1.o
endif
ifdef NEED_AES_UNWRAP
NEED_AES_DEC=y
AESOBJS += ../src/crypto/aes-unwrap.o
endif
ifdef NEED_AES_CBC
NEED_AES_DEC=y
AESOBJS += ../src/crypto/aes-cbc.o
endif
ifdef NEED_AES_DEC
ifdef CONFIG_INTERNAL_AES
AESOBJS += ../src/crypto/aes-internal-dec.o
endif
endif
ifdef NEED_AES
OBJS += $(AESOBJS)
endif

ifdef NEED_SHA1
SHA1OBJS += ../src/crypto/sha1.o
ifdef CONFIG_INTERNAL_SHA1
SHA1OBJS += ../src/crypto/sha1-internal.o
ifdef NEED_FIPS186_2_PRF
SHA1OBJS += ../src/crypto/fips_prf_internal.o
endif
endif
SHA1OBJS += ../src/crypto/sha1-pbkdf2.o
ifdef NEED_T_PRF
SHA1OBJS += ../src/crypto/sha1-tprf.o
endif
ifdef NEED_TLS_PRF
SHA1OBJS += ../src/crypto/sha1-tlsprf.o
endif
endif

ifdef NEED_SHA1
OBJS += $(SHA1OBJS)
endif

ifdef NEED_MD5
ifdef CONFIG_INTERNAL_MD5
OBJS += ../src/crypto/md5-internal.o
endif
endif

ifdef NEED_MD4
ifdef CONFIG_INTERNAL_MD4
OBJS += ../src/crypto/md4-internal.o
endif
endif

ifdef NEED_DES
ifdef CONFIG_INTERNAL_DES
OBJS += ../src/crypto/des-internal.o
endif
endif

ifdef NEED_RC4
ifdef CONFIG_INTERNAL_RC4
OBJS += ../src/crypto/rc4.o
endif
endif

ifdef NEED_SHA256
OBJS += ../src/crypto/sha256.o
ifdef CONFIG_INTERNAL_SHA256
OBJS += ../src/crypto/sha256-internal.o
endif
endif

ifdef NEED_DH_GROUPS
OBJS += ../src/crypto/dh_groups.o
endif
ifdef NEED_DH_GROUPS_ALL
JMK_CFLAGS += -DALL_DH_GROUPS
endif
ifdef CONFIG_INTERNAL_DH_GROUP5
ifdef NEED_DH_GROUPS
OBJS += ../src/crypto/dh_group5.o
endif
endif

ifdef CONFIG_RADIUS_SERVER
JMK_CFLAGS += -DRADIUS_SERVER
OBJS += ../src/radius/radius_server.o
endif

ifdef CONFIG_IPV6
JMK_CFLAGS += -DCONFIG_IPV6
endif

ifdef CONFIG_DRIVER_RADIUS_ACL
JMK_CFLAGS += -DCONFIG_DRIVER_RADIUS_ACL
endif

ifdef CONFIG_FULL_DYNAMIC_VLAN
# define CONFIG_FULL_DYNAMIC_VLAN to have hostapd manipulate bridges
# and vlan interfaces for the vlan feature.
JMK_CFLAGS += -DCONFIG_FULL_DYNAMIC_VLAN
endif

ifdef NEED_BASE64
OBJS += ../src/utils/base64.o
endif

ifdef NEED_AP_MLME
OBJS += ../src/ap/beacon.o
OBJS += ../src/ap/wmm.o
OBJS += ../src/ap/ap_list.o
OBJS += ../src/ap/ieee802_11.o
OBJS += ../src/ap/hw_features.o
JMK_CFLAGS += -DNEED_AP_MLME
endif
ifdef CONFIG_IEEE80211N
OBJS += ../src/ap/ieee802_11_ht.o
endif

ifdef CONFIG_P2P_MANAGER
JMK_CFLAGS += -DCONFIG_P2P_MANAGER
OBJS += ../src/ap/p2p_hostapd.o
endif

ifdef CONFIG_NO_STDOUT_DEBUG
JMK_CFLAGS += -DCONFIG_NO_STDOUT_DEBUG
endif


JMK_TARGET=hostapd
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)
JMK_O_OBJS=$(OBJS)
#JMK_O_OBJS_MULTIPLE_$(TARGET)+=$(OBJS_MULTIPLE)
JMK_LDFLAGS_$(JMK_TARGET)+=-L$(JMKE_BUILDDIR)/pkg/lib/ -lcrypto -lssl 
include $(JMKE_MK)

