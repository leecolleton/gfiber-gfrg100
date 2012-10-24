# Generic utils
SYSTEM_MAP=$(JMKE_BUILDDIR)/os/linux/System.map
GET_SYMBOL=$(shell grep -w $1 $(SYSTEM_MAP) | awk '{ printf "0x%s", $$1 }')

# For U-Boot archs. If some arch need a special args, it can override it.
# NOTE: Currently we are not using mkimage. Using simple binary and 'go' instead
# of bootm in U-Boot.
MKIMG_ARGS = -n 'OpenRG' -A $(ARCH) -O linux -T kernel -C none -a $(LOADADDR) \
  -e $(LOADADDR) -d $< $@
MKIMG = $(JMKE_BUILDDIR)/pkg/boot/u-boot/mkimage $(MKIMG_ARGS)
