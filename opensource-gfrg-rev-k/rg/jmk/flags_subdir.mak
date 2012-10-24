ifeq ($(JMK_WARN2ERR),)
  JMK_WARN2ERR=y
endif

# JMK_MOD_CFLAGS is used both in _mod_[24|26].o and module.o
ifeq ($(JMK_WARN2ERR),y)
  JMK_CFLAGS+=-Werror
  JMK_LOCAL_CFLAGS+=-Werror
  JMK_MOD_CFLAGS+=-Werror
endif

# this works around many places in rg tree which contain this line:
# #include <file.h> 
# where file.h is in the current directory (the correct way is to use "", 
# and then there's no need for this JMK_CFLAGS fix)
JMK_CFLAGS+=-I$(JMKE_PWD_SRC) -I$(JMKE_PWD_BUILD)
JMK_LOCAL_CFLAGS+=-I$(JMKE_PWD_SRC) -I$(JMKE_PWD_BUILD)
JMK_MOD_CFLAGS+=-I$(JMKE_PWD_SRC) -I$(JMKE_PWD_BUILD)

JMK_RG_LINUX_CFLAGS+=$(JMK_LINUX_CFLAGS)
