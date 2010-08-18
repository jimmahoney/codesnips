/*
 * test.c    v1.01
 *
 * Three test cases for dancing_links.h and dancing_links.c .
 * 
 * For compilation instructions, see ./README  and ./Makefile .
 *
 * Copyright (c) 2010 Jim Mahoney, Marlboro College (mahoney@marlboro.edu).
 * This is open source software under the terms of the MIT License 
 * as given at http://www.opensource.org/licenses/mit-license.php .
 */

#include <stdio.h>
#include "dancing_links.h"

/*
 * The first sample problem in data1 is 
 * from http://en.wikipedia.org/wiki/Algorithm_X
 *           0      1      2       3       4       5       6
 *      +-----------------------------------------------------
 *   0  |    1      0       0      1       0       0       1
 *   1  |    1      0       0      1       0       0       0
 *   2  |    0      0       0      1       1       0       1
 *   3  |    0      0       1      0       1       1       0
 *   4  |    0      1       1      0       0       1       1
 *   5  |    0      1       0      0       0       0       1
 *  solutions = {1,               // solutions->n   = how many solutions
 *                   {3,          // solution->n    = how many rows in this one
 *                       {1,3,5}  // solution->rows = row indices of this one
 *                   }}
 * Explicitly, here's the solution with a single 1 in each column.
 *   1  |    1      0       0      1       0       0       0
 *   3  |    0      0       1      0       1       1       0
 *   5  |    0      1       0      0       0       0       1
 */

#define n_rows 6
#define n_cols 7
int data1[n_rows][n_cols] = { {1, 0, 0, 1, 0, 0, 1},
 			      {1, 0, 0, 1, 0, 0, 0},
			      {0, 0, 0, 1, 1, 0, 1},
			      {0, 0, 1, 0, 1, 1, 0},
			      {0, 1, 1, 0, 0, 1, 1},
			      {0, 1, 0, 0, 0, 0, 1} };

/*
 * The second even smaller sample problem is
 *           0      1      2 
 *      +--------------------
 *   0  |    1      1      0 
 *   1  |    1      0      1
 *   2  |    0      1      0 
 *   3  |    0      0      1 
 * solutions = {2,              // 2 solutions:
 *                 {2, {0, 3}}, //   first has 2 rows : 0 and 3
 *                 {2, {1, 2}}, //   second has 2 rows: 1 and 2
 *             }
 */
int data2[] = {1,1,0,  1,0,1,  0,1,0,  0,0,1};  // nrows=4, ncols=3


int main(){
  solutions s;
  printf("-- dancing links simple samples ; see code for details. -- \n\n");

  printf("test case 1 - all solutions \n");
  print_data((int*)data1, n_rows, n_cols);
  s = dancing_links(n_rows, n_cols, (int*)data1, 0);
  print_solutions(s);
  printf("\n");

  printf("test case 2 - 1st solution \n");
  print_data(data2, 4, 3);
  s = dancing_links(4, 3, data2, 1);
  print_solutions(s);
  printf("\n");

  printf("test case 2 - up to 3 solutions \n");
  s = dancing_links(4, 3, data2, 3);
  print_solutions(s);
  printf("\n");

  return 0;
}
