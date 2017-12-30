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
      int m = 20;
      int n = 100;
      t->insert(t,&i);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      t->insert(t,&j);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      t->insert(t,&l);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      t->insert(t,&k);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      t->insert(t,&m);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      t->insert(t,&n);
      printf("*****\n");
      t->showTree(t);
      printf("-------\n");
      return 0;
    }
