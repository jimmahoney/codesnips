/* min_heap.c
 * C code for a min binary heap implementation.
 * See ./README.txt for the details.
 * Jim Mahoney | March 2011 | GPL
 */

#include "minheap.h"

heapnode tmp;  // for swap
int itmp;      // ditto

// key of node with index i in heap; assumes heap is 'heap'
#define key(i) ((heap->nodearray)[i]->key)

// node with index i in heap; assumes heap is 'heap'
#define nth(i) ((heap->nodearray)[i])

// Swap nodes with index i and j in heap, and swaps their stored indices.
// Assumes variable 'heap' is in scope; uses tmp and itmp.
// (Yes, this looks ugly, but it's just tmp=a,a=b,b=tmp done twice.)
//#define swap(i,j) (tmp=(heap->nodearray)[i], (heap->nodearray)[i]=(heap->nodearray)[j], (heap->nodearray)[j]=tmp, itmp=(heap->nodearray)[i]->index, (heap->nodearray)[i]->index=(heap->nodearray)[j]->index, (heap->nodearray)[j]->index=itmp)
void swapem(int i, int j, minheap heap){
  //printf(" start swapem; i,j=%i,%i \n", i,j);
  //print_minheap(heap);
  tmp = (heap->nodearray)[i]; 
  (heap->nodearray)[i] = (heap->nodearray)[j]; 
  (heap->nodearray)[j] = tmp; 
  //print_minheap(heap);
  itmp = (heap->nodearray)[i]->index; 
  (heap->nodearray)[i]->index = (heap->nodearray)[j]->index; 
  (heap->nodearray)[j]->index = itmp;
  //print_minheap(heap);
  //printf(" end swapem\n");
}

// indices of left,right children and parent of node i in heap array.
#define left(i) (2*(i)+1)
#define right(i) (2*(i)+2)
#define parent(i) (((i)-1)/2)

// Each time the heap's array needs to be bigger, grow it by this much.
#define HEAP_ARRAY_INCREMENT_SIZE 100

minheap new_minheap(int initsize){
  int i;
  minheap heap = _malloc(sizeof(struct _minheap));
  heap->size = 0;
  heap->arraysize = initsize;
  heap->nodearray = _malloc(sizeof(initsize * sizeof(heapnode)));
  //printf("in new_minheap; before NULLs\n");
  //print_minheap(heap);
  for (i=0; i<initsize; i++){
    heap->nodearray[i] = NULL;
  }
  //printf("in new_minheap; after NULLs\n");
  //print_minheap(heap);
  return heap;
}

heapnode new_heapnode(int key, char* name, void* data){
  heapnode node = _malloc(sizeof(struct _heapnode));
  node->key = key;
  node->index = -1;
  if (name != NULL){
    // strings end with trailing 0, so need 1 byte more than length.
    node->name = _malloc(sizeof(strlen(name)) + 1);
    strcpy(node->name, name);
  }
  else {
    node->name = NULL;
  }
  node->data = data;
  return node;
}

void free_minheap(minheap heap){
  int i;
  for (i=0; i < heap->size; i++){
    free_heapnode((heap->nodearray)[i]);
  }
  _free(heap);
}

void free_heapnode(heapnode node){
  if (node->name != NULL){
    _free(node->name);
  }
  _free(node);
}

void bubbleup(minheap heap, int which){
  // Move a node in heap upward while it's smaller than its parent.
  // (Note that key(), swap() macros use heap.)
  int inode = which;                          // index of node to bubble up.
  int iparent = parent(inode);                // index of its parent.
  //printf("  in bubbleup: inode=%i, iparent=%i \n", inode, iparent);
  while (key(iparent) > key(inode)){
    swapem(inode, iparent, heap);
    inode = iparent;
    iparent = parent(inode);
    //printf("  in bubbleup:  swapped; now inode=%i, iparent=%i \n", inode, iparent);    
  }
}

