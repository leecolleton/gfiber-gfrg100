JMK_ROOT=../../../
include $(JMK_ROOT)/envir.mak

JMK_TARGET=pluto _pluto_adns
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_EXPORT_HEADERS_DIR=freeswan/pluto
JMK_EXPORT_HEADERS+=rg_utils.h constants.h defs.h whack.h packet.h ipsec_ipc.h

JMK_O_OBJS_pluto=asn1.o connections.o constants.o cookie.o crypto.o defs.o log.o \
	     state.o main.o server.o timer.o pem.o pkcs.o x509.o id.o \
	     ipsec_doi.o kernel.o kernel_comm.o demux.o packet.o preshared.o \
	     dnskey.o rnd.o spdb.o sha1.o md5.o md2.o rg_utils.o block_ip.o \
	     db_ops.o vendor.o dpd.o

# Current version of ARM compiler have some optimization bugs. Following
# ifeq will be removed after compiler update.
ifeq ($(ARCH),arm)
JMK_CFLAGS_demux.o+=-O1
endif

ifeq ($(CONFIG_RG_IPSEC_IKE_ALG),y)
JMK_SUBDIRS=alg
JMK_O_OBJS_pluto+=kernel_alg.o ike_alg.o alg_info.o alg/libalg.o
endif

JMK_O_OBJS_whack=whack.o
JMK_O_OBJS__pluto_adns=adns.o

KLIPSD=../klips/net/ipsec
# -O on Linux makes gcc coredump when compiling sha1.c
# -Wundef is nice but RHL5.2 compiler doesn't support it
JMK_CFLAGS+=-g -Wall -W -Wmissing-prototypes -Wpointer-arith -Wbad-function-cast \
	-Wcast-qual -Wmissing-declarations -Wwrite-strings -Wstrict-prototypes \
	-I$(KLIPSD) -I../lib -I$(JMKE_BUILDDIR)/pkg/gmp -I../libdes \
	-DPLUTO -DKLIPS -DDODGE_DH_MISSING_ZERO_BUG -DDEBUG -DGCC_LINT \
	-fno-builtin-log

ifeq ($(CONFIG_IPSEC_NAT_TRAVERSAL),y)
    JMK_O_OBJS_pluto+=nat_traversal.o
    JMK_CFLAGS+=-DNAT_TRAVERSAL
endif

# libefence is a free memory allocation debugger
JMK_LDFLAGS+=-L$(JMKE_BUILDDIR)/pkg/gmp -L../libdes -L../lib
JMK_LDLIBS:=$(OPENRG_GPL_LDLIBS) -lgmp -ldes -lfreeswan $(JMK_LDLIBS)

JMK_OTHER_DEPS += $(OPENRG_GPL_LIB)

ifeq ($(CONFIG_RG_SYSLOG_GLIBC),y)
    JMK_CFLAGS += -include $(JMK_ROOT)/pkg/syslog/glibc/ulibc_syslog.h
    JMK_LDFLAGS += -L$(JMK_ROOT)/pkg/syslog/glibc/
    JMK_LDLIBS:=-lresolv -lulibc_syslog $(JMK_LDLIBS)
endif

include $(JMKE_MK)

ifeq ($(CONFIG_RG_SYSLOG_GLIBC),y)
$(JMK_TARGET): $(JMKE_BUILDDIR)/pkg/syslog/glibc/libulibc_syslog.a
endif

ifeq ($(CONFIG_RG_IPSEC_NO_SECRET_FILE),y)
JMK_CFLAGS+=-DNO_SECRET_FILE
endif
