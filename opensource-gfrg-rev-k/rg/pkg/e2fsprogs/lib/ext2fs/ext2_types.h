#ifndef _EXT2_TYPES_H
#define _EXT2_TYPES_H

/* 
 * If linux/types.h is already been included, assume it has defined
 * everything we need.  (cross fingers)
 */
#ifndef _LINUX_TYPES_H

typedef unsigned char		__u8;
typedef signed char		__s8;
typedef	short			__s16;
typedef	unsigned short		__u16;
typedef	int			__s32;
typedef	unsigned int		__u32;
typedef signed long long 	__s64;
typedef unsigned long long	__u64;

#endif /* LINUX_TYPES_H */

#endif /* EXT2_TYPES_H */
