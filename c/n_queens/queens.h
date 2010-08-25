
/*
 * n-queens 
 * 
 * Find placement of N queens on NxN chess board
 * so that no two can attack each other.
 *
 * Each board is a permuation of the digits 0..(N-1)
 * giving the column where the queen is in each row.
 *
 * For example, for this solution , with N = 8 , 
 *   columns =  [4, 1, 3, 5, 7, 2, 0, 6]
 * then it looks something like
 * 
 *    .  .  .  .  Q  .  .  .      row 0, queen in column 4
 *    .  Q  .  .  .  .  .  .      row 1  "     "  "      1
 *    .  .  .  Q  .  .  .  .      etc
 *    .  .  .  .  .  Q  .  .      
 *    .  .  .  .  .  .  .  Q      
 *    .  .  Q  .  .  .  .  .      
 *    Q  .  .  .  .  .  .  .      
 *    .  .  .  .  .  .  Q  .  
 *
 * I'm particularly interested in solutions which contain
 * queens along the two long diagonals, so there are also
 * routines here to filter out solutions with that property.
 * 
 */ 

#ifndef H_QUEENS
#define H_QUEENS

/* an n x n board (which is also a node in a list of boards) */
typedef struct _board *board;
struct _board {
  int n;         /* board size */
  int* queens;   /* queens[n] are 0..n-1 = index of column with queen */
  // int queensint; /* queens array as an integer, for ordering/set stuff */
  board next;
};

/* list of boards */
typedef struct _boards *boards;
struct _boards {  
  int count;
  board first;
  board last;
};

int valid_diagonals(int n, int* queens);     /* no queens on same diagonal ? */
int is_queen(board b, int row, int column);  /* queen at row,column ? */

board new_board(int n);                      /* return blank n x n board */
void free_board(board b);                    /* free board's memory */
board new_board_from_queens(int n, int* queens);  /* copy queen locations */
boards new_boards();                         /* return empty list of boards */
void free_boards(boards bs);                 /* free boards and list */
board nth_board(boards bs, int n);           /* return n'th board from list */

void print_board(board b);                   /* print a chess-like board */
void print_boards(boards bs);                
void print_board_as_perm(board b);           /* print as e.g. [1,3,0,2] */
void print_boards_as_perms(boards bs);     

/* return list of all solutions to n-queens problem. */
boards queens_search(int n);

/* return list of sol'ns to n-queens problem including both long diagonals. */
boards queens_search_diagonals(int n);

/* print number of solutions with piece in each board cell */
void print_queens_per_square(boards solutions);

/* return subset which have queens on both diagonals. */
boards in_both_diagonals(boards solutions);

#endif
