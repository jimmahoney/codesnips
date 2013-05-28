/*
 * n_dls.c
 *
 * Arbitray n diagonal latin square problem; 
 * see ./notes.txt
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queens.h"
#include "dancing_links.h"
#include "jims_utils.h"

#define PRINT_NQUEEN_DIAGONALS 0
#define PRINT_BOARDS 0

void print_an_answer(solutions answer, boards diags, int which){
  int row, col, i, soln;
  board b;
  int n = diags->first->n;
  char symbol;
  printf("  +");
  for (col = 0; col < n; col++) printf("--");
  printf("-+\n");
  // printf("  |");
  // for (col=0; col<n; col++) printf("  ");  
  // printf(" |\n");
  for (row = 0; row < n; row++){
    printf("  |");
    for (col = 0; col < n; col++){
      for (i = 0; i < n; i++){
	soln = answer->solns[which]->rows[i]; // which solution in diags
	b = nth_board(diags, soln);
	if ((b->queens)[row]==col){
	  // choose symbols so that first row is (A B C ...)
	  symbol = (char) ((b->queens)[0] + (int)'A');
	  printf(" %c", symbol);
	}
      }
    }
    printf(" |\n");
  }
  printf("  +");
  for (col = 0; col < n; col++) printf("--");
  printf("-+\n\n");
}

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
  int* matrix = _malloc(n_rows * n_cols * sizeof(int));  
  board b = diags->first;
  for (row = 0; row < n_rows; row++){
    for (col = 0; col < n_cols; col++){
      q_row = col / n;
      q_col = col % n;
      *(matrix + row * n_cols + col) = ((b->queens)[q_row] == q_col ? 1 : 0);
      // With the funkier declarations of matrix, that would be :
      //   matrix[row][col] = ((b->queens)[q_row] == q_col ? 1 : 0);
    }
    b = b->next;
  }
  return matrix;
}

void print_answer_perm(solutions answer, boards diags, int which){
  int i, i_row;
  printf("\n  -- %i'th answer rows -- \n", which);
  for (i=0; i < answer->solns[0]->i_rows ; i++){
    i_row = answer->solns[which]->rows[i]; 
    printf("  %4i ", i_row);
    print_board_as_perm(nth_board(diags, i_row));
  }
}

int main() {

  int low =   2;     // search limits
  int high =  11;

  int i, j, n, n_diags_cols, n_diags_rows;
  int* matrix = NULL;
  boards diags = NULL;
  solutions answer = NULL;

  reset_timer();
  for (n = low; n <= high; n++){
    printf(" n=%i ", n);  fflush(stdout);
    diags = queens_search_diagonals(n);
    printf("nqueens_diagonal=%i ", diags->count);  fflush(stdout);
    if (diags->count > n){
      n_diags_cols = pow(diags->first->n, 2);
      n_diags_rows = diags->count;
      matrix = make_matrix(diags);
      // printf("\n === enter dancing_links == \n"); fflush(stdout);
      answer = dancing_links(n_diags_rows, n_diags_cols, matrix, 0);
      // printf(" === leave dancing_links ==\n"); fflush(stdout);
      printf("solns=%i \n", answer->found);  fflush(stdout);
      if (PRINT_NQUEEN_DIAGONALS){ 
	print_boards_as_perms(diags);
	printf("\n");
      }
      if (answer->found > 0){
	for (i = 0; i < answer->found; i++){
	  if (PRINT_NQUEEN_DIAGONALS){
	    printf("  ("); // print which diagonal solutions are in this answer
	    for (j= 0; j < n; j++) printf("%i ", answer->solns[i]->rows[j]);
	    printf(")\n");
	  }
	  if (PRINT_BOARDS) print_an_answer(answer, diags, i);
	}
      }
      _free(matrix);
      free_solutions(answer);
    }
    else {
      printf("\n");
    }
    free_boards(diags);
  }
  printf(" elapsed time ");
  print_elapsed();
  printf("\n");
  return 0;
}
