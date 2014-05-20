ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_SUBDIRS+=lib

ifndef CONFIG_RG_GPL

ifneq ($(CONFIG_RG_WBM)$(CONFIG_RG_SSI_PAGES)$(CONFIG_RG_JNET_SERVER)),)
ifdef ACTIONTEC_NEW_GUI_GOOGLE
  JMK_SUBDIRS+=images_google cgi
else
  JMK_SUBDIRS+=images cgi
endif
endif

ifeq ($(CONFIG_RG_SSI_PAGES),y)
  JMK_SUBDIRS+=html_pages rg_cgi
endif
ifeq ($(CONFIG_RG_SSI),y)
  JMK_SUBDIRS+=ssi
endif

endif #CONFIG_RG_GPL

$(call JMKE_INCLUDE_RULES)
