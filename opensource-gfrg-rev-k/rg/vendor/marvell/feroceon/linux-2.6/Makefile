ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

KERNEL_BUILD=$(JMKE_BUILDDIR)/os/linux-2.6/
KERNEL_SRC=$(JMK_ROOT)/os/linux-2.6/

JMK_LINKS=$(addprefix $(KERNEL_BUILD)/,\
	arch/arm/mach-feroceon-kw2 \
	arch/arm/lib \
	arch/arm/mm \
	arch/arm/plat-feroceon \
	include \
	$(if $(CONFIG_MV_INCLUDE_USB),drivers/usb/host))

JMK_SUBDIRS+=$(if $(CONFIG_MV_CESA_OCF),crypto/ocf)

JMK_INTERNAL_INCLUDES=arch/arm/mm/proc-macros.S 

JMK_ARCHCONFIG_FIRST_TASKS+=$(JMK_LINKS) $(JMK_INTERNAL_INCLUDES)

DST_PATH=$(JMKE_BUILDDIR)/os/linux-2.6/
MV_DIR=vendor/marvell/feroceon/linux-2.6

$(JMK_LINKS):
	$(JMKE_MKDIR) $(dir $@)
	$(JMKE_CP_LN) $(JMKE_PWD_SRC)/$(MV_DIR)/$(patsubst $(KERNEL_BUILD)%,%,$@) $@

$(JMK_INTERNAL_INCLUDES):
	$(JMKE_CP_LN) $(KERNEL_SRC)/$@ $(KERNEL_BUILD)/$@

$(call JMKE_INCLUDE_RULES)
