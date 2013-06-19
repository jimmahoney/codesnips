/*
 * test.c
 *
 * Some tests for the n_queens problem;
 * see ./queens.h and ./README for the details.
 *
 */ 

#include <stdio.h>
#include "queens.h"
#include "jims_utils.h"

int main() {
  int good_queens[] = {4, 1, 3, 5, 7, 2, 0, 6};
  int bad_queens[] = {4, 3, 1, 5, 7, 2, 0, 6};
  board good_board = new_board_from_queens(8, good_queens);
  board bad_board = new_board_from_queens(8, bad_queens);
  boards solutions, diagonals;
  int n;

  printf("==== print_board(good_board) ====\n");
  print_board(good_board);
  printf("==== print_board_as_perm(good_board) ====\n");
  print_board_as_perm(good_board);
  printf("==== valid_diagonals ====\n");
  printf(" good board : %i \n", valid_diagonals(8, good_queens));
  printf(" bad board  : %i \n", valid_diagonals(8, bad_queens));
  free_board(good_board);
  free_board(bad_board);

  printf("==== small n behavior ====\n");
  for (n=0; n<4; n++){
    printf(" n=%i : ", n); 
    solutions = queens_search(n);
    print_boards_as_perms(solutions);
    free_boards(solutions);
  }

  printf("==== queens_search n=4 ====\n");
  solutions = queens_search(4);
  print_boards(solutions);
  free_boards(solutions);

  printf("==== search n=8 ====\n");
  solutions = queens_search(8);
  printf(" number of solutions = %i \n", solutions->count);
  free_boards(solutions);
  diagonals = queens_search_diagonals(8);
  printf(" number of queens_search_diagonals = %i \n", diagonals->count);
  free_boards(diagonals);

  diagonals = in_both_diagonals(solutions);  // same, another way
  printf(" number of in_both_diagonals = %i \n", diagonals->count);
  free_boards(diagonals);

  printf("==== queens_search_diagonals for various n ====\n", n);
  for (n=4; n<15; n++){
    reset_timer();
    diagonals = queens_search_diagonals(n);
    printf(" n = %2i ; count = %7i ; time = ", n, diagonals->count);
    print_elapsed(); printf("\n"); fflush(stdout);
    free_boards(diagonals);
  }

  printf("=== memory check ===\n");
  printf(" malloc() + calloc() - free() = %i \n\n", get_allocation_count());

  return 0;
}

