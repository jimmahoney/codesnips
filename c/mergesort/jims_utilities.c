/*
 * jims_utilities.c ; see jims_utilities.h
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "jims_utilities.h"

clock_t ticks;
int test_output = 0;  // 1 => print debug output

void reset_timer(){
  ticks = clock();
}

float elapsed_time(){
  return (clock() - ticks) * 1.0 / CLOCKS_PER_SEC;
}

void print_array(int n, int array[]){
  // Print array[0,...(n-1)], followed by a newline.
  int i;
  printf("[");
  for (i=0; i<n; i++){
    printf("%i, ", array[i]);
  }
  printf("]\n");
}

int random_int(int min, int max){
  // Return a random integer i such that min <= i <= max.
  int result = min + (rand() % (max - min + 1));
  if (test_output) printf("   random_int : %i \n", result);
  return result;
}

void swap_list(int i, int j, int numbers[]){
  int tmp;
  tmp = numbers[i];
  numbers[i] = numbers[j];
  numbers[j] = tmp;
}

void random_ints(int n, int numbers[]){
  // Put the numbers 1..n into numbers[],
  // then shuffle them with many random exchanges.
  int i,j, p,q;
  for (i=0; i<n; i++){            // First put 1..n into numbers[]
    numbers[i] = i+1;
  }
  if (test_output) {
    printf(" shuffle before : ");
    print_array(n, numbers);
  }
  for (j=0; j<n*100; j++){        // Then swap many random pairs
    p = random_int(0, n-1);
    q = random_int(0, n-1);
    if (p != q){
      swap_list(p, q, numbers);
    }
  }
  if (test_output) {
    printf(" shuffle after : ");
    print_array(n, numbers);
  }
}
