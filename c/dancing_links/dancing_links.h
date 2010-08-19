
#ifndef H_DANCING_LINKS
#define H_DANCING_LINKS

#include <stdio.h>
#include <stdlib.h>

/*
 * dancing_links.h    v1.02
 *
 * An interface to a dancing links implementation.
 *
 * For compilation notes and a simple test case, see README and test.c .
 * For implementation notes see dancing_links.c .
 * 
 *
 * Copyright (c) 2010 Jim Mahoney, Marlboro College (mahoney@marlboro.edu).
 * This is open source software under the terms of the MIT License 
 * as given at http://www.opensource.org/licenses/mit-license.php .
 *
 */

#define DEFAULT_MAX_SOLUTIONS 1000

typedef struct _solution *solution;
struct _solution {
  int i_rows;         // number of rows in this solution
  int* rows;          // rows[0..i_rows-1] are row indices (0..n_rows-1)
};

typedef struct _solutions *solutions;
struct _solutions {
  int n_rows;         // number of rows in original matrix = max solution size
  int i_solns;        // number of solutions found = index of current partial
  int max_solns;      // maximum number of solutions requested
  solution* solns;    // (0..i_solns-1) complete; solns[i_solns] in progress
};

// Solve a "complete cover" problem using the dancing links algorithm.
// input: 
//   n_row, n_cols        : data dimensions
//   data[n_rows][n_cols] : matrix of 0's and 1's defining exact cover problem
//   max_solns              maximum number of solutions to return;
//                          1 => first; 0 => all up to DEFAULT_MAX_SOLUTIONS
solutions dancing_links(int n_rows, int n_cols, int* data, int max_solns);

// Prints total number of solitions found,
// and one line of row indices for each solution.
void print_solutions(solutions s);

// Print matrix of 1's and 0'2 from data[n_rows][n_cols]
void print_data(int* data, int Nrows, int Ncols);

#endif
