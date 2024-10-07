/******************************************************
 * Copyright Grégory Mounié 2018-2022                 *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include "mem_internals.h"
#include "mem.h"
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
  printf("\n[MARK_MEMARE] DEBUG START");
  printf("\n address pointed by ptr: %p\n", ptr);
  unsigned long *tmpPtr = (unsigned long *)ptr;
  // Left part
  unsigned long *tailleLeft = tmpPtr;
  *tailleLeft = size;
  printf("\nsize == %lu\n", size);
  printf("\nsize inside location == %lu\n", *tailleLeft);
  unsigned long *magicLeft = tmpPtr + 1;
  // getting mmix value
  *magicLeft = knuth_mmix_one_round((unsigned long)tmpPtr);
  // converting last 2 bits to 1
  *magicLeft &= ~0b11UL;
  // doing an and operation to set the k in the last 2 bits setted to 1
  *magicLeft |= k;
  // returned
  unsigned long *utilizableZone = tmpPtr + 2; // 8o taille + 8o magic

  // Right part
  unsigned long *magicRight =
      (tmpPtr + (size / sizeof(unsigned long *))) -
      2; // I know that the size is the end so getting the
         //
  *magicRight = knuth_mmix_one_round((unsigned long)tmpPtr);
  // converting last 2 bits to 1
  *magicRight &= ~0b11UL;
  // doing an and operation to set the k in the last 2 bits setted to 1
  // this is becoming everything more significant = 0 and that's nto what i want
  // i want xyz......tw 111..1...00
  *magicRight |= k;
  // pointer of the last part and sustracting make sense
  unsigned long *tailleRight = magicRight + 1;
  *tailleRight = size;
  return utilizableZone;
  printf("\n[MARK_MEMARE] DEBUG END");
}

// typedef struct _Alloc {
//     void *ptr;
//     MemKind kind;
//     unsigned long size;
// } Alloc;
Alloc mark_check_and_get_alloc(void *ptr) {
  /* ecrire votre code ici */

  printf("\n [MARK_CHECK] DEBUG START");
  Alloc a = {};
  unsigned long *tmpPtr = (unsigned long *)ptr;
  unsigned long *tailleLeft = tmpPtr - 2;
  unsigned long *tailleRight =
      (tmpPtr - 2) + (*tailleLeft / sizeof(unsigned long *)) - 1;
  assert(*tailleLeft == *tailleRight);
  unsigned long *magicLeft = tmpPtr - 1;
  unsigned long *magicRight =
      (tmpPtr - 2) + (*tailleLeft / sizeof(unsigned long *)) - 2;
  assert(*magicLeft == *magicRight);
  a.ptr = tmpPtr - 2;
  a.size = *tailleLeft;
  a.kind = *magicLeft & 0B11UL;
  printf("\nprinting structure here\n ");
  printf("printing structure ptr: %p\n ", a.ptr);
  printf("printing structure size: %d\n ", (int)a.size);
  printf("printing structure kind: %d\n ", a.kind);
  printf("\n [MARK_CHECK] DEBUG FINISH");
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
