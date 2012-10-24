
OpenRG 6.0.7.1.4 for feroceon reference board Compilation
--------------------------------------------------
To build the openrg.img run:
$ cd /home/tranand/rg-6.0.7.1.4/rg
$ make config DIST=FIBERTEC_FEROCEON LIC=/home/tranand/rg-6.0.7.1.4/jpkg_fibertec.lic && make


OpenRG 5.4.13.1.24 for feroceon reference board with Marvell Wireless Compilation
---------------------------------------------------------------------------------
make config DIST=FIBERTEC_FEROCEON CONFIG_MV_WIFI_8764=m LIC=../jpkg_fibertec.lic && make
make config DIST=FIBERTEC_FEROCEON CONFIG_MV_WIFI_8764=m CONFIG_RG_VOIP_MEDIA5=y LIC=../jpkg_fibertec.lic && make
make config DIST=FIBERTEC_FEROCEON CONFIG_RG_PROD_IMG=y CONFIG_MV_WIFI_8764=m LIC=../jpkg_fibertec.lic && make
make config DIST=FIBERTEC_FEROCEON CONFIG_RG_PROD_IMG=y CONFIG_MV_WIFI_8764=m CONFIG_RG_VOIP_MEDIA5=y LIC=../jpkg_fibertec.lic && make


OpenRG 5.4.13.1.24 for feroceon reference board with Atheros Wireless Compilation
---------------------------------------------------------------------------------
make config DIST=FIBERTEC_FEROCEON CONFIG_RG_ATHEROS_FUSION=y LIC=../jpkg_fibertec.lic && make
make config DIST=FIBERTEC_FEROCEON CONFIG_RG_ATHEROS_FUSION=y CONFIG_RG_VOIP_MEDIA5=y LIC=../jpkg_fibertec.lic && make
