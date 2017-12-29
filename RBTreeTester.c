#include <stdio.h>
#include <stdlib.h>

#define _DEBUG_RBTREE_
#include "RBTree.h"

void *alloc(size_t b){
  return malloc(b);
}

void dealloc(void *v){
  free(v);
}

int
comp(void *i, void *j){
  int *ii = (int*)i;
  int *ji = (int*)j;

  return ((*ii < *ji) ? -1 : ((*ii == *ji) ? 0 : 1));
}

void show(void *i){
  printf("%d\n",*((int*)i));
}

int main(){
  RBTree_t *t = createRBTree(alloc,
                             dealloc,
                             comp,
                             show);

  if(!t){
    printf("error creating tree");
    exit(1);
  }

  int i = 5;
  int j = 3;
  int l = 2;
  int k = 10;
  t->insert(t,&i);
  t->showTree(t);
  t->insert(t,&j);
  t->showTree(t);
  t->insert(t,&l);
  t->showTree(t);
  t->insert(t,&k);
  t->showTree(t);

  return 0;
}
