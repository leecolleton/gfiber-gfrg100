/*
  This is a version (aka dlmalloc) of malloc/free/realloc written by
  Doug Lea and released to the public domain.  Use, modify, and
  redistribute this code without permission or acknowledgement in any
  way you wish.  Send questions, comments, complaints, performance
  data, etc to dl@cs.oswego.edu

  VERSION 2.7.2 Sat Aug 17 09:07:30 2002  Doug Lea  (dl at gee)

  Note: There may be an updated version of this malloc obtainable at
           ftp://gee.cs.oswego.edu/pub/misc/malloc.c
  Check before installing!

  Hacked up for uClibc by Erik Andersen <andersen@codepoet.org>
*/

#include "malloc.h"

libc_hidden_proto(munmap)

/* ------------------------- __malloc_trim -------------------------
   __malloc_trim is an inverse of sorts to __malloc_alloc.  It gives memory
   back to the system (via negative arguments to sbrk) if there is unused
   memory at the `high' end of the malloc pool. It is called automatically by
   free() when top space exceeds the trim threshold. It is also called by the
   public malloc_trim routine.  It returns 1 if it actually released any
   memory, else 0.
*/
static int __malloc_trim(size_t pad, mstate av)
{
    long  top_size;        /* Amount of top-most memory */
    long  extra;           /* Amount to release */
    long  released;        /* Amount actually released */
    char* current_brk;     /* address returned by pre-check sbrk call */
    char* new_brk;         /* address returned by post-check sbrk call */
    size_t pagesz;

    pagesz = av->pagesize;
    top_size = chunksize(av->top);

    /* Release in pagesize units, keeping at least one page */
    extra = ((top_size - pad - MINSIZE + (pagesz-1)) / pagesz - 1) * pagesz;

    if (extra > 0) {

	/*
	   Only proceed if end of memory is where we last set it.
	   This avoids problems if there were foreign sbrk calls.
	   */
	current_brk = (char*)(MORECORE(0));
	if (current_brk == (char*)(av->top) + top_size) {

	    /*
	       Attempt to release memory. We ignore MORECORE return value,
	       and instead call again to find out where new end of memory is.
	       This avoids problems if first call releases less than we asked,
	       of if failure somehow altered brk value. (We could still
	       encounter problems if it altered brk in some very bad way,
	       but the only thing we can do is adjust anyway, which will cause
	       some downstream failure.)
	       */

	    MORECORE(-extra);
	    new_brk = (char*)(MORECORE(0));

	    if (new_brk != (char*)MORECORE_FAILURE) {
		released = (long)(current_brk - new_brk);

		if (released != 0) {
		    /* Success. Adjust top. */
		    av->sbrked_mem -= released;
		    set_head(av->top, (top_size - released) | PREV_INUSE);
		    check_malloc_state();
		    return 1;
		}
	    }
	}
    }
    return 0;
}

/* ------------------------- malloc_trim -------------------------
  malloc_trim(size_t pad);

  If possible, gives memory back to the system (via negative
  arguments to sbrk) if there is unused memory at the `high' end of
  the malloc pool. You can call this after freeing large blocks of
  memory to potentially reduce the system-level memory requirements
  of a program. However, it cannot guarantee to reduce memory. Under
  some allocation patterns, some large free blocks of memory will be
  locked between two used chunks, so they cannot be given back to
  the system.

  The `pad' argument to malloc_trim represents the amount of free
  trailing space to leave untrimmed. If this argument is zero,
  only the minimum amount of memory to maintain internal data
  structures will be left (one page or less). Non-zero arguments
  can be supplied to maintain enough trailing space to service
  future expected allocations without having to re-obtain memory
  from the system.

  Malloc_trim returns 1 if it actually released any memory, else 0.
  On systems that do not support "negative sbrks", it will always
  return 0.
*/
int malloc_trim(size_t pad)
{
  mstate av = get_malloc_state();
  __malloc_consolidate(av);
  return __malloc_trim(pad, av);
}

