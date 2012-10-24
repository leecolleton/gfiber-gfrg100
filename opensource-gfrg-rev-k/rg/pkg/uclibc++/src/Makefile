JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_SUBDIRS+=abi/libsupc

# cxa_demangle.o comes from Infineon toolchain, but is not really needed
JMK_L_OBJS+=$(filter-out $(wildcard abi/libsupc/cxa_demangle.o),\
  $(wildcard abi/libsupc/*.o))

ifneq ($(shell $(CC) -print-file-name=libgcc_eh.a),libgcc_eh.a)
JMK_SUBDIRS+=abi/libgcc_eh
JMK_L_OBJS+=$(wildcard  abi/libgcc_eh/*.o)
endif

ifdef CONFIG_DYN_LINK
JMK_SO_TARGET+=libuClibc++.so
JMK_EXPORT_LIBS+=libuClibc++.so
JMK_RAMDISK_LIB_FILES+=libuClibc++.so
else
JMK_A_TARGET+=libuClibc++.a
JMK_EXPORT_LIBS+=libuClibc++.a
JMK_RAMDISK_LIB_FILES+=libuClibc++.a
endif

JMK_O_OBJS+=algorithm.o del_opv.o iomanip.o locale.o numeric.o stdexcept.o \
  bitset.o del_opvnt.o ios.o map.o ostream.o streambuf.o char_traits.o \
  deque.o iostream.o new_op.o queue.o string.o complex.o exception.o istream.o \
  new_opnt.o set.o utility.o del_op.o fstream.o iterator.o new_opv.o sstream.o \
  valarray.o del_opnt.o func_exception.o list.o new_opvnt.o stack.o vector.o \
  new_handler.o abi/abi.o


include $(JMKE_MK)
