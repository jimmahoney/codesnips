
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
 */ 

#ifndef H_QUEENS
#define H_QUEENS

/* a board (and a node in a list of boards) */
typedef struct _board *board;
struct _board {
  int n;         /* board size */
  int* queens;   /* queens[n] are 0..n-1 columns containing queen */
  // int queensint; /* same as integer, for ordering/set stuff */
  board next;
};

/* list of boards, for saving solutions */
typedef struct _boards *boards;
struct _boards {  
  int count;
  board first;
  board last;
};

int valid_diagonals(int n, int* queens);
int is_queen(board b, int row, int column);

board new_board(int n);
board new_board_from_queens(int n, int* queens);
boards new_boards();

void print_board(board b);
void print_board_as_perm(board b);
void print_boards(boards bs);
void print_boards_as_perms(boards bs);

/* return list of all solutions to n-queens problem. */
boards queens_search(int n);

/* print number of solutions with piece in each board cell */
void print_queens_per_square(boards solutions);

/* return subset which have queens on both diagonals. */
boards in_both_diagonals(boards solutions);

#endif
