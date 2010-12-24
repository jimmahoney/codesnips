/*
 *  See ./queens.h and ./README
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
void free_board(board b){
  if (b==NULL) return;
  free(b->queens);
  free(b);
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
void free_boards(boards bs){
  board next;
  if (bs == NULL) return;
  board b = bs->first;
  while (b != NULL){
    next = b->next;
    free_board(b);
    b = next;
  }
  free(bs);
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

int queen_in_1st_diagonal(int n, int* queens){
  /* return true if this board has a queen in the \ main diagonal */
  int col;
  for (col = 0; col < n; col++){
    if (queen_at(queens, col, col)) return 1;
  }
  return 0;
}
int queen_in_2nd_diagonal(int n, int* queens){
  /* return true if this board has a queen in the / secondary diagonal */
  int col;
  for (col = 0; col < n; col++){
    if (queen_at(queens, col, n-col-1)) return 1;
  }
  return 0;
}
boards in_both_diagonals(boards solutions){
  // given a list of boards, return the subset with queens in both diagonals */
  boards diags = new_boards();
  board b = solutions->first;
  int n = b->n;
  while (b != NULL){
    if (queen_in_1st_diagonal(b->n, b->queens) && 
	queen_in_2nd_diagonal(b->n, b->queens))
      append(diags, n, b->queens);
    b = b->next;
  }
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
  if (bs->count == 0){
    printf(" none\n");
  }
  else {
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
}

board nth_board(boards bs, int n){
  // return n'th board from list
  if (bs->count == 0) return (board) NULL;
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
  if (bs->count == 0){
    printf(" none\n");
  }
  else {
    int i = 0;
    board b = bs->first;
    while (b != NULL){
      printf(" %3i ", i);
      print_board_as_perm(b);
      b = b->next;
      i++;
    }
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

// --- The routines below share these variables.

boards solutions;
int n_permutations;

void queens_permutation(int n, int* queens){
  // callback routine for permute()
  n_permutations++;
  // printf(" . %i . \n", n_checked);
  if (valid_diagonals(n, queens)) {
    // printf(" solution found \n");
    append(solutions, n, queens);
  }
}

void queens_diagonal_permutation(int n, int* queens){
  // another callback routine for permute()
  n_permutations++;
  if (valid_diagonals(n, queens) && 
      queen_in_1st_diagonal(n, queens) &&
      queen_in_2nd_diagonal(n, queens)){
    append(solutions, n, queens);
  }
}

boards queens_search_diagonals(int n){
  // return solutions to n-queens with queens on both long diagonals
  solutions = new_boards();
  permute(n, &queens_diagonal_permutation);
  return solutions;
}

boards queens_search(int n){
  // return solutions to n-queens problem.
  n_permutations = 0;
  solutions = new_boards();
  permute(n, &queens_permutation);
  // printf("searched %i permutations.\n", n_permutations);
  return solutions;
}

