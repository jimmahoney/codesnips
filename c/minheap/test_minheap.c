/* test_minheap.c
 * See ./README.txt
 * Jim Mahoney | March 2011 | GPL
 */

#include "minheap.h"

// -- test heap ---
//     3
//   5       8
// 10  7    .  .
//
#define N 5
char* names[]    = {"a", "b", "c", "d", "e"};
int priorities[] = {10,   7,   5,  3,   8};

int main(){
  int i, count;
  heapnode node, nodes[N];
  minheap heap; 

  printf("creating nodes\n");
  for (i = 0; i < N; i++){
    nodes[i] = new_heapnode(priorities[i], names[i], NULL);
    print_heapnode(nodes[i]);
  }
  printf("\n");

  printf("building the heap.\n");
  heap = new_minheap(16);
  for (i = 0; i < N; i++){
    printf("  adding i=%i : \n", i);
    add(nodes[i], heap);
    print_minheap(heap);
  }
  printf("\n");

  printf("changing node b's key from 7 to 2\n");
  changekey(heap, nodes[1], 2);
  print_minheap(heap);
  printf("\n");

  printf("removing nodes one by one:\n");
  while (heap->size > 0){
    node = popmin(heap);
    print_heapnode(node);
    free_heapnode(node);
  }
  free_minheap(heap);
  printf("\n");

  printf("memory check:\n");
  printf(" malloc() + calloc() - free() = %i \n\n", get_allocation_count());

  return 0;
}
