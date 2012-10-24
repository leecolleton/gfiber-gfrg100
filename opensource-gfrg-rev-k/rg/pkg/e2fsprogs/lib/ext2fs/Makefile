E2FSSRC=../..
include $(E2FSSRC)/envir.mak

JMK_A_TARGET=libext2fs.a

JMK_O_OBJS_libext2fs.a=bb_compat.o cmp_bitmaps.o fileio.o inode_io.o namei.o \
  write_bb_file.o rs_bitmap.o dupfs.o test_io.o imager.o \
  ext2_err.o alloc.o alloc_stats.o alloc_tables.o badblocks.o bb_inode.o \
  bitmaps.o bitops.o block.o bmap.o check_desc.o closefs.o dblist.o \
  dblist_dir.o dirblock.o dirhash.o dir_iterate.o expanddir.o ext_attr.o \
  finddev.o flushb.o freefs.o gen_bitmap.o get_pathname.o getsize.o icount.o \
  initialize.o inline.o inode.o ismounted.o link.o llseek.o lookup.o mkdir.o \
  mkjournal.o native.o newdir.o openfs.o read_bb.o read_bb_file.o \
  rw_bitmaps.o swapfs.o unix_io.o unlink.o valid_blk.o version.o

ERR+=ext2_err.c ext2_err.h
ET=ext2_err.et

include $(E2FSMK)

archconfig:: $(ERR)

$(ERR): $(ET)
	DIR=. ET_DIR=../et ../et/compile_et $<
