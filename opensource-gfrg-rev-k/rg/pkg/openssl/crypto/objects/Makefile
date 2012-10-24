ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=objects.h
JMK_CD_EXPORTED_FILES=obj_dat.pl objects.pl objects.txt obj_mac.num.orig \
		      objxref.pl obj_xref.txt

JMK_O_OBJS=o_names.o obj_dat.o obj_lib.o obj_err.o obj_xref.o

JMK_ARCHCONFIG_JPKG_FIRST_TASKS+=$(JMKE_PWD_BUILD)/obj_dat.h \
  $(JMKE_PWD_BUILD)/obj_mac.num $(JMKE_PWD_BUILD)/obj_xref.h

$(JMKE_PWD_BUILD)/obj_dat.h: $(JMKE_PWD_SRC)/obj_dat.pl \
  $(JMKE_PWD_BUILD)/obj_mac.h
	$(PERL) $(JMKE_DEPS) $@

# objects.pl both reads and writes obj_mac.num
$(JMKE_PWD_BUILD)/obj_mac.h: $(JMKE_PWD_SRC)/objects.pl \
  $(JMKE_PWD_SRC)/objects.txt $(JMKE_PWD_BUILD)/obj_mac.num
	$(PERL) $(JMKE_DEPS) $@

JMK_EXPORT_HEADERS+=obj_mac.h

$(JMKE_PWD_BUILD)/obj_xref.h: $(JMKE_PWD_SRC)/objxref.pl \
  $(JMKE_PWD_BUILD)/obj_mac.num $(JMKE_PWD_SRC)/obj_xref.txt
	$(PERL) $(JMKE_DEPS) > $@

$(JMKE_PWD_BUILD)/obj_mac.num: $(JMKE_PWD_SRC)/obj_mac.num.orig
	@cp $(JMKE_DEPS) $@
	chmod u+w $@

$(call JMKE_INCLUDE_RULES)
