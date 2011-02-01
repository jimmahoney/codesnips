/*
 * mergesort.c
 * Jim M | Feb 2011 | GPL
 */ 

#include "mergesort.h"

// C has a "comma operator" which makes these macros work as one statement.
// (I've seen some arguments (e.g. wikipedia C preprocessor) 
// that the comma is cleaner than a semicolon, since this
// the macro behaves as expected in an "if" without braces.)
#define move1(from, to) out[to]=in1[from], from++, to++
#define move2(from, to) out[to]=in2[from], from++, to++

void merge(int n1, int in1[], int n2, int in2[], int out[]){
  // On entry, in1[0,...,(n1-1)] and in2[0,...,(n2-1)] are already sorted.
  // This routine copies and sorts them into out[0,...,(n1+n2-1)], using the
  // move1 and move2 macros which increment the indices and copy an int. */
  int i1 = 0; // in1[] index
  int i2 = 0; // in2[] index
  int j = 0;  // out[] index
  //printf("     -- debug merge -- \n");
  //printf("     in1[%i] = ", n1); print_array(n1, in1);
  //printf("     in2[%i] = ", n2); print_array(n2, in2);
  while (i1 < n1 || i2 < n2){
    if (i2 >= n2)                 move1(i1, j);     // in2 finished 
    else if (i1 >= n1)            move2(i2, j);     // in1 finished
    else if (in1[i1] <= in2[i2])  move1(i1, j);     // in1 <=
    else                          move2(i2, j);     // in2 <
  }
  //printf("     out[%i] = ", n1+n2); print_array(n1+n2, out);
}

void mergesort(int n, int list[], int scratch[]){
  // On entry, list[n] is a list; scratch[n] is extra space.
  // On exit, the entries in list[n] are sorted.
  int* listA = list;         // a pointer to a (list of) integers
  int* listB = scratch;      // ditto
  int* listTmp;              // ditto
  int sublistSize = 1;
  int sublistSize2 = 1;
  int i1, i2, j;
  //printf(" == mergesort ==\n");
  //printf("    ptr(list)=%p, ptr(extra)=%p \n", list, scratch);  
  while (sublistSize < n){         // merge all pairs of sublists from A to B
    // Example: say sublistSize = 2, n = 7 ; 
    // and on this pass  listA = [ 1,5,  3,4,   1,10,   6   ] .
    // Then after this time through we get listB = [ 1,3,5,4      1,6,10 ]
    // and on the following pass, sublistSize=4.
    i1 = 0;
    while (i1 < n){                // loop over pairs of sublists
      // merge listA[i1 ... i1+sublistSize] with listA[i2 ... i2+sublistSize2]
      // and put result into same places in listB.
      // In the above example: the 1st sublist is [1,5], 2nd sublist is [3,4].
      // At the list end, several edge effects must be handled; see below.
      i2 = i1 + sublistSize;

      //printf("  ptr(listA)=%p, ptr(listB)=%p \n", listA, listB);
      //printf("  listA = "); print_array(n, listA);
      //printf("  listB = "); print_array(n, listB);
      //printf("  sublistSize=%i, i1=%i, sublistSize2=%i, i2=%i \n", 
      //     sublistSize, i1, sublistSize2, i2);
      //printf("\n");

      if (i2 > n){ // 1st end effect: 2nd list start is past list[]'s end;
	           // just copy the 1st list over.
	for (j=i1; j<n; j++){ 
	  listB[j] = listA[j];
	}
      }
      else {
	if (i2 + sublistSize >= n){ 
	  // 2nd end effect: end of the 2nd list is past list[]
	  // adjust its size accordingly.
	  sublistSize2 = n - i2;
	}
	// The heart of the algorithm : merge two sublists from listA => listB
	merge(sublistSize, &listA[i1], sublistSize2, &listA[i2], &listB[i1]);
      }
      i1 += 2*sublistSize;
    }
    sublistSize *= 2;
    sublistSize2 = sublistSize;
    swap(listA, listB, listTmp);  // exchange what we're calling listA and listB.
  }

  //printf("  have result : \n");
  //printf("    ptr(list)=%p, ptr(scratch)=%p \n", list, scratch);
  //printf("    ptr(listA)=%p, ptr(listB)=%p \n", listA, listB);  
  //printf("    listA = "); print_array(n, listA);
  //printf("    listB = "); print_array(n, listB);

  if (listA != list){    // (This compares *addresses*, not numbers in the list.)
    // Final result is in scratch[], but we want it in list[].
    for (j=0; j<n; j++) {
      list[j] = listA[j];
    }
  }
}