/*
  Initialize a malloc_state struct.

  This is called only from within __malloc_consolidate, which needs
  be called in the same contexts anyway.  It is never called directly
  outside of __malloc_consolidate because some optimizing compilers try
  to inline it at all call points, which turns out not to be an
  optimization at all. (Inlining it in __malloc_consolidate is fine though.)
*/
static void malloc_init_state(mstate av)
{
    int     i;
    mbinptr bin;

    /* Establish circular links for normal bins */
    for (i = 1; i < NBINS; ++i) {
	bin = bin_at(av,i);
	bin->fd = bin->bk = bin;
    }

    av->top_pad        = DEFAULT_TOP_PAD;
    av->n_mmaps_max    = DEFAULT_MMAP_MAX;
    av->mmap_threshold = DEFAULT_MMAP_THRESHOLD;
    av->trim_threshold = DEFAULT_TRIM_THRESHOLD;

#if MORECORE_CONTIGUOUS
    set_contiguous(av);
#else
    set_noncontiguous(av);
#endif


    set_max_fast(av, DEFAULT_MXFAST);

    av->top            = initial_top(av);
    av->pagesize       = malloc_getpagesize;
#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC__
    av->alloc_head.next = av->alloc_head.prev = &av->alloc_head;
#ifdef __CONFIG_RG_DBG_ULIBC_MEM_POISON__
    av->quarantine.next = av->quarantine.prev = &av->quarantine;
#endif
#endif
}


/* ----------------------------------------------------------------------
 *
 * PUBLIC STUFF
 *
 * ----------------------------------------------------------------------*/


/* ------------------------- __malloc_consolidate -------------------------

  __malloc_consolidate is a specialized version of free() that tears
  down chunks held in fastbins.  Free itself cannot be used for this
  purpose since, among other things, it might place chunks back onto
  fastbins.  So, instead, we need to use a minor variant of the same
  code.

  Also, because this routine needs to be called the first time through
  malloc anyway, it turns out to be the perfect place to trigger
  initialization code.
*/
void attribute_hidden __malloc_consolidate(mstate av)
{
    mfastbinptr*    fb;                 /* current fastbin being consolidated */
    mfastbinptr*    maxfb;              /* last fastbin (for loop control) */
    mchunkptr       p;                  /* current chunk being consolidated */
    mchunkptr       nextp;              /* next chunk to consolidate */
    mchunkptr       unsorted_bin;       /* bin header */
    mchunkptr       first_unsorted;     /* chunk to link to */

    /* These have same use as in free() */
    mchunkptr       nextchunk;
    size_t size;
    size_t nextsize;
    size_t prevsize;
    int             nextinuse;
    mchunkptr       bck;
    mchunkptr       fwd;

    /*
       If max_fast is 0, we know that av hasn't
       yet been initialized, in which case do so below
       */

    if (av->max_fast != 0) {
	clear_fastchunks(av);

	unsorted_bin = unsorted_chunks(av);

	/*
	   Remove each chunk from fast bin and consolidate it, placing it
	   then in unsorted bin. Among other reasons for doing this,
	   placing in unsorted bin avoids needing to calculate actual bins
	   until malloc is sure that chunks aren't immediately going to be
	   reused anyway.
	   */

	maxfb = &(av->fastbins[fastbin_index(av->max_fast)]);
	fb = &(av->fastbins[0]);
	do {
	    if ( (p = *fb) != 0) {
		*fb = 0;

		do {
		    check_inuse_chunk(p);
		    nextp = p->fd;

		    /* Slightly streamlined version of consolidation code in free() */
		    size = p->size & ~PREV_INUSE;
		    nextchunk = chunk_at_offset(p, size);
		    nextsize = chunksize(nextchunk);

		    if (!prev_inuse(p)) {
			prevsize = p->prev_size;
			size += prevsize;
			p = chunk_at_offset(p, -((long) prevsize));
			unlink(p, bck, fwd);
		    }

		    if (nextchunk != av->top) {
			nextinuse = inuse_bit_at_offset(nextchunk, nextsize);
			set_head(nextchunk, nextsize);

			if (!nextinuse) {
			    size += nextsize;
			    unlink(nextchunk, bck, fwd);
			}

			first_unsorted = unsorted_bin->fd;
			unsorted_bin->fd = p;
			first_unsorted->bk = p;

			set_head(p, size | PREV_INUSE);
			p->bk = unsorted_bin;
			p->fd = first_unsorted;
			set_foot(p, size);
		    }

		    else {
			size += nextsize;
			set_head(p, size | PREV_INUSE);
			av->top = p;
		    }

		} while ( (p = nextp) != 0);

	    }
	} while (fb++ != maxfb);
    }
    else {
	malloc_init_state(av);
	check_malloc_state();
    }
}

