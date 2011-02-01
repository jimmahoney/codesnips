/*
 * test jims_utilities
 */

#include "jims_utilities.h"
#include <stdio.h>

#define SIZE 10

int main(){

  int a = 10;
  int b = 13;
  int tmp;
  
  int list[SIZE];
  float seconds;

  reset_timer();
  printf(" -- test_utils -- \n");

  printf(" 1. (a,b) = (%i, %i)\n", a, b);
  swap(a, b, tmp);
  printf("    after swap : (%i, %i)\n", a, b);

  random_ints(SIZE, list);
  printf(" 2. %i random ints : ", SIZE);
  print_array(SIZE, list);

  seconds = elapsed_time();
  printf(" 3. seconds elapsed = %f \n", seconds);

  return 0;
}
