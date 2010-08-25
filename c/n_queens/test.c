/*
 * test.c
 *
 * Some tests for the n_queens problem.
 * See ./queens.h and ./README for details.
 *
 * == queens_search_diagonals timing ====
 *   n = 10 ; count =   544 ; time = 00:00:00.773
 *   n = 11 ; count =  1744 ; time = 00:00:09.314
 *   n = 12 ; count =  9440 ; time = 00:02:01.546
 *   n = 13 ; count = 52008 ; time = 00:29:49.561  52k*13*(4 bytes) = 2.7MB
 *   n = 14                   estimate about 7 hours (*14) ~ 40MB
 *   n = 15                   estimate about 4 days (*15) ~ 600MB
 *
 * Times are with 2.66GHz Intel Core i7 running Mac OS X 10.6.4 on Aug 2010.
 */ 

#include <stdio.h>
#include <time.h>
#include "queens.h"

clock_t start_time;
void start_timer(){
  start_time = clock();
}
double elapsed_seconds(){
  return ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
}
void print_elapsed(){
  double elapsed = elapsed_seconds();
  int hour = (int) (elapsed / 3600);
  int min = (int) (elapsed - hour*3600)/60;
  double sec = elapsed - hour*3600 - min*60;
  printf("%02i:%02i:%06.3f", hour, min, sec);
}

int main() {
  int good_queens[] = {4, 1, 3, 5, 7, 2, 0, 6};
  int bad_queens[] = {4, 3, 1, 5, 7, 2, 0, 6};
  board good_board = new_board_from_queens(8, good_queens);
  board bad_board = new_board_from_queens(8, bad_queens);
  boards solutions, diagonals;
  int n;

  printf("==== print_board(good_board) ====\n");
  print_board(good_board);

  printf("==== valid_diagonals ====\n");
  printf(" good board : %i \n", valid_diagonals(8, good_queens));
  printf(" bad board  : %i \n", valid_diagonals(8, bad_queens));

  printf("==== small n behavior ====\n");
  for (n=0; n<4; n++){
    printf(" n=%i : ", n); 
    print_boards_as_perms(queens_search(n));
  }

  printf("==== queens_search n=4 ====\n");
  solutions = queens_search(4);
  print_boards(solutions);

  printf("==== search n=8 ====\n");
  solutions = queens_search(8);
  printf(" number of solutions = %i \n", solutions->count);
  diagonals = queens_search_diagonals(8);
  printf(" number of queens_search_diagonals = %i \n", diagonals->count);

  free_boards(diagonals);
  diagonals = in_both_diagonals(solutions);  // same, another way
  printf(" number of in_both_diagonals = %i \n", diagonals->count);

  printf("==== queens_search_diagonals for various n ====\n", n);
  for (n=4; n<11; n++){
    start_timer();
    free_boards(diagonals);
    diagonals = queens_search_diagonals(n);
    printf(" n = %2i ; count = %5i ; time = ", n, diagonals->count);
    print_elapsed(); printf("\n"); fflush(stdout);
  }

  free_boards(solutions);
  free_boards(diagonals);
  return 0;
}