#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC__
#include <sys/param.h>
#define DUMPLINE 32
void __display_chunk(mchunkptr p)
{
    int i;
    jmalloc_header_t *jhead = chunk2mem(p);
    size_t size;
    unsigned char *addr, *end;

    size = chunksize(p);
    malloc_printf("MEMORY: chunk %p size %d (%#x) PREV_INUSE %d "
	"IS_MMAPPED %d\n", p, size, size, prev_inuse(p), chunk_is_mmapped(p));

    malloc_printf("next %p, prev %p, alloc_ra %p, free_ra %p, len %#x(%d), "
	"magic %#x\n", jhead->next, jhead->prev, jhead->alloc_ra,
	jhead->free_ra, jhead->len, jhead->len, jhead->magic);

    jhead++;
    addr = (unsigned char *)jhead;
    /* Decrease chunk size for jhead and for chunk header */
    size -= sizeof(jmalloc_header_t) + 2*sizeof(size_t);
    end = addr + size;

    while (addr < end)
    {
	malloc_printf("%4x: ", addr - (unsigned char *)jhead);

	for (i = 0; i < DUMPLINE; i++)
	{
	    malloc_printf("%02x", addr[i]);
	    if ((i&0x3) == 0x3)
		malloc_printf(" ");
	}

	malloc_printf(" |");
	for (i = 0; i < DUMPLINE; i++)
	    malloc_printf("%c", isprint(addr[i])? addr[i]: '.');
	malloc_printf("|\n");
	addr += DUMPLINE;
    }
}

static int _is_mem_valid(void *mem, void *ra)
{
    jmalloc_header_t *jhead;

    if (mem == NULL)
	return 1;

    jhead = mem;
    jhead--;
    if (jhead->magic == JMALLOC_MAGIC)
    {
	int old_size = jhead->len;
	unsigned char *footer = mem + old_size;

	if (!memcmp(footer, JMALLOC_FOOT, sizeof(JMALLOC_FOOT)))
	    return 1;

	malloc_printf("MEMORY: pid %d - block of %d bytes allocated from %p "
	    "and freed from %p - limits has been exceeded\n", getpid(),
	    old_size, jhead->alloc_ra, ra);

	__display_chunk(mem2chunk(jhead));
    }
    else if (jhead->magic == JFREE_MAGIC)
    {
	malloc_printf("MEMORY: pid %d about to free from %p memory %p "
	    "(%d bytes), allocated from %p and already released from %p",
	    getpid(), ra, mem, jhead->len, jhead->alloc_ra, jhead->free_ra);
    }
    else
    {
	malloc_printf("MEMORY: pid %d about to free garbage %p from %p\n",
	    getpid(), mem, ra);
	__display_chunk(mem2chunk(jhead));
    }
    return 0;
}

int is_mem_valid(void *__ptr)
{
    return _is_mem_valid(__ptr, __builtin_return_address(0));
}

#ifdef __CONFIG_RG_DBG_ULIBC_MEM_POISON__
static inline void* mem_not_chr(unsigned char *addr, unsigned char byte,
    size_t size)
{
    while (size-- && (*addr == byte))
	addr++;

    return addr;
}

static int check_poisoning(void *mem)
{
    jmalloc_header_t *jhead = mem;
    void *end_of_poison;
    int ret = 0, poison_len;
    static int is_recursive;

    /* In case of success we call no "free" from this function. Recursive call
     * may take place only during prining messages about error found. We want to
     * prevent "nested" reports. Let's resolve problems one by one. */
    if (is_recursive)
	return 0;

    is_recursive = 1;

    mem = jhead + 1;

    poison_len = MIN(jhead->len, __CONFIG_RG_DBG_ULIBC_MEM_POISON__);
    end_of_poison = mem_not_chr(mem, 0xeb, poison_len);
    if ((end_of_poison - mem) != poison_len)
    {
	malloc_printf("MEMORY: pid %d - poisoning changed in quarantine\n",
	    getpid());
	__display_chunk(mem2chunk(jhead));

#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC_CRASH__
	/* Let's give to complete the printing */
	sleep(2);
	*((int *)0) = 0xfaceface;
#endif
	ret = -1;
    }
    is_recursive = 0;
    return ret;
}

