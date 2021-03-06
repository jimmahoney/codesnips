/*
 * dancing_links.c
 *
 * An implementation in C of Knuth's Dancing Links 
 * approach to Algorithm X for the "exact cover" problem.
 *
 * For compilation and usage notes, see ./README and ./dancing_links.h .
 * For an overview of the algorithm, see en.wikipedia.org/wiki/Dancing_Links .
 *
 * Set DEBUG_PRINT to 1 below to see some diagnostic output,
 * or uncomment some of the tests in main().
 *
 * Copyright (c) 2010 Jim Mahoney, Marlboro College (mahoney@marlboro.edu).
 * This is open source software under the terms of the MIT License 
 * as given at http://www.opensource.org/licenses/mit-license.php .
 */

/**********************************************************
 * implementation notes
 *
 * The "exact cover" starts with a matrix of 1's and 0'2.
 * The task is to select a subset of the rows such that
 * there is exactly one 1 in each column, thus covering
 * the columns exactly.  A number of search problems
 * (including suduko) can be efficiently represented this way.
 *
 * A small example of such a matrix is
 *
 *   row/col 0 1 2
 *         --------
 *     0   | 1 1 0
 *     1   | 1 0 1
 *     2   | 0 1 0
 *     3   | 0 0 1
 *
 * which in C would be
 *
 *   int matrix[4][3] = {{1,1,0},{1,0,1},{0,1,0},{0,0,1}};
 *
 * There are two solutions to this matrix, namely
 *
 *   int solutionA[2] = {0,3}  // row 0 and row 3
 *   int solutionB[2] = {1,2}  // row 1 and row 2
 *
 * The "dancing links" method starts by representing 
 * each 1 in the matrix as a node in a mulitiply linked 
 * grid, leaving out the 0's.  Each node (i.e. 1) is linked 
 * horizontally and vertically to it's nearest neighbors
 * (also 1's), with the links "wrapping around" at the edges.
 *
 * Extra root and column header nodes are added for bookkeeping.
 * For the matrix above, the structure looks something like this:
 *
 *      ^          ^             ^           ^
 *      |          |             |           |
 *   <---root-------header 0------header 1----header 2--->
 *      |          |             |           |
 *   <--------------node 0,0------node 1,0--------------->
 *      |          |             |           |
 *   <--------------node 0,1------------------node 2,1--->
 *      |          |             |           |
 *   <----------------------------node 1,2--------------->
 *      |          |             |           |
 *   <----------------------------------------node 2,3--->
 *      |          |             |           |
 *      v          v             v           v
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "dancing_links.h"
#include "jims_utils.h"

// 1 for diagnostic printing
#define DEBUG_PRINT 0

// -1 for no printing; else print chosen row,col down to given search depth.
#define PRINT_PROGRESS_DEPTH -1

// amount to grow allocated number of solutions when space for more is needed.
#define SOLN_BUFFER_INCREMENT 256

// The nodes fall into three categories :
//   one      root node      horizontal links to headers
//                             headcount=-2; row=n_rows; col=n_cols
//   n_cols   header nodes   vertical links part of column loop; 
//                             headcount is 1's left; row=-1
//   n 1's    matrix nodes     headcount=-1; row,col give coords
typedef struct _node *node;
struct _node {
  node left;     // matrix neighbor (next 1 in exact cover problem)
  node right;    // matrix neighbor
  node up;       // matrix neighbor
  node down;     // matrix neighbor
  node header;   // matrix element's column header 
  int headcount; // number of entries in this column (column headers only)
  int row;       // matrix row index (0 .. max_rows-1)
  int col;       // matrix column index (.. max_cols-1)
  node next;     // linear list (memory management)
  node prev;     // linear list
};

node node_list_head = NULL;  // first in list of all created nodes
node node_list_tail = NULL;  // last in list of all created nodes
int node_list_count = 0;

int is_root(node n){
  return (n->headcount == -2);
}
int is_header(node n){
  return (n->headcount >= 0);
}
int is_matrix(node n){
  return (n->headcount == -1);
}
node new_node(){
  node n = _malloc(sizeof(struct _node));
  n->left = n->right = n->up = n->down = n->next = n->prev = n->header = NULL;
  n->row = n->col = n->headcount = -1;
  if (node_list_head == NULL){
    node_list_head = node_list_tail = n;
  }
  else {
    n->prev = node_list_tail;
    n->prev->next = n;
  }
  node_list_tail = n;
  node_list_count++;
  return n;
}
void print_node_prevnext(char* message, node n){
  printf("%s %p (prev=%p, next=%p, row=%i, col=%i, headcount=%i) \n", 
	 message, n, n->prev, n->next, n->row, n->col, n->headcount);
  fflush(stdout);
}
void print_node(char* message, node n){
  printf("%s %p (row=%i, col=%i, headcount=%i) \n", 
	 message, n, n->row, n->col, n->headcount);
  fflush(stdout);
}
void free_node(node n){
  // This does *not* deal with links in the grid 
  // (which may be funky anyway due to temporary removals);
  // instead, remove_left_right and remove_up_down should 
  // take care of grid links if need be.
  // This only updates the node_list memory management prev/next stuff.
  if (n == NULL) return;
  if (n->prev != NULL) n->prev->next = n->next;
  if (n->next != NULL) n->next->prev = n->prev;
  if (node_list_head == n) node_list_head = n->next;
  if (node_list_tail == n) node_list_tail = n->prev;
  if (node_list_count > 0) node_list_count--;
  _free(n);
}
void free_all_nodes(){
  node n = node_list_head;
  node next;
  while (n != NULL){
    next = n->next;
    if (DEBUG_PRINT){
      printf(" node count is now %i\n", node_list_count);
      print_node_prevnext("  freeing", n);
    }
    free_node(n);
    n = next;
  }
}

void test_node_list(){
  node a, b, c, d;
  printf("  -- testing node list --\n");
  if (node_list_count > 0){
    printf("  Oops : test_node_list should be run before making other nodes.\n");
    return;
  }
  else {
    a = new_node();
    b = new_node();
    c = new_node();
    d = new_node();
    printf("  made 4 nodes\n");
    print_node_prevnext("  a",a); 
    print_node_prevnext("  b",b); 
    print_node_prevnext("  c",c); 
    print_node_prevnext("  d",d);
    free_node(c);
    printf("  freed c\n");
    print_node_prevnext("  a",a); 
    print_node_prevnext("  b",b); 
    print_node_prevnext("  d",d);
    free_node(a);
    printf("  freed a\n");
    print_node("  b", b); 
    print_node("  d", d);
    free_node(d);
    printf("  freed d\n");
    print_node("  b", b);
  }
}

void remove_left_right(node n){
  // Remove a node from its grid row by rearraging left/right links.
  // print_node("  * removing L/R ", n);
  n->left->right = n->right;
  n->right->left = n->left;
}
void replace_left_right(node n){
  // Undo remove_left_right.
  // print_node("  * replacing L/R ", n);
  n->left->right = n;
  n->right->left = n;
}
void remove_up_down(node n){
  // Remove a node from its grid col by rearraging up/down links,
  // and decrement its header's node count.
  // print_node("  * removing U/D ", n);
  n->up->down = n->down;
  n->down->up = n->up;
  n->header->headcount--;
}
void replace_up_down(node n){
  // undo remove_up_down.
  // print_node("  * replacing U/D ", n);
  n->up->down = n;
  n->down->up = n;
  n->header->headcount++;
}

// Create the grid of nodes corresponding to the 1's and 0's in data,
// including root, header, and matrix nodes.  Return its root.
node new_grid(int n_rows, int n_cols, int* data){
  // The approach here is to first, create the entire 
  // n_rows x n_cols grid as if all of data was a 1,
  // then remove all the ones where data is 0.
  node root, h, m, up, down, right, left;
  int row, col;

  // Storage for the nodes while the links between them are being setup.
  // This was on the stack, which caused problems as N got big.
  //  node headers[n_cols];
  //  node matrix[n_rows][n_cols];  // matrix[i][j] => matrix[i * n_cols + j]
  node* headers = _malloc(n_cols * sizeof(node));
  node* matrix = _malloc(n_rows * n_cols * sizeof(node));

  // create root node, vertically linked to itself
  root = new_node();
  root->headcount = -2;
  root->row = n_rows;
  root->col = n_cols;
  root->up = root->down = root;
  if (DEBUG_PRINT) printf(" new_grid: \n");
  // create header nodes with headcount and column index.
  for (col = 0; col < n_cols; col++){
    h = headers[col] = new_node();
    h->col = col;
    h->header = h;         // h is in its own column
    h->headcount = n_rows; // starting with full grid
  }
  if (DEBUG_PRINT) printf(" new_grid: have headers \n");
  // link root horizontally to headers
  root->left = headers[n_cols-1];
  root->right = headers[0];
  // link headers horizontally
  for (col = 0; col < n_cols; col++){
    if (col==0){
      left = root;
    }
    else {
      left = headers[col-1];
    }
    if (col==n_cols-1){
      right = root;
    }
    else {
      right = headers[col+1];
    }
    h = headers[col];
    h->left = left;
    h->right = right;
  }
  if (DEBUG_PRINT) printf(" new_grid: root linked to headers \n");
  // create matrix nodes with headers and indices
  for (col = 0; col < n_cols; col++){
    for (row = 0; row < n_rows; row++){
      m = matrix[row * n_cols + col] = new_node();
      m->row = row;
      m->col = col;
      m->header = headers[col];
    }
  }
  if (DEBUG_PRINT) printf(" new_grid: matrix nodes created \n");
  // link matrix nodes horizontally and vertically; headers at top
  for (col = 0; col < n_cols; col++){
    for (row = 0; row < n_rows; row++){
      if (row == 0){
	up = headers[col];
      }
      else {
	up = matrix[(row-1) * n_cols + col];
      }
      if (row == n_rows-1){
	down = headers[col];
      }
      else {
	down = matrix[(row+1) * n_cols + col];
      }
      m = matrix[row * n_cols + col];
      m->up = up;
      m->down = down;
      m->left = matrix[row * n_cols + ((col+n_cols-1) % n_cols)]; // in C, -1%n is -1.
      m->right = matrix[row * n_cols + ((col+1) % n_cols)];
    }
  }
  // link headers vertically
  for (col = 0; col < n_cols; col++){
    headers[col]->down = matrix[0 * n_cols + col];
    headers[col]->up = matrix[(n_rows-1) * n_cols + col];
  }
  // delete matrix nodes where data is 0
  for (col = 0; col < n_cols; col++){
    for (row = 0; row < n_rows; row++){
      if (*(data + row*n_cols + col) == 0){
	m = matrix[row * n_cols + col];
	// remove it from the grid and throw it away
	remove_left_right(m);
	remove_up_down(m);
	free_node(m);
      }
    }
  }
  _free(headers);
  _free(matrix);
  return root;
}

void print_solutions(solutions ss){
  int i,j;
  solution soln;
  printf("Number of solutions found = %i ; rows :\n", ss->found);
  for (i = 0; i < ss->found; i++){
    soln = ss->solns[i];
    printf(" {%i", soln->rows[0]);
    for (j = 1; j < soln->i_rows; j++){
      printf(", %i", soln->rows[j]);
    }
    printf("}\n");
  }
}

solution new_solution(int n_rows){
  solution s = _malloc(sizeof(struct _solution));
  s->rows = _malloc(n_rows * sizeof(int));
  s->i_rows = 0;
  return s;
}

solution clone_solution(solution s){
  int i;
  solution s_clone = _malloc(sizeof(struct _solution));
  s_clone->i_rows = s->i_rows;
  s_clone->rows = _malloc(s->i_rows * sizeof(int));
  for (i=0; i < s->i_rows; i++) s_clone->rows[i] = s->rows[i];
  return s_clone;
}

void free_solution(solution s){
  _free(s->rows);
  _free(s);
}

solutions new_solutions(int n_rows, int max_solns){
  int i;
  solutions ss = _malloc(sizeof(struct _solutions));
  ss->n_rows = n_rows;
  ss->requested = max_solns;
  ss->found = 0;
  ss->buffer_size = ss->requested==0 ? SOLN_BUFFER_INCREMENT : ss->requested;
  ss->solns = _calloc(ss->buffer_size, sizeof(solution));
  ss->solns[0] = new_solution(ss->n_rows);
  return ss;
}

int found_requested_solutions(solutions ss){
  return ss->requested != 0 && ss->found >= ss->requested;
}

void solution_found(solutions ss){
  // Update solutions by incrementing count of solutions found,
  // setting up space for the solution to be found (if still searching)
  // and allocating more space for buffer if needed.
  int i;
  int old_buffer_size = ss->buffer_size;
  solution* old_solns = ss->solns;
  ss->found++;
  if (found_requested_solutions(ss)) return;
  if (ss->found+1 >= ss->buffer_size) ss->buffer_size += SOLN_BUFFER_INCREMENT;
  if (ss->buffer_size != old_buffer_size){
    // allocate new buffer, copy over old data, delete old buffer
    ss->solns = _calloc(ss->buffer_size, sizeof(solution));
    for (i = 0; i < ss->found; i++) ss->solns[i] = old_solns[i];
    _free(old_solns);
  }
  // Setup next solution.
  ss->solns[ss->found] = clone_solution(ss->solns[ss->found - 1]);
}

void free_solutions(solutions ss){
  int i;
  for (i = 0; i < ss->buffer_size; i++){
    if (ss->solns[i] != NULL){
      free_solution(ss->solns[i]);
    }
  }
  _free(ss->solns);
  _free(ss);
}

void reduce_grid(node n){
  node r, c, o;
  //  Remove header (via left/right links) for node n's column.
  remove_left_right(n->header);
  //
  //  Remove conflicting rows :
  //  For each 1 in n's row, 
  //    Move along each of those columns,
  //      traverse r across to each row with a 1 in that column.
  //      and remove r_i from its column (via top/right links).
  for (o = n->right; o != n; o = o->right){   // 1's not in n's column
    for (c = o->up; c != o; c = c->up){
      if (! is_header(c)){
	for (r = c->right; r != c; r = r->right){
	  remove_up_down(r);
	}
      }
    }
  }
  for (c = n->up; c != n; c = c->up){        // 1 in n's column
    if (! is_header(c)){
      for (r = c->right; r != c; r = r->right){
	remove_up_down(r);
      }
    }
  }
  //  Remove satisfied constraint columns :
  //    For each 1 in n's row
  //      traverse c down to each c_j (including header)
  //      remove row c_j from its row (via left/right links).
  for (r = n->right; r != n; r = r->right){
    for (c = r->up; c != r; c = c->up){
      remove_left_right(c);
    }
  }
}

void undo_reduce_grid(node n){
  node r, c, o;
  // undo the steps in reduce_grid in reverse order
  for (r = n->left; r != n; r = r->left){
    for (c = r->down; c != r; c = c->down){
      replace_left_right(c);
    }
  }
  for (c = n->down; c != n; c = c->down){
    if (! is_header(c)){
      for (r = c->left; r != c; r = r->left){
	replace_up_down(r);
      }
    }
  }
  for (o = n->left; o != n; o = o->left){
    for (c = o->down; c != o; c = c->down){
      if (! is_header(c)){
	for (r = c->left; r != c; r = r->left){
	  replace_up_down(r);
	}
      }
    }
  }
  replace_left_right(n->header);
}

node choose_column(node root){
  // Return head of column with smallest number of nodes
  node h, column;
  int low = root->right->headcount;
  column = root->right;
  if (root->right == root) return root; // no columns available.
  for (h = root->right; h != root; h = h->right){
    if (h->headcount < low){
      low = h->headcount;
      column = h;
    }
  }
  return column;
}

void print_data(int* data, int Nrows, int Ncols){
  int row, col;
  printf("      ");
  for (col = 0; col < Ncols; col++) printf("%3i", col);
  printf("\n");
  printf("     +");
  for (col = 0; col < Ncols; col++) printf("---");
  printf("\n");
  for (row = 0; row < Nrows; row++){
    printf(" %3i | ", row);
    for (col = 0; col < Ncols; col++){
      if (*(data + row*Ncols + col)){
	printf(" 1 ");
      }
      else {
	printf(" 0 ");
      }
    }
    printf("\n");
  }
}

void print_grid(node root){
  // print a grid as a matrix of 1's and 0's, e.g.
  //          0    1    2 
  //       ---------------
  //    0 |   1    1    0  
  //    1 |   1    0    1  
  //    2 |   0    1    0  
  //    3 |   0    0    1  
  //
  int row, col, col_count, i, is_one, one_in_row;
  node h, n;
  col_count = 0;
  printf("      ");
  for (h = root->right; h != root; h = h->right){
    printf("%3i", h->col);
    col_count++;
  }
  printf("\n");
  printf("     +");
  for (i=0; i < col_count; i++) printf("---");
  printf("\n");
  for (row=0; row < root->row; row++){
    one_in_row = 0;
    for (h = root->right; h != root; h = h->right){
      for (n = h->down; n != h; n = n->down){
	if (n->row == row) one_in_row = 1;
      }
    }
    if (one_in_row){
      printf(" %3i | ", row);
      for (h = root->right; h != root; h = h->right){
	is_one = 0;
	for (n = h->down; n != h; n = n->down){
	  if (n->row == row){
	    printf(" 1 ");
	    is_one = 1;
	  }
	}
	if (! is_one) printf(" 0 ");
      }
      printf("\n");
    }
  }
  printf("\n");
}

void grid_analysis(node root){
  node h, m, n;
  int r;
  int in_grid = 0;
  printf("  -- grid analysis --\n");
  printf("  nodes in memory = %i\n", node_list_count); fflush(stdout);
  for (h = root->right; h != root; h = h->right){
    printf("  column  %i has %i down: ", h->col, h->headcount); fflush(stdout);
    in_grid += h->headcount;
    for (m = h->down; m != h; m = m->down) // check links down
      printf("(%i,%i) ", m->row, m->col); fflush(stdout);
    printf("| up: ");
    for (m = h->up; m != h; m = m->up)     // check links up
      printf("(%i,%i) ", m->row, m->col); fflush(stdout);
    m = h->down; 
    printf("\n    (%i,%i) right : ", m->row, m->col); fflush(stdout);
    n = m->right;
    do {                                   // check links right
      if (n->header != h){ 
	printf("(%i,%i) ", n->row, n->col); fflush(stdout);
	n = n->right;
      } 
    } while (m != n);
    printf("| left : ", m->row, m->col); fflush(stdout);
    n = m->left;
    do {                                   // check links left
      if (n->header != h){ 
	printf("(%i,%i) ", n->row, n->col); fflush(stdout);
	n = n->left;
      }
    } while (m != n);
    printf("\n");
  }
  printf("  1s in grid = %i \n\n", in_grid);
}

void search(node root, solutions ss, int depth){
  solution s;
  node n, column;
  int i, solution_row_index;
  if (PRINT_PROGRESS_DEPTH >=0 && depth == 0){
    printf(" - dancing links search - printing (col, row) to depth %i - \n",
	   PRINT_PROGRESS_DEPTH);
  }
  if (DEBUG_PRINT){
    printf(" -- searching at depth=%i --\n", depth);
    print_grid(root);
    fflush(stdout);
  }
  if (root->right == root){
    solution_found(ss);
    if (DEBUG_PRINT) printf(" -- solution found\n");
  }
  else {
    // This is the heart of algorithm : 
    // First, choose a column.
    column = choose_column(root);
    if (column->headcount == 0){
      if (DEBUG_PRINT){
	printf(" -- column %i is all zero; this is a dead end.\n", column->col);
      }
      return;  // current matrix can't be solved
    }
    if (DEBUG_PRINT){
      printf("    -- chose column %i \n", column->col);
      fflush(stdout);
      print_node("    -- column = ", column);
    }
    // Start with its first node
    n = column->down;
    if (DEBUG_PRINT) print_node("    -- first node in column = ", n);
    // For each remaining row in that column,
    // while we're still searching for solutions :
    while (n != column && !found_requested_solutions(ss)){
      // if (DEBUG_PRINT) print_node("    -- trying node ", n);
      if (depth <= PRINT_PROGRESS_DEPTH) {
	printf("    ");
	for (i=0; i<depth; i++) printf("  ");
	printf("(%i, %i)", n->col, n->row);
	if (depth == PRINT_PROGRESS_DEPTH) printf(" ... ");
	printf("\n");
      }
      // Push the row to the current partial solution.
      s = ss->solns[ss->found];
      s->rows[s->i_rows] = n->row;
      s->i_rows++;
      // Remove the selected node from the grid,
      // as well as its row, conflicting rows and satisfied colums.
      reduce_grid(n);
      // progress report
      // Recursively search the smaller grid.
      search(root, ss, depth+1);
      if (!found_requested_solutions(ss)){        // continue looking ?
	// Reverse the grid removals,
	undo_reduce_grid(n);
	// Pull the row from the partial solution.
	s = ss->solns[ss->found];  // (May have changed during search.)
	s->i_rows--;
	// Move on to the next row in this column.
	n = n->down;
      }
    }
  }
}

node new_test_grid(){
  int data[] = {1,1,0,0,1,  1,0,1,0,0,  0,1,0,0,0,  0,0,1,1,0}; // rows=4,cols=5
  //       0 1 2 3 4  col
  // row +---------
  //  0  | 1 1 0 0 1
  //  1  | 1 0 1 0 0
  //  2  | 0 1 0 0 0
  //  3  | 0 0 1 1 0
  return new_grid(4, 5, data);  
}

void test_reduce(){
  node root, n, head;
  solutions solns;
  printf(" -- testing reduce --\n");
  if (node_list_count > 0){
    printf("  Oops : test_reduce should be run before making other nodes.\n");
    return;
  }
  root = new_test_grid();
  grid_analysis(root);
  head = choose_column(root);
  print_node("  -- chose column: ", head);
  n = head->down;
  print_node("  -- reducing grid from ", n);
  reduce_grid(n);
  grid_analysis(root);
  printf("  -- undoing reduce \n");
  undo_reduce_grid(n);
  grid_analysis(root);
}

void test_search(){
  node root = new_test_grid();
  solutions ss = new_solutions(10, 10);
  printf(" -- testing search --\n");
  search(root, ss, 0);
  print_solutions(ss);
  free_solutions(ss);
}

void tests(){
  node root;

  test_reduce();     
  free_all_nodes();

  test_node_list();  
  free_all_nodes();

  root = new_test_grid(); 
  print_grid(root); 
  grid_analysis(root);
  free_all_nodes();

  test_search();

  printf("\n");
}

solutions dancing_links(int n_rows, int n_cols, int* data, int max_solns){
  if (DEBUG_PRINT) printf(" dancing_links: debug \n");
  solutions solns = new_solutions(n_rows, max_solns);
  if (DEBUG_PRINT) printf(" dancing_links: have solns \n");
  node root = new_grid(n_rows, n_cols, data);
  if (DEBUG_PRINT) printf(" dancing_links: have grid \n");
  search(root, solns, 0);
  if (DEBUG_PRINT) printf(" dancing_links: finished search \n");
  free_all_nodes(); 
  return solns;
}
