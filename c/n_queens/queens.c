/*
 *  See queens.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "queens.h"
#include "permute.h"

// Return true if there's a queen at the given row and column.
#define queen_at(queens, row, column) ((queens)[(row)] == (column))

int is_queen(board b, int row, int column){
  return queen_at(b->queens, row, column);
}

board new_board(int n){
  board b = (board) malloc(sizeof(struct _board));
  int* queens = (int*) malloc(n*sizeof(int));
  int i;
  for (i=0; i<n; i++) queens[i] = 0;
  b->n = n;
  b->queens = queens;
  b->next = NULL;
  return b;
}

// void queens2int(board b){
//   int j;
//   int m = 1;
//   b->queensint = 0;
//   for (j=0; j < b->n; j++){
//     b->queensint += b->queens[j] * m;
//     m *= b->n;
//   }
// }

board new_board_from_queens(int n, int* queens){
  board b = new_board(n);
  int i;
  for (i=0; i<n; i++) b->queens[i] = queens[i];
  return b;
}

boards new_boards(){
  boards bs = (boards) malloc(sizeof(struct _boards));
  bs->count = 0;
  bs->first = NULL;
  bs->last = NULL;
  return bs;
}

void append(boards bs, int n, int* queens){
  board b = new_board_from_queens(n, queens);
  if (bs->count == 0){
    bs->first = b;
    bs->last = b;
  }
  else {
    bs->last->next = b;
  }
  bs->last = b;
  bs->count++;
}

// Return false (i.e. 0) if there are at least two queens
// on the diagonal which starts at [row,column] and 
// continues along the [delta_row, delta_column] direction;
// otherwise return true (i.e. 1).
int ok_diagonal(int n, int* queens, 
		int row, int column,
		int delta_row, int delta_column){
  int queens_seen = 0;
  do {
    if (queen_at(queens, row, column)){
      if (queens_seen > 0) 
	return 0;
      queens_seen++;
    }
    row += delta_row;
    column += delta_column;
  } while (row >= 0 && row < n && column >= 0 && column < n);
  return 1;
}

// Return true (i.e. 1) if all diagonals on queens are OK,
// that is, have at most one queen.  
int valid_diagonals(int n, int* queens){
  int row, col;
  for (row = 1; row < n; row++){                    // start left, sloping up
    if (!ok_diagonal(n, queens, row, 0, -1, 1))
      return 0;
  }
  for (col = 1; col < n-1; col++){                  // start bottom, sloping up
    if (!ok_diagonal(n, queens, n-1, col, -1, 1))
      return 0;
  }
  for (row = 0; row < n-1; row++){                  // start left, sloping down
    if (!ok_diagonal(n, queens, row, 0, 1, 1))
      return 0;
  }
  for (col = 1; col < n-1; col++){                  // start top, sloping down
    if (!ok_diagonal(n, queens, 0, col, 1, 1))
      return 0;
  }
  return 1;
}

boards in_both_diagonals(boards solutions){
  boards diags = new_boards();
  int* in_diag1;
  int* in_diag2;
  board b;
  int col, i, j_soln;
  int n = solutions->first->n;
  in_diag1 = (int*) malloc(sizeof(int) * solutions->count);
  in_diag2 = (int*) malloc(sizeof(int) * solutions->count);
  for (i=0; i < n; i++){
    in_diag1[i] = 0;
    in_diag2[i] = 0;
  }
  for (col=0; col < n; col++){
    b = solutions->first;
    j_soln = 0;
    while (b != NULL){
      if (is_queen(b, col, col)) in_diag1[j_soln] = 1;
      if (is_queen(b, n-col-1, col)) in_diag2[j_soln] = 1;
      b = b->next;
      j_soln++;
    }
  }
  b = solutions->first;
  j_soln = 0;
  while (b != NULL){
    if (in_diag1[j_soln] && in_diag2[j_soln]) append(diags, b->n, b->queens);
    b = b->next;
    j_soln++;
  }
  free(in_diag1);
  free(in_diag2);
  return diags;
}


void print_board(board b){
  int row, column;
  int* queens = b->queens;
  int n = b->n;
  for (row = 0; row < n; row++){
    printf("  ");
    for (column = 0; column < queens[row]; column++) 
      printf(" . ");
    printf(" Q ");
    for (column = queens[row]+1; column < n; column++) 
      printf(" . ");
    printf("\n");
  }
  printf("\n");
}

void print_boards(boards bs){
  board b;
  printf("%i boards :\n", bs->count);
  if (bs->count > 0){
    b = bs->first;
    while (b != NULL){
      printf(" ==\n");
      print_board(b);
      b = b->next;
    }
  }
}

board nth_board(boards bs, int n){
  // return n'th board from list
  board b = bs->first;
  int i = 0;
  while (i < n){
    b = b->next;
    i++;
  }
  return b;
}

void print_board_as_perm(board b){
  int i;
  int n = b->n;
  printf(" [%i", b->queens[0]);
  for (i=1; i <b->n; i++){
    printf(",%i", b->queens[i]);
  }
  printf("]\n");
}

void print_boards_as_perms(boards bs){
  board b = bs->first;
  int i = 0;
  while (b != NULL){
    printf(" %3i ", i);
    print_board_as_perm(b);
    b = b->next;
    i++;
  }
}

boards solutions;
int n_permutations;
void check_permutation(int n, int* queens){
  n_permutations++;
  // printf(" . %i . \n", n_checked);
  if (valid_diagonals(n, queens)) {
    // printf(" solution found \n");
    append(solutions, n, queens);
  }
}

void print_queens_per_square(boards solutions){
  board b;
  int i,j;
  int n = solutions->first->n;
  int totals[8][8];
  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
      totals[i][j] = 0;
    }
  }
  // printf("working ");
  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
      // printf("."); fflush(stdout);
      b = solutions->first;
      while (b != NULL){
	if (is_queen(b, i, j)) totals[i][j]++;
	b = b->next;
      }
    }
  }
  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
      printf(" %4i", totals[i][j]);
    }
    printf("\n");
  }
}

boards queens_search(int n){
  n_permutations = 0;
  solutions = new_boards();
  permute(n, &check_permutation);
  // printf("searched %i permutations.\n", n_permutations);
  return solutions;
}

