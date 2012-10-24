JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_O_OBJS_archival.a+=$(if $(CONFIG_AR),ar.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_BUNZIP2),bunzip2.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_CPIO),cpio.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_DPKG),dpkg.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_DPKG_DEB),dpkg_deb.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_GUNZIP),gunzip.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_GZIP),gzip.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_RPM2CPIO),rpm2cpio.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_RPM),rpm.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_TAR),tar.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_UNCOMPRESS),uncompress.o)
JMK_O_OBJS_archival.a+=$(if $(CONFIG_UNZIP),unzip.o)

ifneq ($(strip $(JMK_O_OBJS_archival.a)),)
  JMK_A_TARGET+=archival.a
endif

include $(JMKE_MK)
