# uClibc dlopen tests
# Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.

# rules need a little love to work with glibc ...
export UCLIBC_ONLY := 1

TESTS := dltest dltest2 dlstatic test1 test2 test3 dlundef dlafk

include ../Test.mak

JMK_CFLAGS_dltest    := -DLIBNAME="\"./libtest.so\""
JMK_CFLAGS_dltest2   := -DLIBNAME="\"./libtest3.so\""

JMK_LDFLAGS_dlstatic := -ldl
JMK_LDFLAGS_dltest   := -ldl -lpthread
JMK_LDFLAGS_dltest2  := -ldl -lpthread
JMK_LDFLAGS_dlundef  := -ldl
JMK_LDFLAGS_dlafk    := -ldl ./libafk.so -Wl,-rpath,.
JMK_LDFLAGS_test1    := -ldl
JMK_LDFLAGS_test2    := -ldl
JMK_LDFLAGS_test3    := -ldl ./libtest1.so ./libtest2.so -Wl,-rpath,.

DEBUG_LIBS := X
WRAPPER := env $(DEBUG_LIBS)=all LD_LIBRARY_PATH="$$PWD:.:$(LD_LIBRARY_PATH)"

dltest: libtest.so
dltest2: libtest3.so
dlstatic: libstatic.so
dlundef: libundef.so
dlafk: libafk.so
libafk.so: libafk-temp.so
JMK_LDFLAGS_libafk.so := ./libafk-temp.so -Wl,-rpath,.
test1: libtest1.so
test2: libtest1.so libtest2.so
test3: libtest1.so libtest2.so
libtest1.so: libtest2.so
JMK_LDFLAGS_libtest1.so := ./libtest2.so -Wl,-rpath,.
JMK_LDFLAGS_libtest2.so := -Wl,-rpath,.
JMK_LDFLAGS_libtest3.so := -lpthread -Wl,-rpath,.
