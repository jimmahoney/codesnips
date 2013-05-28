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

  printf(" malloc'ing space for %lu integers ...\n", bytes);
  array = _malloc(bytes * sizeof(int));
  array[0] = 10;
  array[bytes - 1] = 20;
  if (array[0] == 10 && array[bytes - 1] == 20){
    printf("   ... array[%lu] at %p looks OK \n", bytes, array);
  }
  _free(array);

  printf(" calloc'ing space for %lu integers ...\n", bytes);
  array = _calloc(bytes, sizeof(int));
  if (array[0] == 0 && array[bytes - 1] == 0){
    printf("   ... array[%lu] at %p has 0 first & last \n", bytes, array);
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

  printf(" (a,b) = (%i, %i)\n", a, b);
  swap(a, b, tmp);
  printf("    after swap : (%i, %i)\n", a, b);

  random_ints(SIZE, list);
  printf(" %i random ints : ", SIZE);
  print_array(SIZE, list);

  seconds = elapsed_time();
  printf(" seconds elapsed = %f, i.e. ", seconds);
  print_elapsed();
  printf("\n");

  // Keep asking for bigger chunks of memory until something breaks.
  //   while (size < ULONG_MAX){
  //    array_test(size);
  //    size <<= 6;
  //   }
  // ... but that breaks different ways on different systems,
  // and isn't always caught within _malloc and _calloc.
  //
  // So instead, just test a couple of memory allocations :
  array_test(128);
  array_test(5000);

  printf(" memory check: ");
  printf(" malloc() + calloc() - free() = %i \n", get_allocation_count());

  return 0;
}

