/*
 * randomlist.c : see randomlist.h
 */

#include <stdlib.h>
#define swap(a, b, tmp)  tmp=a, a=b, b=tmp

int random_int(int min, int max){
  // Return a random integer i such that min <= i <= max.
  int result = min + (rand() % (max - min + 1));
  if (test_output) printf("   random_int : %i \n", result);
  return result;
}

void randomlist(int n, int numbers[]){
  // Put the numbers 1..n into numbers[],
  // then shuffle them with many random exchanges.
  int i,j, p,q;
  for (i=0; i<n; i++){            // First put 1..n into numbers[]
    numbers[i] = i+1;
  }
  if (test_output) {
    printf(" shuffle before : ");
    print_list(n, numbers);
  }
  for (j=0; j<n*100; j++){        // Then swap many random pairs
    p = random_int(0, n-1);
    q = random_int(0, n-1);
    if (p != q){
      swap(p, q, numbers);
    }
  }
  if (test_output) {
    printf(" shuffle after : ");
    print_list(n, numbers);
  }
}

