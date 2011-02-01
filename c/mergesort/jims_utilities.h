/*
 * jims_utilities.h
 *
 * Various utility functions, including 
 * list printing, randoms, and timing.

 * Usage:
 *   --- sample calls in your_program.c ---
 *   include "jims_utilities.h"
 *   reset_timer();
 *   float elapsed = elapsed_time();
 *
 *   --- how to compile & run ---
 *   $ gcc -c jims_utilities.c -o jims_utilities.o
 *   $ gcc your_program.c jims_utilities.o -o your_program
 *   $ ./your_program 

 * Jim Mahoney | Feb 2011 | GPL
 */

#define swap(a, b, tmp)  tmp=a, a=b, b=tmp

void print_array(int n, int array[]);   // array[n] as e.g. "[1, 2, 4,]\n"

void reset_timer();
float elapsed_time();                   // seconds since last reset_timer()

int random_int(int min, int max);       // min <= random_int <= max
void random_ints(int n, int numbers[]); // integers 1..n in random order.

