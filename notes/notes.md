# Tp notes

## Goal
- Remake
  - malloc : allocate heap memory and return a pointer
  - free : remove that allocated memory

## Marquage des blocs

- initOfZone
    - taille -> 8o
    - magic -> 8o
- finalOfZone
    - taille -> 8o
    - magic -> 8o
- taille = initOfZone + [utilisateur + delta] + finalOfZone

## Calcul magic:

## mem small

- so what was important to know here
- For writting memory addresses u need to do so
- in a void* variable.
- Is important to know that the compiler need
   need to know how many bytes it needs to consider
   that's why casting is important
- One of my mistakes was thinking that the
    arena.chunkpool has a pointer to the head
    but is the head arena.chunkpool so mistake by my side

## Mem medium

- so basically needed to check again buddy algo
- As we are using an allocation method by subdivision
   so we have an array an each i element of the array has a linked list with blocks
   of size i
- [!image](./img/mem_medium.png)

- So now what is needed to be done is understand the buddy algo, make an sketch in code 
- Could be with python and then try to put everything together
- Really helpfull to understand buddy algo link: https://www.youtube.com/watch?v=-kmjlWRXkT4&t=924s

