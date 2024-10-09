/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include "mem.h"
#include "mem_internals.h"
#include <assert.h>

// typedef struct _MemArena {
//     void *chunkpool;
//     void *TZL[TZL_SIZE];
//     int small_next_exponant;
//     int medium_next_exponant;
// } MemArena;
//
// unsigned long mem_realloc_small() {
//   assert(arena.chunkpool == 0);
//   unsigned long size = (FIRST_ALLOC_SMALL << arena.small_next_exponant);
//   arena.chunkpool = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
//                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//   if (arena.chunkpool == MAP_FAILED)
//     handle_fatalError("small realloc");
//   arena.small_next_exponant++;
//   return size;
// }
void *emalloc_small(unsigned long size) {
  /* ecrire votre code ici */
  // sizeof(unsigned long) == 8
  // CHUNKSIZE = 96
  printf("\nentering to emalloc_small\n");
  unsigned long *tmpArenaChunkpool = (unsigned long *)arena.chunkpool;
  unsigned long *freeNode;
  if (arena.chunkpool != NULL) {
    printf("chunk is not empty");
    freeNode = tmpArenaChunkpool;
    arena.chunkpool = tmpArenaChunkpool + CHUNKSIZE / sizeof(unsigned long *);
    return (unsigned long *)mark_memarea_and_get_user_ptr(freeNode, CHUNKSIZE,
                                                          SMALL_KIND);
  } else {
    unsigned long bloc = mem_realloc_small();
    unsigned long **tmpArenaChunkpool = (unsigned long **)arena.chunkpool;

    unsigned long **HeadTmpArenaChunkpool = tmpArenaChunkpool + 12;
    unsigned long **antHeadTmpArenaChunkpool = tmpArenaChunkpool;
    for (int i = 0; i < bloc / CHUNKSIZE; i += 12) {
      *antHeadTmpArenaChunkpool = (unsigned long *)HeadTmpArenaChunkpool;
      antHeadTmpArenaChunkpool += i;
      HeadTmpArenaChunkpool += i;
    }
    return antHeadTmpArenaChunkpool;
    // int i = 0;
    // int counter = 0;
    // while (tmpArenaChunkpool != NULL && counter <= bloc / 12) {
    //   tmpArenaChunkpool += 12;
    //   counter++;
    // }
    // while ((unsigned long *)arena.chunkpool != NULL) {
    //   (unsigned long *)arena.chunkpool =
    //       ((unsigned long *)arena.chunkpool) + 12;
    //   counter++;
    // }
    // if (((unsigned long *)arena.chunkpool + 12) == NULL) {
    //   printf("here is deborded chunkpool. ");
    // }
    // printf("\ncounter: %d\n", counter);
    // for (; i < bloc / CHUNKSIZE; i += CHUNKSIZE) {
    //   ;
    // }
    // printf("\n number of chunks %d\n", i);
    // return (void *)0;
  }
}

void efree_small(Alloc a) {
  /* ecrire votre code ici */
  if (arena.chunkpool != NULL) {
    a.ptr = arena.chunkpool;
    arena.chunkpool = a.ptr;
  }
}
