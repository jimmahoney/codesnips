/* see ./README */

#include <stdlib.h>
#include "permute.h"
#include "jims_utils.h"

void permute(int n, void (*func)(int, int*)) {
  int *array, *dir, tmp, tmp2, i, max;
  if (n < 1) 
    return;
  array = _malloc(n * sizeof(int));
  dir = _calloc(n, sizeof(int));
  for (i = 0; i < n; i++) array[i] = i;
  max = n - 1;
  if (n != 1)
    do {
      func(n, array);
      tmp = array[max];
      tmp2 = dir[max];
      i = !dir[max] ? max - 1 : max + 1;
      array[max] = array[i];
      array[i] = tmp;
      dir[max] = dir[i];
      dir[i] = tmp2;
      for (i = 0; i < n; i++){
	if (array[i] > tmp){
	  dir[i] = !dir[i];
	}
      }
      max = n;
      for (i = 0; i < n; i++){
	if ( ((!dir[i] && i != 0 && array[i] > array[i-1]) ||  \
	      (dir[i] && i != n-1 && array[i] > array[i+1]))   \
	     && (array[i] > array[max] || max == n)){
	  max = i;
	}
      }
    } while (max < n);
  func(n, array);
  _free(dir);
  _free(array);
}

