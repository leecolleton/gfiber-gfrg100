#
# Makefile for the linux cramfs routines.
#

JMK_ROOT=../../../../

CFLAGS_inode.o+=-DCRAMFS_TIMESTAMP=$(shell date +%s)

obj-$(CONFIG_CRAMFS) += cramfs.o

cramfs-objs := inode.o uncompress.o

# Always recompile to update cramfs timestamp with every compilation
ifdef JMKE_IS_BUILDDIR
inode.o: dummy
endif

