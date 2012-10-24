# internal functions for implementing PATH_UP
PWD_REL_LIST=$(subst /,! ,$1)
PWD_REL_NUM=$(words $(call PWD_REL_LIST,$1))
PWD_REL_MINUS_ONE=$(words $(wordlist 2,$(call PWD_REL_NUM,$1),$(call PWD_REL_LIST,$1)))
PATH_UP_REMOVE_LAST=$(wordlist 1,$(call PWD_REL_MINUS_ONE,$1),$(call PWD_REL_LIST,$1))

# usage: "$(call PATH_UP,a/b/c/d)" gives "a/b/c"
PATH_UP=$(subst !,,$(subst ! ,/,$(call PATH_UP_REMOVE_LAST,$1)))

# PWD_REL=pkg/util --> PATH_UP_START=./pkg/util --> ./pkg/util ./pkg .
# PWD_REL=. --> PATH_UP_START=./ --> .
PATH_UP_START:=$(if $(filter .,$(PWD_REL)),./,./$(PWD_REL))
PATH_UP_RECURSIVE=$(strip $(if $1,$(call PATH_UP_RECURSIVE,$(call PATH_UP,$1)),) $1)
PATH_UP_LIST=$(call PATH_UP_RECURSIVE,$(PATH_UP_START))

# Include envir.subdirs.mak in all the parent directories up to JMK_ROOT
-include $(addsuffix /envir.subdirs.mak,$(addprefix $(JMK_ROOT)/,$(PATH_UP_LIST)))

# We dont define JMK_CFLAGS in rg_config.mk because the kernel does not like it.
# However, JMK_CFLAGS may be defined in envir.subdirs.mak, so append and don't
# override
JMK_CFLAGS+=$(CFLAGS_ENVIR)
JMK_LDFLAGS+=$(LDFLAGS_ENVIR)
