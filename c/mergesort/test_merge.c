/*
 * test merge.c
 */

#include "mergesort.h"
#include <stdio.h>

int main(){
  int mergein1[] = {1, 3, 6, 8, 9};          // This syntax only works
  int mergein2[] = {2, 3, 7};                // for initializing variables.
  int mergeout[] = {0, 0, 0, 0, 0, 0, 0, 0};

  int mergeshort[] = {3,10};

  int sorttest[] = {2, 1, 5, 4, 3};
  int scratch[]  = {0, 0, 0, 0, 0};

  int sorttest2[] = {2, 1, 5, 4, 3, 20, 22, 50, 1, 18};
  int scratch2[]  = {0, 0, 0, 0, 0, 0,  0,  0,  0,  0};

  printf(" -- test merge() --\n");
  printf(" mergein1 = "); print_array(5, mergein1);
  printf(" mergein2 = "); print_array(3, mergein2);
  merge(5, mergein1, 3, mergein2, mergeout);
  printf(" 1,2 mergeout = "); print_array(8, mergeout);
  merge(3, mergein2, 5, mergein1, mergeout);
  printf(" 2,1 mergeout = "); print_array(8, mergeout);
  merge(1, &mergeshort[0], 1, &mergeshort[1], mergeout);
  printf(" mergeshort = "); print_array(2, mergeout);

  printf(" -- test mergesort() --\n");
  printf(" sorttest = "); print_array(5, sorttest);
  mergesort(5, sorttest, scratch);
  printf(" sorted = "); print_array(5, sorttest);
  printf(" sorttest2 = "); print_array(10, sorttest2);
  mergesort(10, sorttest2, scratch2);
  printf(" sorted = "); print_array(10, sorttest2);

  return 0;
}
