JMK_ROOT=../../
include $(JMK_ROOT)/envir.mak

LIB_IW=libiw.a

JMK_A_TARGET=$(LIB_IW)

JMK_TARGET=iwconfig iwlist iwpriv iwspy iwgetid iwevent ifrename
JMK_RAMDISK_BIN_FILES+=$(JMK_TARGET)

JMK_O_OBJS_$(LIB_IW)+=iwlib.o

JMK_O_OBJS_iwconfig=iwconfig.o
JMK_O_OBJS_iwlist+=iwlist.o
JMK_O_OBJS_iwpriv+=iwpriv.o
JMK_O_OBJS_iwspy+=iwspy.o
JMK_O_OBJS_iwgetid+=iwgetid.o
JMK_O_OBJS_iwevent+=iwevent.o
JMK_O_OBJS_ifrename+=ifrename.o

JMK_LDFLAGS+=-L$(JMKE_BUILDDIR)/pkg/wireless_tools -liw -lm 

JMK_EXPORT_LIBS=$(JMK_SO_TARGET)

include $(JMKE_MK)
