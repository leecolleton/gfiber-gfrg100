export DOC_SCRIPTS:=$(JMK_ROOT)/pkg/doc/scripts/
export PATH:=/usr/local/openrg/bin:$(PATH)
export RG_SHELL_FUNCS=. $(JMK_ROOT)/jmk/old/mk_funcs.sh
export JAVA_ARGS=-Xms5m -Xmx1000m \
  -Dlog4j.configuration=file:////usr/share/java/log4j.xml

DOCS_DIR=$(JMK_ROOT)/pkg/doc
FOP=fop
XSLTPROC=xsltproc
XMLFMT=$(JMK_ROOT)/pkg/doc/scripts/xmlformat.pl \
  --config-file $(JMK_ROOT)/pkg/doc/scripts/xmlformat.conf
	
XML_FMT:=$(XML_TARGET:%.xml=fmt_%.xml)
XML_PDF:=$(XML_TARGET:%.xml=%.pdf)
XML_RTF:=$(XML_TARGET:%.xml=%.rtf)
XML_HTML:=$(XML_TARGET:%.xml=html_%/)
XML_HTML_WEB:=$(XML_TARGET:%.xml=%.html/)
XML_FO=$(patsubst %.xml,%.fo,$(XML_TARGET))
XML_ONLINE=$(XML_TARGET:%.xml=%.xml.online) 
XML_GEN_FILES=version.ent 
XML_DEP+=$(XML_GEN_FILES) $(DOC_SCRIPTS)/definitions.ent \
  $(DOC_SCRIPTS)/common.xsl $(DOC_SCRIPTS)/print.xsl \
  $(DOC_SCRIPTS)/html_theme.css $(DOC_SCRIPTS)/html.xsl \
  $(DOC_SCRIPTS)/html_chunk.xsl $(DOC_SCRIPTS)/html_common.xsl \
  $(JMKE_BUILDDIR)/pkg/doc/scripts/j_titlepage.xsl \
  $(JMKE_BUILDDIR)/pkg/doc/scripts/j_titlepage_a5.xsl \
  $(DOC_SCRIPTS)/preamble.ent runtime_profile.xsl
ARCH_DEPENDENT_PNGS:=$(addprefix $(JMKE_PWD_BUILD)/images/,$(ARCH_IMG))

ifdef CONFIG_MACH_PUMA5_BVW3653
  BOARD_SUFFIX=-hitron
endif
ifdef CONFIG_RG_HW_IAD303
  BOARD_SUFFIX=-vtech
endif

JMK_ARCHCONFIG_FIRST_TASKS+=$(UM_IMG_LINK) $(PG_IMG_LINK) $(UM_XML_LINK) \
  $(PG_XML_LINK)
JMK_ARCHCONFIG_LAST_TASKS+=$(ARCH_DEPENDENT_PNGS) 

XML_TARGETS=$(XML_PDF) $(XML_RTF) $(XML_HTML) $(XML_HTML_WEB)

xmldocs: $(DOCS_SUBDIRS_) $(XML_TARGETS)
xmlfmt: $(XML_FMT)

# Add to clean target all documentation related files
ifneq ($(XML_TARGET)),)
  JMK_CLEAN+=$(XML_GEN_FILES) $(XML_ONLINE) $(XML_FO) $(XML_PDF) $(XML_HTML) 
  JMK_CLEAN+=$(XML_RTF) $(XML_HTML_WEB) runtime_profile.xsl module_list.sh
endif

$(XML_FMT) : fmt_%.xml : %.xml
	$(XMLFMT) -i $<

$(XML_PDF) : %.pdf : print_profile_% %.fo 
	$(FOP) -c $(JMK_ROOT)/pkg/doc/scripts/fop_conf.xml -fo  \
	  $(filter-out print_profile_%,$^) -pdf  $@

$(XML_FO) : %.fo : %.xml.online
	$(XSLTPROC) --output $@ \
	  $(if $(findstring $(strip $(PAGE_SIZE_$*)),"A5"),\
	  $(JMK_ROOT)/pkg/doc/scripts/print_a5.xsl,\
	  $(JMK_ROOT)/pkg/doc/scripts/print.xsl) $<
	
