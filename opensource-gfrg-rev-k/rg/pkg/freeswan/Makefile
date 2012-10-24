JMK_ROOT=../../
include $(JMK_ROOT)/envir.mak

JMK_EXPORT_HEADERS_DIR=freeswan
JMK_EXPORT_HEADERS+=rg/netbios_rt/netbios_rt.h

JMK_JPKG_EXPORTED_DIR+=doc klips lib libcrypto libdes packaging pluto testing \
  utils zlib

ifeq ($(CONFIG_FREESWAN),y)
  JMK_ARCHCONFIG_FIRST_TASKS=kernel_links
  JMK_SUBDIRS=lib libdes pluto utils rg klips

else
  ifeq ($(CONFIG_LIBDES),y)
    JMK_SUBDIRS=libdes
  endif
endif

ifeq ($(CONFIG_FREESWAN),y)
  KERNELKLIPS=$(CURDIR)/klips/net/ipsec
  KLIPSALG=$(KERNELKLIPS)/alg
  PLUTOALG=$(CURDIR)/pluto/alg
  JMK_LINK_DIRS=$(addprefix $(KERNELKLIPS)/,libfreeswan zlib libcrypto libdes)
  JMK_LINK_DIRS+=$(addprefix $(KLIPSALG)/,libmd5 libsha1 libaes)
  JMK_LINK_DIRS+=$(addprefix $(PLUTOALG)/,libaes)
endif

ifeq ($(CONFIG_FREESWAN),y)
$(JMK_LINK_DIRS):
	$(MKDIR) $@

clean_dirs:
	rm -fr $(JMK_LINK_DIRS)

zlib_links:
	$(MKDIR) zlib
	$(JMKE_LN) $(RG_INCLUDE_DIR)/zlib/c/*.c zlib
	$(JMKE_LN) $(RG_INCLUDE_DIR)/zlib/*.h zlib

kernel_links: $(JMK_LINK_DIRS) zlib_links
	$(JMKE_LN) $(JMKE_PWD_SRC)/lib/Makefile.kernel $(KERNELKLIPS)/libfreeswan/Makefile
	$(JMKE_LN) $(JMKE_PWD_SRC)/lib/*.[ch] $(KERNELKLIPS)/libfreeswan
	$(JMKE_LN) $(JMKE_PWD_SRC)/libdes/Makefile.kern $(KERNELKLIPS)/libdes/Makefile
	$(JMKE_LN) $(JMKE_PWD_SRC)/libdes/*.[ch] $(KERNELKLIPS)/libdes
	$(JMKE_LN) $(JMKE_PWD_SRC)/zlib/Makefile $(KERNELKLIPS)/zlib/Makefile
	$(JMKE_LN) $(CURDIR)/zlib/*.[chS] $(KERNELKLIPS)/zlib
	$(JMKE_LN) $(JMKE_PWD_SRC)/libcrypto/* $(KERNELKLIPS)/libcrypto
	$(JMKE_LN) $(KERNELKLIPS)/libcrypto/libmd5/* $(KLIPSALG)/libmd5
	$(JMKE_LN) $(KERNELKLIPS)/libcrypto/libsha1/* $(KLIPSALG)/libsha1
	$(JMKE_LN) $(KERNELKLIPS)/libcrypto/libaes/Makefile.kern $(KLIPSALG)/libaes/Makefile
	$(JMKE_LN) $(KERNELKLIPS)/libcrypto/libaes/*.[ch] $(KLIPSALG)/libaes
	$(JMKE_LN) $(JMKE_PWD_SRC)/libcrypto/libaes/Makefile $(PLUTOALG)/libaes/Makefile
	$(JMKE_LN) $(JMKE_PWD_SRC)/libcrypto/libaes/*.[ch] $(PLUTOALG)/libaes

JMK_RAMDISK_LAST_TASKS+=ramdisk_ipsec.d
ramdisk_ipsec.d:
	$(JMKE_MKDIR) $(JMKE_RAMDISK_RW_DIR)/etc/ipsec.d
	$(JMKE_MKDIR) $(JMKE_RAMDISK_RW_DIR)/etc/ipsec.d/private
	$(JMKE_MKDIR) $(JMKE_RAMDISK_RW_DIR)/etc/ipsec.d/cacerts
endif

include $(JMKE_MK)
