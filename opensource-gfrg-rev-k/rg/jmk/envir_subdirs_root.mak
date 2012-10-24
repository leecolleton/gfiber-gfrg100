# internal functions for implementing PATH_UP
PWD_REL_LIST=$(subst /,! ,$1)
PWD_REL_NUM=$(words $(call PWD_REL_LIST,$1))
PWD_REL_MINUS_ONE=$(words $(wordlist 2,$(call PWD_REL_NUM,$1),$(call PWD_REL_LIST,$1)))
PATH_UP_REMOVE_LAST=$(wordlist 1,$(call PWD_REL_MINUS_ONE,$1),$(call PWD_REL_LIST,$1))

# usage: "$(call PATH_UP,a/b/c/d)" gives "a/b/c"
PATH_UP=$(subst !,,$(subst ! ,/,$(call PATH_UP_REMOVE_LAST,$1)))

# PWD_REL=pkg/util --> PATH_UP_START=./pkg/util --> ./pkg/util ./pkg .
# PWD_REL=. --> PATH_UP_START=./ --> .
PATH_UP_RECURSIVE=$(strip $(if $1,$(call PATH_UP_RECURSIVE,$(call PATH_UP,$1)),) $1)
PATH_UP_LIST=$(call PATH_UP_RECURSIVE,$(PATH_UP_START))
