/*
 * time_merge.c
 * stats for timing of mergesort on random lists
 *
 * Here's the output when run on my 2.66GHz intel core i7 Macbook Pro:
 *
 *   $ time ./time_meg
 *           n        secs   n log2(n)       ratio 
 *       -----   ---------   ---------   --------- 
 *          10   0.0000009   3.322e+01   3.730e+07 
 *         100   0.0000057   6.644e+02   1.165e+08 
 *        1000   0.0000714   9.966e+03   1.395e+08 
 *       10000   0.0009541   1.329e+05   1.393e+08 
 *      100000   0.0118117   1.661e+06   1.406e+08 
 *    
 *    real	0m3.898s
 *    user	0m3.861s
 *    sys	0m0.029s
 *
 * So this C code mergesort on 100,000 integers (0.4MB of memory) takes 0.01 sec.
 * The fairly fairly constant (n log(n))/secs ratio shows that this O(n log(n))
 *
 * Jim M | Feb 2011 | GPL
 */

#include <stdio.h>
#include <math.h>
#include "mergesort.h"

#define MAXSIZE 100000
int list[MAXSIZE];
int scratch[MAXSIZE];

float time_random_sort(int n, int repititions){
  // Return time in seconds it takes to mergesort a random array of
  // given size, averaged over the given number of repititions.
  float seconds = 0.0;
  int reps;
  if (n > MAXSIZE){
    printf(" ERROR in time_random_sort : %i is too big.\n", n);
    return(-1.0);
  }
  for (reps=0; reps < repititions; reps++){
    random_ints(n, list);
    reset_timer();
    mergesort(n, list, scratch);
    seconds += elapsed_time();
  }
  return seconds/repititions;
}

int main(){
  int runs   = 5;
  int ns[]   = {10,   1e2, 1e3, 1e4, 1e5};   // list sizes
  int reps[] = {3e4, 3e3, 3e2,   3e1, 3};     // number of repititions
  float seconds, nlogn;
  char* formatstrings = "  %6s  %10s  %10s  %10s \n";
  char* formatnumbers = "  %6i  %10.7f  %10.3e  %10.3e \n";
  int i;
  printf(formatstrings, "n", "secs", "n log2(n)", "ratio");
  printf(formatstrings, "-----", "---------", "---------", "---------");
  for (i=0; i < runs; i++){
    seconds = time_random_sort(ns[i], reps[i]);
    nlogn = ns[i] * log(ns[i]*1.0)/log(2.0);
    printf(formatnumbers, ns[i], seconds, nlogn, nlogn/seconds);
  }
  return 0;
}
