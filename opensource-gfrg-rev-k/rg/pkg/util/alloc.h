/****************************************************************************
 *
 * rg/pkg/util/alloc.h
 * 
 * Copyright (C) Jungo LTD 2004
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02111-1307, USA.
 *
 * Developed by Jungo LTD.
 * Residential Gateway Software Division
 * www.jungo.com
 * info@jungo.com
 */
/* SYNC: rg/pkg/util/alloc.h <-> project/tools/util/alloc.h */

#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdlib.h>

#include "rg_error.h"

#if defined(CONFIG_RG_DBG_ULIBC_MALLOC) && defined(__TARGET__)
/* during memory debug we have to use those defines for prevention "hidding"
 * of the caller. We know that log will not written in error case */
#define malloc_log(level, size) malloc(size)
#define zalloc_log(level, size) zalloc(size)
#define realloc_log(level, p, size) realloc(p, size)
#else

/** Return a pointer to an allocated memory block of 'size' bytes.
 * On failure, NULL is returned and an error message is logged at
 * level 'level'.
 */
void *malloc_log(rg_error_level_t level, size_t size);

/** Return a pointer to an allocated memory block of 'size' bytes and zero
 * them. On failure, NULL is returned and an error message is logged at
 * level 'level'.
 */
void *zalloc_log(rg_error_level_t level, size_t size);

/** Change the size of the memory block pointed by 'p' to 'size' bytes. Return
 * a pointer to the newly allocated memory. On failure, NULL is returned and an
 * error message is logged at level 'level'.
 */
void *realloc_log(rg_error_level_t level, void *p, size_t size);

#endif
/** As malloc_log() with 'level' set to LERR */
static inline void *malloc_l(size_t size)
{
    return malloc_log(LERR, size);
}

/** As malloc_log() with 'level' set to LEXIT (reboots the board or exits
 * the host application).
 */
static inline void *malloc_e(size_t size)
{
    return malloc_log(LEXIT, size);
}

/** Return a pointer to a newly allocated memory block of 'size' bytes.
 * The memory is initialized to zero.
 */
static inline void *zalloc(size_t size)
{
    return calloc(size, 1);
}

/** As zalloc_log() with 'level' set to LERR */
static inline void *zalloc_l(size_t size)
{
    return zalloc_log(LERR, size);
}

/** As zalloc_log() with 'level' set to LEXIT (reboots the board or exits
 * the host application).
 */
static inline void *zalloc_e(size_t size)
{
    return zalloc_log(LEXIT, size);
}

/** As realloc_log() with 'level' set to LERR */
static inline void *realloc_l(void *p, size_t size)
{
    return realloc_log(LERR, p, size);
}

/** As realloc_log() with 'level' set to LEXIT (reboots the board or exits
 * the host application).
 */
static inline void *realloc_e(void *p, size_t size)
{
    return realloc_log(LEXIT, p, size);
}

/** Same as free(). Used to indicate that 'p' may be NULL and that it's OK */
static inline void nfree(void *p)
{
    free(p);
}

/** Free the contents of the pointer 'p' and set it to NULL */
#define ZNFREE(p) do { nfree(p); p = NULL; } while (0)

#endif

