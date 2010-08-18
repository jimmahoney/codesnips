
/*
 * Run tests
 */ 

#include <stdio.h>
#include "queens.h"

int main() {
  int good_queens[] = {4, 1, 3, 5, 7, 2, 0, 6};
  int bad_queens[] = {4, 3, 1, 5, 7, 2, 0, 6};
  board good_board = new_board_from_queens(8, good_queens);
  board bad_board = new_board_from_queens(8, bad_queens);
  boards solutions;

  printf("==== print_board(good_board) ====\n");
  print_board(good_board);

  printf("==== valid_diagonals ====\n");
  printf(" good board : %i \n", valid_diagonals(8, good_queens));
  printf(" bad board  : %i \n", valid_diagonals(8, bad_queens));

  printf("== queens_search ====\n");
  printf("n = 4\n");
  solutions = queens_search(4);
  print_boards(solutions);

  return 0;
}

