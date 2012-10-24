#
# linux/arch/arm/lib/Makefile
#
# Copyright (C) 1995-2000 Russell King
#

lib-y		:= backtrace.o changebit.o csumipv6.o csumpartial.o   \
		   csumpartialcopy.o csumpartialcopyuser.o clearbit.o \
		   delay.o findbit.o memchr.o memcpy.o		      \
		   memmove.o memset.o memzero.o setbit.o              \
		   strncpy_from_user.o strnlen_user.o                 \
		   strchr.o strrchr.o                                 \
		   testchangebit.o testclearbit.o testsetbit.o        \
		   lib1funcs.o div64.o sha1.o               \
		   io-readsb.o io-writesb.o io-readsl.o io-writesl.o

ifneq ($(CONFIG_AVALANCHE_COMMON)$(CONFIG_FEROCEON),)
# XXX: This is an assembly version of this the files from newer version of
# linux kernel.
# It does not work (currently) to our current arm 2.6 boards so insted of
# replacing the .C with .S, keep the 2 versions.
# It seems that a newer version of kernel will fix it and the _ files can be
# removed.
  lib-y += _lshrdi3.o _muldi3.o _ucmpdi2.o _ashldi3.o _ashrdi3.o
else
  lib-y += lshrdi3.o muldi3.o ucmpdi2.o ashldi3.o ashrdi3.o
endif

mmu-y	:= clear_user.o copy_page.o getuser.o putuser.o

# the code in uaccess.S is not preemption safe and
# probably faster on ARMv3 only
ifeq ($(CONFIG_PREEMPT),y)
  mmu-y	+= copy_from_user.o copy_to_user.o
else
ifneq ($(CONFIG_CPU_32v3),y)
  mmu-y	+= copy_from_user.o copy_to_user.o
else
  mmu-y	+= uaccess.o
endif
endif

ifeq ($(CONFIG_COPY_USER_MEMCPY),y)
  mmu-y	+= uaccess_memcpy.o
endif

lib-$(CONFIG_MMU) += $(mmu-y)

ifeq ($(CONFIG_CPU_32v3),y)
  lib-y	+= io-readsw-armv3.o io-writesw-armv3.o
else
  lib-y	+= io-readsw-armv4.o io-writesw-armv4.o
endif

lib-$(CONFIG_ARCH_RPC)		+= ecard.o io-acorn.o floppydma.o
lib-$(CONFIG_ARCH_CLPS7500)	+= io-acorn.o
lib-$(CONFIG_ARCH_L7200)	+= io-acorn.o
lib-$(CONFIG_ARCH_SHARK)	+= io-shark.o

$(obj)/csumpartialcopy.o:	$(obj)/csumpartialcopygeneric.S
$(obj)/csumpartialcopyuser.o:	$(obj)/csumpartialcopygeneric.S
