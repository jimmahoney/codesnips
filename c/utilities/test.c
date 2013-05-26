/*
 * test.c
 *
 * see ./README
 */ 

#include <stdio.h>
#include <limits.h>
#include "jims_utils.h"

#define SIZE 10

void array_test(unsigned long bytes){
  int* array = NULL;
  printf(" trying to allocate space for %lu integers ...\n", bytes);
  array = _malloc(bytes * sizeof(int));
  array[0] = 10;
  array[bytes - 1] = 20;
  if (array[0] == 10 && array[bytes - 1] == 20){
    printf("   ... array[%lu] at %p looks OK \n", bytes, array);
  }
  _free(array);
}

int main() {
  unsigned long size = 8;

  int a = 10;
  int b = 13;
  int tmp;
  
  int list[SIZE];
  float seconds;

  printf("==== testing jims_utils.c ====\n");

  reset_timer();

  printf(" 1. (a,b) = (%i, %i)\n", a, b);
  swap(a, b, tmp);
  printf("    after swap : (%i, %i)\n", a, b);

  random_ints(SIZE, list);
  printf(" 2. %i random ints : ", SIZE);
  print_array(SIZE, list);

  seconds = elapsed_time();
  printf(" 3. seconds elapsed = %f \n", seconds);

  while (size < ULONG_MAX){
    // Keep asking for bigger chunks of memory until something breaks.
    array_test(size);
    size <<= 6;
  }
  // We shouldn't get here; _malloc() should quit 
  // when too much memory is requested.
  printf("Done.\n");
  return 0;
}

