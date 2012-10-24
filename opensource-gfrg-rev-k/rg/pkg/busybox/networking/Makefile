JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_A_TARGET+=networking.a

JMK_O_OBJS_networking.a+=$(if $(CONFIG_ARPING),arping.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_FTPGET),ftpgetput.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_FTPPUT),ftpgetput.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_HOSTNAME),hostname.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_HTTPD),httpd.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IFCONFIG),ifconfig.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IFUPDOWN),ifupdown.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_INETD),inetd.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IP),ip.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IPCALC),ipcalc.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IPADDR),ipaddr.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IPLINK),iplink.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IPROUTE),iproute.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_IPTUNNEL),iptunnel.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_NAMEIF),nameif.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_NC),nc.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_NETSTAT),netstat.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_NSLOOKUP),nslookup.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_PING),ping.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_PING6),ping6.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_ROUTE),route.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_TELNET),telnet.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_TELNETD),telnetd.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_TFTP),tftp.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_TRACEROUTE),traceroute.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_VCONFIG),vconfig.o)
JMK_O_OBJS_networking.a+=$(if $(CONFIG_WGET),wget.o)

ifdef CONFIG_FEATURE_HTTPD_AUTH_MD5
  JMK_LDLIBS_networking.a:=lcrypt $(JMK_LDLIBS_networking.a)
endif

include $(JMKE_MK)
