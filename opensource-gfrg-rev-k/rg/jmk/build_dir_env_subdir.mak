# when done converting, JMKE_IS_BUILDDIR will never exist and we can use:
# JMKE_PWD_SRC:=$(CUR_DIR)
# JMKE_PWD_BUILD:=$(CUR_DIR:$(JMK_ROOT)%=$(JMKE_BUILDDIR)%)
# PWD_JPKG:=$(CUR_DIR:$(JMK_ROOT)%=$(JPKG_DIR)%)
export JMKE_PWD_SRC:=$(if $(JMKE_IS_BUILDDIR),$(CUR_DIR:$(JMKE_BUILDDIR)%=$(JMK_ROOT)%),$(CUR_DIR))
export JMKE_PWD_BUILD:=$(if $(JMKE_IS_BUILDDIR),$(CUR_DIR),$(CUR_DIR:$(JMK_ROOT)%=$(JMKE_BUILDDIR)%))
PWD_JPKG:=$(CUR_DIR:$(if $(JMKE_IS_BUILDDIR),$(JMKE_BUILDDIR),$(JMK_ROOT))%=$(JPKG_DIR)%)
PWD_REL:=$(strip $(if $(filter $(CUR_DIR),$(JMKE_BUILDDIR) $(JMK_ROOT)),.,\
  $(subst $(JMK_ROOT)/,,$(subst $(JMKE_BUILDDIR)/,,$(CUR_DIR)))))
