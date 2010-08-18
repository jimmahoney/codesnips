/*
 * test.c
 *
 * see ./README
 */ 

#include <stdio.h>
#include "permute.h"

void print_permutation(int n, int *permutation) {
  int i;
  for (i = 0; i < n; i++)
    printf("%d", permutation[i]);
  printf("\n");
}

int main() {
  printf("==== permute(3, &print_permutation) ====\n");
  permute(3, &print_permutation);

  return 0;
}

