/*
 * mergesort.h
 * Jim M | Feb 2011 | GPL
 */ 

#include <stdio.h>
#include "jims_utilities.h"

// On entry, in1[n1] and in2[n2] are already sorted.
// On exit, out[n1+n2] has all the numbers in the correct order.
void merge(int n1, int in1[], int n2, int in2[], int out[]);

// On entry, list[n] is a list; scratch[n] is extra space.
// On exit, the integers in list[n] have been sorted.
void mergesort(int n, int list[], int scratch[]);
