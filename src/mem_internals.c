/******************************************************
 * Copyright Grégory Mounié 2018-2022                 *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include "mem_internals.h"
#include "mem.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <sys/mman.h>

unsigned long knuth_mmix_one_round(unsigned long in) {
  return in * 6364136223846793005UL % 1442695040888963407UL;
}

void *mark_memarea_and_get_user_ptr(void *ptr, unsigned long size, MemKind k) {
  /* ecrire votre code ici */
  // void* = sizeof(void*) == 8bytes
  // getting a tmpPointer even though not necessary
  void *tmpPtr = ptr;
  void *tailleLeft = tmpPtr;
  unsigned long tmpPtrAddressValue = *(unsigned long *)tmpPtr;
  *(unsigned long *)tailleLeft = size;
  void *magicLeft = tmpPtr + 1;
  *(unsigned long *)magicLeft =
      set_last_n_bits(knuth_mmix_one_round(tmpPtrAddressValue), 2);
  void *utilizableZone = tmpPtr + 2; // 8o taille + 8o magic
  void *magicRight = (tmpPtr + size / sizeof(void *)) -
                     2; // I know that the size is the end so getting the
                        // pointer of the last part and sustracting make sense
  void *tailleRight = magicRight + 1;
  return utilizableZone;
}

// typedef struct _Alloc {
//     void *ptr;
//     MemKind kind;
//     unsigned long size;
// } Alloc;
Alloc mark_check_and_get_alloc(void *ptr) {
  /* ecrire votre code ici */
  Alloc a = {};

  return a;
}

unsigned long mem_realloc_small() {
  assert(arena.chunkpool == 0);
  unsigned long size = (FIRST_ALLOC_SMALL << arena.small_next_exponant);
  arena.chunkpool = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (arena.chunkpool == MAP_FAILED)
    handle_fatalError("small realloc");
  arena.small_next_exponant++;
  return size;
}

unsigned long mem_realloc_medium() {
  uint32_t indice = FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant;
  assert(arena.TZL[indice] == 0);
  unsigned long size = (FIRST_ALLOC_MEDIUM << arena.medium_next_exponant);
  assert(size == (1UL << indice));
  arena.TZL[indice] = mmap(0,
                           size * 2, // twice the size to allign
                           PROT_READ | PROT_WRITE | PROT_EXEC,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (arena.TZL[indice] == MAP_FAILED)
    handle_fatalError("medium realloc");
  // align allocation to a multiple of the size
  // for buddy algo
  arena.TZL[indice] += (size - (((intptr_t)arena.TZL[indice]) % size));
  arena.medium_next_exponant++;
  return size; // lie on allocation size, but never free
}

// used for test in buddy algo
unsigned int nb_TZL_entries() {
  int nb = 0;

  for (int i = 0; i < TZL_SIZE; i++)
    if (arena.TZL[i])
      nb++;

  return nb;
}
