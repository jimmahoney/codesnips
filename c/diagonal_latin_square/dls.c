/*
 * dls.c
 *
 * diagonal latin square problem; see ./notes.txt
 * 
 */ 

#include <stdio.h>
#include <math.h>
#include "queens.h"
#include "dancing_links.h"

/* Return matrix[n_rows][n_cols] matrix of 1's and 0's 
   for the exact cover problem.  
     n_cols = 64 : (row=0,col=0) (row=0,col=1) ...
     n_rows = 48 : one for each 8-queens diagonal solution.
   There's a 1 in the matrix if that solution has a queen
   at that row and column.   */
int* make_matrix(boards diags){
  int row, col;              // in exact cover matrix
  int q_row, q_col;          // on N-queens board
  int n_rows = diags->count;
  int n_cols = pow(diags->first->n, 2);
  int n = diags->first->n;
  //  Here I might use a funky multidimensional array declaration.
  //  The compiler needs to know the size across of the matrix, 
  //  but doesn't need to know how many rows.  Thus here we declare 
  //  that matrix is a pointer to array of n_cols ints; see for example 
  //  wikipedia's C_variable_types_and_declarations .
  // int (*matrix)[n_cols] = (int (*)[n_cols]) malloc(n_rows*n_cols*sizeof(int));
  //  But the dancing_links code expects matrix to be just int*, 
  //  and in any case that seems simple and explicit.
  int* matrix = (int *) malloc(n_rows*n_cols*sizeof(int));  
  board b = diags->first;
  for (row = 0; row < n_rows; row++){
    for (col = 0; col < n_cols; col++){
      q_row = col / n;
      q_col = col % n;
      *(matrix + row*n_cols + col) = ((b->queens)[q_row] == q_col ? 1 : 0);
      // With the funkier declarations of matrix, that would be :
      //   matrix[row][col] = ((b->queens)[q_row] == q_col ? 1 : 0);
    }
    b = b->next;
  }
  return matrix;
}

int main() {
  int n = 8;

  boards solns = queens_search(n);
  printf("-- Found %i solutions to the %i-queens problem.\n", solns->count, n);
  printf("-- Here's the first solution:\n");
  print_board(solns->first);
  printf("-- Same one, shown as queen's column index for each row:\n");
  print_board_as_perm(solns->first);

  printf("-- Number of queens in each square over all solutions:\n");
  print_queens_per_square(solns);

  boards diags = in_both_diagonals(solns);
  printf("-- The %i solutions with queens in both diagonals :\n", diags->count);
  print_boards_as_perms(diags);

  printf("-- Queens per square for those diagonal solutions:\n");
  print_queens_per_square(diags);

  int n_diags_cols = pow(diags->first->n, 2);
  int n_diags_rows = diags->count;
  int* matrix = make_matrix(diags);
  //printf("-- The %i by %i exact cover matrix:\n", n_diags_rows, n_diags_cols);
  //print_data(matrix, n_diags_rows, n_diags_cols);

  printf("-- Looking for 1st solution with dancing links algorithm ...\n");
  solutions answer = dancing_links(n_diags_rows, n_diags_cols, matrix, 1);
  printf("-- Result from dancing links is:\n");
  print_solutions(answer);

  printf("\n");
  return 0;
}
