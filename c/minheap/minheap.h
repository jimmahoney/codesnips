/* min_heap.h
 *
 * C header for a min binary heap implementation.
 *
 * Currently the only way to create the heap is
 * to insert one node at a time, which for n nodes
 * is O(n log(n)). A faster technique is described
 * at http://en.wikipedia.org/wiki/Binary_heap, 
 * which isn't included here.
 *
 * There is a decreasekey method, so that this 
 * can be used for Dijkstra's shortest path algorithm.
 *
 * See ./README.txt for more info.
 *
 * Jim Mahoney | March 2011 | GPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jims_utils.h"

typedef struct _heapnode *heapnode;
struct _heapnode {
  int key;
  int index;       // index of this node in heap's array
  char* name;
  void* data;
};

typedef struct _minheap *minheap;
struct _minheap {
  int size;
  int arraysize;
  heapnode* nodearray;
};

// Create a new min-heap with given initial size.
// (At present the size is fixed; make it big enough at the start.)
minheap new_minheap(int initsize);

// Create a new node.
// If name is not NULL, it's duplicated to node->name.
// The data pointer is copied to node->data, without allocating new memory.
heapnode new_heapnode(int key, char* name, void* data);

// Print the heap or one node.
void print_heapnode(heapnode node);
void print_minheap(minheap heap);

// Free memory of heap and its nodes.
void free_minheap(minheap heap);

// Free memory of a node and its name string.
void free_heapnode(heapnode node);

// Add a node to a heap.
// Time is max O(log size); average time is O(1).
void add(heapnode node, minheap heap);

// Remove and return the mode with the smallest key; 
// Time is O(long size).
heapnode popmin(minheap heap);

// Return the node with the smallest key, without removing it.
// Time is O(1).
heapnode findmin(minheap heap);

// Change the key of a node within a heap and adjust the heap accordingly.
// Max time is O(log size)
void changekey(minheap heap, heapnode node, int newkey);

