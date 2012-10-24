DEFINES+=-DSYSCONFDIR=\"'/etc/'\"

JMK_CFLAGS+=-I$(JMKE_BUILDDIR)/pkg/route/quagga/quagga \
 -I$(JMKE_BUILDDIR)/pkg/route/quagga/quagga/lib
JMK_CFLAGS+= $(DEFINES) -DMULTIPATH_NUM=1 -DHAVE_CONFIG_H


JMK_LDFLAGS+=-L$(JMKE_BUILDDIR)/pkg/route/quagga/quagga/lib
JMK_LDFLAGS+=-L$(JMKE_BUILDDIR)/pkg/route/quagga/quagga
