
/*
 * the TR problem.
 */ 

#include <stdio.h>
#include "queens.h"
#include "dancing_links.h"

int main() {
  int n=8;

  boards solns = queens_search(n);
  printf("--- Found %i solutions to the %i-queens problem.\n", solns->count, n);
  printf("--- Here's the first :\n");
  print_board(solns->first);
  printf("--- Same board as a permutation (i.e. column indeces by rows) :\n");
  print_board_as_perm(solns->first);

  printf("--- Number of queens in each square over all solutions :\n");
  print_queens_per_square(solns);

  boards diags = in_both_diagonals(solns);
  printf("--- %i solutions with queens in both diagonals :\n", diags->count);
  print_boards_as_perms(diags);

  printf("\n");
  return 0;
}
