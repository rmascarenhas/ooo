/* allok is a memory allocator that may be used to replace malloc(3)
   in a program, for example. However, it's much, much simpler than malloc(3),
   and was written *just* for learning purposes.

Usage:

    void *allok(size_t size)
    Allocates +size+ bytes of memory, returning a pointer to the base address.
    Returns NULL in failure.

    void freek(void *ptr)
    Frees the memory pointed to by +ptr+ *which must be a pointer returned by a 
    previous call to allok*.
*/

#define _BSD_SOURCE
#include <unistd.h>

#include <stdio.h>

/*
   allok behaves similarly with malloc(3). It mantains a free list of memory
   that can be reused in future calls of allok. When it is necessary to expand
   the program break, it is done by an amount bigger than the requested size,
   avoiding unecessary system calls.

   Implementation details:
   allok uses a doubly linked list to find the next block of free memory available.
   Also, the first bytes of memory contains the size of the block of memory.
   allok uses a first fit algorithm to find a free block that is big enough for
   the request. This avoids the problem of having many small free blocks of memory
   that are hard to be reused, which is typical of best fit algorithms.

   The program break is only shrinked when there is at least ALOK_MAX_FREE_MEM
   joint bytes bellow the program break.


   Free memory list:

                 ,-----------------------------------,
                 v                                   | -
   free_list -> |L|P|N|     | |-------------------|L|P|N|          | ...
                   - |                             ^ 
                     `----------------------------'

    L: length of the block of memory
    P: pointer to the previous free block
    N: pointer to the next block    
    -: NULL pointer
*/

#define ALOK_MAX_FREE_MEM 128 * 1024 /* 128 kB */
#define ALLOK_GROWTH_BASE        1024

void *
allok(size_t size) {


/* shrinks the program break */
#define shrink() \
    allocated_size = size + ALLOK_GROWTH_BASE * ++resizes; \
    free_list = sbrk(size + ALLOK_GROWTH_BASE * ++resizes); \
    /* could not change program break */ \
    if (free_list == (void *) -1) { \
        return NULL; \
    } \
    /* store the block size in the first bytes */ \
    b_siz = (intptr_t *) free_list; \
    *b_siz = allocated_size - sizeof(intptr_t);


    static void *free_list = NULL;
    static int resizes     = 0; /* how many times the program break was increased */

    intptr_t allocated_size; /* amount of memory increased in program break */
    intptr_t *b_siz;         /* size, in bytes, of a block of memory */
    char *base;              /* base address, used to calculate size in bytes */

    /* initial memory allocation */
    if (free_list == NULL) {
        shrink();

        /* there are no previous or next blocks in this case */
        void **ptrs = (void **) ++b_siz;
        *ptrs++ = NULL;
        *ptrs   = NULL;
    }

    /* find first fit that is big enough for the request */
    intptr_t *it;
    for (it = (intptr_t *) free_list; 
            it != NULL && *it < size; 
            it = (intptr_t *) *(((void **) ++it)+1)
    );

    /* No big enough block of free memory */
    if (it == NULL) {
        /* shrink program break */
        void *previous_fl = free_list;
        shrink();
        *b_siz = size;

        base = (char *) free_list;
        free_list = (void *) (base + sizeof(intptr_t) + size);

        b_siz = (intptr_t *) free_list;
        *b_siz = allocated_size - size - 2*sizeof(intptr_t);

        /* previous pointer is NULL; next is old free list */
        void **ptrs = (void **) ++b_siz;
        *ptrs++ = NULL;
        *ptrs   = previous_fl;

        b_siz = (intptr_t *) base;

        return (void *) ++b_siz; /* memory right after the block size information */

    } else  {
        /* there is a big enough block of memory and it is pointed to by +it+. */

        void **previous_block = (void **) it+1;
        void **next_block = previous_block+1;

        /* an exact match */
        if (*it == size) {
            if (*previous_block == NULL) {
                /* update free_list */
                free_list = *(((void **) it+1) +1);
            } else {
                /* adjust +N+ field */
                previous_block = (void **) (((intptr_t *) *previous_block)+1) +1;
                *previous_block = (void *) (((void **) (it+1))+1);
            }

            return (void *) it+1; /* memory right after the block size information */
        }

        /* block size is bigger than requested size */

        printf("Free block size: %d\n", *it);

        /* Write meta information in the remaining memory */
        base = (char *) it;
        base += sizeof(intptr_t) + size;
        b_siz = (intptr_t *) base;
        *b_siz = *it - size - sizeof(intptr_t);

        /* Update free_list pointer */
        free_list = (void *) base;

        /* restore pointers previous and next free blocks */
        void **ptrs = (void **) ++b_siz;
        *ptrs++ = *previous_block;
        *ptrs   = *next_block;

        /* Write meta information in the returning block */
        *it = size;
        return (void *) ++it; /* Block of memory after size information */
    }
}



/*********************************************************************
 ********************************* TEST ******************************
 *********************************************************************/

//#include<malloc.h>

static const int N_TESTS = 1024;

int
main() {
    int **v = (int **) allok(N_TESTS*sizeof(int *));;

    int i;
    for (i = 0; i < N_TESTS; ++i) {
        v[i] = (int *) allok(sizeof(int));
        *(v[i]) = i;
    }

    for (i = 0; i < N_TESTS; ++i) {
        printf("%d\n", *(v[i]));
    }

    return 0;
}
