/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include "mem.h"
#include "mem_internals.h"
#include <assert.h>

void *emalloc_small(unsigned long size) {
  if (arena.chunkpool == NULL) {
    // 96o * 128
    // void* is taken as 1byte
    unsigned long bloc = mem_realloc_small();
    unsigned long **tmpHead = arena.chunkpool;
    for (int i = 0; i < (bloc / CHUNKSIZE); i++) {
      *tmpHead = (unsigned long *)tmpHead + 12;
      tmpHead += 12;
    }
    // 1288 / 96 == 128
    //(x + 96 )*128
    //  here the last element has to be in the pos
    //(x + 96 )*128
    //  x == address of arena.chunkpool + bloc
    assert((unsigned long **)arena.chunkpool + 12 * 128 == tmpHead);
  }
  void **firstNode = arena.chunkpool;
  arena.chunkpool = *(void **)firstNode;
  return mark_memarea_and_get_user_ptr(firstNode, CHUNKSIZE, SMALL_KIND);
}
void efree_small(Alloc a) {
  assert(a.ptr != NULL);
  a.ptr = *(void **)arena.chunkpool;
  *(void **)arena.chunkpool = a.ptr;
}
