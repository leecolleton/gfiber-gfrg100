PATH_UP_START:=$(if $(filter .,$(PWD_REL)),./,./$(PWD_REL))

# Include envir.subdirs.mak in all the parent directories up to JMK_ROOT
-include $(addsuffix /envir.subdirs.mak,$(addprefix $(JMK_ROOT)/,$(PATH_UP_LIST)))

# We dont define JMK_CFLAGS in rg_config.mk because the kernel does not like it.
# However, JMK_CFLAGS may be defined in envir.subdirs.mak, so append and don't
# override
JMK_CFLAGS+=$(CFLAGS_ENVIR)