void siftdown(minheap heap, int which){
  // Swap a node in heap downward while it's bigger than a child.
  // (Note that key(), swap() macros use heap.)
  int inode = which;              // index of node to sift downwards.
  int ileft = left(inode);        // index of one child
  int iright = right(inode);      // index of one child
  int ismaller;                   // index of smaller child
  if (iright >= heap->size){
    iright = ileft;
  }
  if (ileft >= heap->size){
    return;
  }
  // Is current node key bigger than one of its children?
  while ((key(ileft) < key(inode)) || (key(iright) < key(inode))){
    // Yes; swap with smaller of children.
    ismaller = key(ileft) <= key(iright) ? ileft : iright;
    //printf("  in siftdown; swapping %i and %i \n", inode, ismaller);
    swapem(inode, ismaller, heap);
    //printf("  in siftdown after swap \n");
    //print_minheap(heap);
    inode = ismaller;
    ileft = left(inode);
    iright = right(inode);
    if (iright >= heap->size){
      iright = ileft;
    }
    if (ileft >= heap->size){
      return;
    }
  }
}

void add(heapnode node, minheap heap){
  heapnode* newarray;
  int index;
  // ---------
  // *** not yet implemented : grow heap->nodearray if it isn't big enough
  //
  //int newsize = heap->size + 1;
  // printf(" in add: heap->arraysize = %i, newsize = %i \n", heap->arraysize, newsize);
  //while (heap->arraysize <= newsize){
  //  printf(" growing nodearray \n");
  //  // Make sure there's enough room in the node array to store the new node.
  //  heap->arraysize += HEAP_ARRAY_INCREMENT_SIZE;
  //  newarray = (heapnode*) sizeof(heap->arraysize * sizeof(heapnode));
  //  memcpy(newarray, heap->nodearray, newsize - 1);
  //  free(heap->nodearray);
  //  heap->nodearray = newarray;
  //}
  // ------------
  heap->size++;
  index = heap->size - 1;             // Where to put node being inserted.
  node->index = index;                // Store index within node.
  //printf(" ** in add :\n");
  //print_minheap(heap);
  heap->nodearray[index] = node;    // Set last element to new node.
  //printf(" ** before bubbleup \n");
  //print_minheap(heap);
  //printf(" calling bubbleup \n");
  bubbleup(heap, index);              // Swap it upwards as needed.
}

heapnode popmin(minheap heap){
  // minimum node is first.
  // Save it, move last to first's spot, and then sift it downward.
  if (heap->size == 0){
    return NULL;
  }
  else {
    //printf(" in popmin \n");
    //print_minheap(heap);
    heapnode minnode = heap->nodearray[0];                
    swapem(0, heap->size-1, heap);
    heap->size--;
    //printf(" in popmin after bottom to top \n");
    //print_minheap(heap);
    if (heap->size > 0){
      siftdown(heap, 0);
    }
    //printf(" in popmin after sift \n");
    //print_minheap(heap);
    minnode->index = -1;
    return minnode;
  }
}

heapnode findmin(minheap heap){
  // node with smallest key is first in the array.
  if (heap->size == 0){
    return NULL;
  }
  else {
    return (heap->nodearray)[0];
  }
}

void changekey(minheap heap, heapnode node, int newkey){
  int oldkey = node->key;
  node->key = newkey;
  if (newkey < oldkey){
    bubbleup(heap, node->index);
  }
  else if (newkey > oldkey){
    siftdown(heap, node->index);
  }
}

void print_heapnode(heapnode node){
  if (node == NULL){
    printf("node(NULL)\n");
  }
  else {
    printf("node('%s' at %p key=%i index=%i)\n", node->name, node, node->key, node->index);
  }
}

void print_minheap(minheap heap){
  int i;
  heapnode node;
  printf("heap(at %p size=%i arraysize=%i nodearray=%p\n", 
	 heap, heap->size, heap->arraysize, heap->nodearray);
  for (i=0; i < heap->size; i++){
    printf("  %p at %p : ", heap->nodearray[i], &(heap->nodearray[i]));
    node = nth(i);
    print_heapnode(node);
  }
  printf(")\n");
}
