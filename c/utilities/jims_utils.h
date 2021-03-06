/*
 * jims_utils.h
 *
 * see ./README
 */ 

#ifndef H_JIMS_UTILS
#define H_JIMS_UTILS

#define swap(a, b, tmp)  tmp=a, a=b, b=tmp

// replacement for C's malloc()
//   adds simple return value check and (somewhat) graceful error exit.
void* _malloc(size_t bytes);
void* _calloc(size_t num, size_t size);

// replacement for C's free()
//   currently identical to original.
//   (If _malloc() eventually keeps track of memory, this may do more.)
void _free(void* ptr);

// count memory allocation calls, i.e. ( _malloc()'s + _calloc()'s - _free()'s )
int get_allocation_count();

void print_array(int n, int array[]);   // array[n] as e.g. "[1, 2, 4,]\n"

void reset_timer();
float elapsed_time();                   // seconds since last reset_timer()
void print_elapsed();                   // print elapsed_time() as "d:hh:mm:ss.sss"

int random_int(int min, int max);       // min <= random_int <= max
void random_ints(int n, int numbers[]); // integers 1..n in random order.

#endif

