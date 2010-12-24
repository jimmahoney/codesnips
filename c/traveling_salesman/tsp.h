/*
 * traveling salesman problem
 */

#ifndef H_TSP
#define H_TSP

typedef struct _cities *cities;
struct _cities {
  int n;
  int* x;
  int* y;
};

#endif