static void *quarantine_mem(jmalloc_header_t *jhead)
{
    mchunkptr p;
    mstate av;
    size_t size;

    /* We are under lock here */
    av = get_malloc_state();
    p = mem2chunk(jhead);

    /* We are not interested in mmapped chanks, because them are too big for
     * quarantine. More of this - in case of writing there adter unmap we
     * receive error. */
    if (chunk_is_mmapped(p))
	return jhead;
    size = chunksize(p);

    /* Don't save too big slices in quarantine */
    if (size > (64 * 1024))
	return jhead;

    /* Save chunk in head of quarantine */
    jhead->next = av->quarantine.next;
    jhead->next->prev = jhead;
    jhead->prev = &av->quarantine;
    av->quarantine.next = jhead;

    av->n_quarantine++;
    av->quarantine_mem += size;

    /* Quarantine length is 100 (hardcoded). After using in real life we can
     * deside - save this principe or define volume of the quarantine according
     * no memory size. For example - 1M quarantine... */
    if (av->n_quarantine <= 100)
	return NULL;

    jhead = av->quarantine.prev;
    av->quarantine.prev = jhead->prev;
    av->quarantine.prev->next = &av->quarantine;

    av->n_quarantine--;
    av->quarantine_mem -= chunksize(mem2chunk(jhead));

    return jhead;
}

/* Internal function used on exit for validation of memory in quarantine. We
 * want to do it, because it will not checked already in the regular way. */