$(XML_ONLINE) : %.xml.online : %.xml $(XML_DEP)
	$(XSLTPROC) --xinclude --output $@ \
	  $(JMK_ROOT)/pkg/doc/scripts/jmacro.xsl $<
	$(XSLTPROC) --output $@ runtime_profile.xsl $@
	
$(XML_HTML) : html_% : html_profile_% %.xml.online 
	$(XSLTPROC) --output $@/ \
	  $(JMK_ROOT)/pkg/doc/scripts/html_chunk.xsl $(word 2,$^)
	@$(MKDIR) $@/images
	$(RG_SHELL_FUNCS) && cpy_imgs $@ $(JMK_ROOT)

$(XML_HTML_WEB) : %.html : html_profile_% %.xml.online 
	@$(MKDIR) $@/images
	$(XSLTPROC) --output \
	  $@/$@ $(JMK_ROOT)/pkg/doc/scripts/html.xsl $(word 2,$^)
	$(RG_SHELL_FUNCS) && cpy_imgs $@ $(JMK_ROOT)

$(XML_RTF) : %.rtf : print_profile_% %.fo
	$(FOP) -fo $(filter-out print_profile_%,$^) -rtf  $@

# $(UM_IMG_LINK) and $(PG_IMG_LINK) should hold a full-path target filenames
$(UM_IMG_LINK): $(IMG_DIR)
	$(JMKE_LN) $(JMK_ROOT)/pkg/doc/user_manual/images/$(notdir $@) $@

$(PG_IMG_LINK): $(IMG_DIR)
	$(JMKE_LN) $(JMK_ROOT)/pkg/doc/programmer_guide/images/$(notdir $@) $@

$(IMG_DIR):
	$(MKDIR) $@

.PHONY: $(ARCH_DEPENDENT_PNGS)
$(ARCH_DEPENDENT_PNGS) : $(JMKE_PWD_BUILD)/images/%.png : \
  $(JMKE_PWD_SRC)/images/%$(BOARD_SUFFIX).png
	$(JMKE_LN) $< $@

$(UM_XML_LINK):
	$(JMKE_LN) $(JMK_ROOT)/pkg/doc/user_manual/$@ ./

$(PG_XML_LINK):
	$(JMKE_LN) $(JMK_ROOT)/pkg/doc/programmer_guide/$@ ./

version.ent: $(JMK_ROOT)/pkg/include/rg_version_data.h
	$(JMK_ROOT)/pkg/doc/scripts/j_version.awk < $< > $@
	@echo -e "<!ENTITY j_rgdocver \"&j_rgversiondotless;-`date -u "+%G%m%d-%H%M%S"`\">" >> $@
	
html_profile_%: module_list
	$(JMK_ROOT)/pkg/doc/scripts/jprofile.sh \
	  $(if $(strip $(DOC_PROFILE_$*)),$(DOC_PROFILE_$*),$(DOC_PROFILE)) \
	  'html' > runtime_profile.xsl

print_profile_%: module_list 
	$(JMK_ROOT)/pkg/doc/scripts/jprofile.sh \
	  $(if $(strip $(DOC_PROFILE_$*)),$(DOC_PROFILE_$*),$(DOC_PROFILE)) \
	  'print' > runtime_profile.xsl

module_list:
# module_list.sh is being included in jprofile.sh that creates
# runtime_profile.xsl
	$(shell cat $(JMK_ROOT)/build/rg_config.mk| \
	  grep "^\(MODULE_RG\|CONFIG_\).*=y" > module_list.sh)
# Checks if all CONFIG|MODULE flags are valid using createconfig -f
	$(shell grep "condition=" *.xml|grep -v REMARK| \
	  cut -d= -f2-3|cut -d= -f2| \
	  cut -d\" -f2|xargs echo|sed -e "s/;/ /g"|sed -e "s/ / -f /g"| \
	  xargs $(JMKE_BUILDDIR)/pkg/build/create_config -f 2>&1| grep ERROR)

