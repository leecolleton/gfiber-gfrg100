# DBGFR: print a function arguments, and return value
# $1 - function body 
# $2 - function name
# $3 - original arg 1
# $4 - original arg 2
# $5 - original arg 3
ifneq ($(MAKEDEBUG),)
  DBGFR=$(warning $2: ENTER:$$1:[$3] $$2:[$4] $$3:[$5])$(strip $1)$(warning $2 : RETURN [$(strip $1)])
else
  DBGFR=$(strip $1)
endif

# define debug variables for the jmk logic.
# NOTE:
# When adding debug code (and please do), all code that for debug, should be:
#   ${if ${debug_xxx},DEBUG CODE}
# Make sure that it always with ${} and not $() to make reading easier.
# For some reason, make does not always like the ${ }, seems like make bug.
# At least make sure that the debug variable is with { }.
# E.g:
#   $(if ${debug_XXX},$(warning ...))

# debug_subdirs=y
# debug_create_config=y
# debug_libs_a_to_so=y
# debug_ramdisk=y
