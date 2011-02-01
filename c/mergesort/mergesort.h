/*
 * mergesort.h
 * Jim M | Feb 2011 | GPL
 */ 

#include <stdio.h>
#include "jims_utilities.h"

// On entry, in1[0,...,(n1-1)] and in2[0,...,(n2-1)] are already sorted.
// This routine copies and sorts them into out[0,...,(n1+n2-1)], using the
// move1 and move2 macros which increment the indices and copy an int. */
void merge(int n1, int in1[], int n2, int in2[], int out[]);

// On entry, list[n] is a list; scratch[n] is extra space.
// On exit, the entries in list[n] are sorted.
void mergesort(int n, int list[], int scratch[]);
