JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_TARGET=excepttest algotest bitsettest dequetest fstreamtest  \
  listtest maptest mmaptest newdel numerictest settest sstreamtest \
  stacktest streambuftest streamitertest stringtest vectortest io

$(foreach t, $(JMK_TARGET), $(eval JMK_O_OBJS_$t:=$(t:%=%.o)))

JMK_LDFLAGS+=-L$(JMKE_BUILDDIR)/pkg/uclibc++/src -luClibc++ -ldl

JMK_CFLAGS=-I../include -I$(JMKE_BUILDDIR) -g

include $(JMKE_MK)
