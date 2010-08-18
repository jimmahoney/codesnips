/*
 * Johnson-Trotter permutation generation,
 * based on http://www.dreamincode.net/code/snippet4203.htm
 *
 * see ./README
 */ 

#ifndef H_PERMUTE
#define H_PERMUTE

// Loop over all permutations of integers 0..(n-1), 
// calling func(int n, int* permutation) on each.
void permute(int n, void (*func)());

#endif