void __check_quarantine(void)
{
    mstate av;
    jmalloc_header_t *jhead;

    __MALLOC_LOCK;
    av = get_malloc_state();
    jhead = av->quarantine.prev;

    /* If the task never used malloc - quarantine was not initialized */
    if (!jhead)
	goto Exit;

    while (jhead != &av->quarantine)
    {
	/* If we found dirty chank in quarantine - no needs continue check
	 * quarantine. Even if CONFIG_RG_DBG_ULIBC_MALLOC_CRASH was not used -
	 * possible current chunk is already remooved from quarantine. So we
	 * can't continue run on it */
	if (check_poisoning(jhead))
	    break;
	jhead = jhead->prev;

    }
Exit:
    __MALLOC_UNLOCK;
}
#endif
#endif
/* ------------------------------ free ------------------------------ */
void free(void* mem)
{
    mstate av;

    mchunkptr       p;           /* chunk corresponding to mem */
    size_t size;        /* its size */
    mfastbinptr*    fb;          /* associated fastbin */
    mchunkptr       nextchunk;   /* next contiguous chunk */
    size_t nextsize;    /* its size */
    int             nextinuse;   /* true if nextchunk is used */
    size_t prevsize;    /* size of previous contiguous chunk */
    mchunkptr       bck;         /* misc temp for linking */
    mchunkptr       fwd;         /* misc temp for linking */
#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC__
    jmalloc_header_t *jhead;
    void *ra = __builtin_return_address(0);
#endif

    /* free(0) has no effect */
    if (mem == NULL)
	return;

#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC__
    if (!_is_mem_valid(mem, ra))
    {
#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC_CRASH__
	/* Let's give to complete the printing */
	sleep(2);
	*((int *)0) = 0xfaceface;
#endif
	return;
    }

    jhead = mem;
    jhead--;

    jhead->magic = JFREE_MAGIC;
    jhead->free_ra = ra;
#ifdef __CONFIG_RG_DBG_ULIBC_MEM_POISON__
    memset(mem, 0xeb, MIN(jhead->len, __CONFIG_RG_DBG_ULIBC_MEM_POISON__));
#endif
#endif

    __MALLOC_LOCK;
#ifdef __CONFIG_RG_DBG_ULIBC_MALLOC__
    /* Remove chunk from list of allocated chunks */
    jhead->prev->next = jhead->next;
    jhead->next->prev = jhead->prev;

#ifdef __CONFIG_RG_DBG_ULIBC_MEM_POISON__
    /* Put chunk into quarantine. Get discharged from the quarantine, if the
     * quarantine is full already */
    mem = quarantine_mem(jhead);
    if (!mem)
    {
	/* Quarantine is too small. No ills for discharge. */
	goto Exit;
    }
    check_poisoning(mem);
#else
    mem = jhead;
#endif
#endif

    av = get_malloc_state();
    p = mem2chunk(mem);
    size = chunksize(p);

    check_inuse_chunk(p);

    /*
       If eligible, place chunk on a fastbin so it can be found
       and used quickly in malloc.
       */

    if ((unsigned long)(size) <= (unsigned long)(av->max_fast)

#if TRIM_FASTBINS
	    /* If TRIM_FASTBINS set, don't place chunks
	       bordering top into fastbins */
	    && (chunk_at_offset(p, size) != av->top)
#endif
       ) {

	set_fastchunks(av);
	fb = &(av->fastbins[fastbin_index(size)]);
	p->fd = *fb;
	*fb = p;
    }

    /*
       Consolidate other non-mmapped chunks as they arrive.
       */

    else if (!chunk_is_mmapped(p)) {
	set_anychunks(av);

	nextchunk = chunk_at_offset(p, size);
	nextsize = chunksize(nextchunk);

	/* consolidate backward */
	if (!prev_inuse(p)) {
	    prevsize = p->prev_size;
	    size += prevsize;
	    p = chunk_at_offset(p, -((long) prevsize));
	    unlink(p, bck, fwd);
	}

	if (nextchunk != av->top) {
	    /* get and clear inuse bit */
	    nextinuse = inuse_bit_at_offset(nextchunk, nextsize);
	    set_head(nextchunk, nextsize);

	    /* consolidate forward */
	    if (!nextinuse) {
		unlink(nextchunk, bck, fwd);
		size += nextsize;
	    }

	    /*
	       Place the chunk in unsorted chunk list. Chunks are
	       not placed into regular bins until after they have
	       been given one chance to be used in malloc.
	       */

	    bck = unsorted_chunks(av);
	    fwd = bck->fd;
	    p->bk = bck;
	    p->fd = fwd;
	    bck->fd = p;
	    fwd->bk = p;

	    set_head(p, size | PREV_INUSE);
	    set_foot(p, size);

	    check_free_chunk(p);
	}

	/*
	   If the chunk borders the current high end of memory,
	   consolidate into top
	   */

	else {
	    size += nextsize;
	    set_head(p, size | PREV_INUSE);
	    av->top = p;
	    check_chunk(p);
	}

	/*
	   If freeing a large space, consolidate possibly-surrounding
	   chunks. Then, if the total unused topmost memory exceeds trim
	   threshold, ask malloc_trim to reduce top.

	   Unless max_fast is 0, we don't know if there are fastbins
	   bordering top, so we cannot tell for sure whether threshold
	   has been reached unless fastbins are consolidated.  But we
	   don't want to consolidate on each free.  As a compromise,
	   consolidation is performed if FASTBIN_CONSOLIDATION_THRESHOLD
	   is reached.
	   */

	if ((unsigned long)(size) >= FASTBIN_CONSOLIDATION_THRESHOLD) {
	    if (have_fastchunks(av))
		__malloc_consolidate(av);

	    if ((unsigned long)(chunksize(av->top)) >=
		    (unsigned long)(av->trim_threshold))
		__malloc_trim(av->top_pad, av);
	}

    }
    /*
       If the chunk was allocated via mmap, release via munmap()
       Note that if HAVE_MMAP is false but chunk_is_mmapped is
       true, then user must have overwritten memory. There's nothing
       we can do to catch this error unless DEBUG is set, in which case
       check_inuse_chunk (above) will have triggered error.
       */

    else {
	size_t offset = p->prev_size;
	av->n_mmaps--;
	av->mmapped_mem -= (size + offset);
	munmap((char*)p - offset, size + offset);
    }
#ifdef __CONFIG_RG_DBG_ULIBC_MEM_POISON__
Exit:
#endif
    __MALLOC_UNLOCK;
}

