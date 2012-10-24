#ifndef _NTFS_MISC_H_
#define _NTFS_MISC_H_

#ifdef CONFIG_RG_DBG_ULIBC_MALLOC
#define ntfs_calloc(sz) calloc(1,sz)
#define ntfs_malloc(sz) malloc(sz)
#else
void *ntfs_calloc(size_t size);
void *ntfs_malloc(size_t size);
#endif

#endif /* _NTFS_MISC_H_ */

