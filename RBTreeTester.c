    #include <stdio.h>
    #include <stdlib.h>

    #define _DEBUG_RBTREE_
    #include "RBTree.h"

    typedef struct {
        int k;
        int v;
    } Pair;

    void *alloc(size_t b){
      return malloc(b);
    }

    void dealloc(void *v){
      free(v);
    }

    int
    test1Comp(void *i, void *j){
      int *ii = (int*)i;
      int *ji = (int*)j;

      return ((*ii < *ji) ? -1 : ((*ii == *ji) ? 0 : 1));
    }

    void test1Show(void *i){
      printf("%d\n",*((int*)i));
    }



    int
    test2Comp(void *i, void *j){
        Pair *p1 = (Pair*)i;
        Pair *p2 = (Pair*)j;

        return (p1->k < p2->k) ? -1 : ((p1->k == p2->k) ? 0 : 1);
    }

    void
    test2Show(void *i){
        printf("(%d,%d)\n",((Pair*)i)->k,((Pair*)i)->v);
    }

    void testInsert1(){
        RBTree_t *t = createRBTree(alloc,
                                   dealloc,
                                   test1Comp,
                                   test1Show);

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
    }

    void testInsert2() {
        RBTree_t *t = createRBTree(alloc,
                                   dealloc,
                                   test2Comp,
                                   test2Show);

        int i = 0;
        if (!t) {
            printf("error creating tree");
            exit(1);
        }

        Pair arrP[] = {{.k = 10, .v = 11},
                       {.k = 6, .v = 12},
                       {.k = 4, .v = 11},
                       {.k = 2, .v = 12},
                       {.k = -1, .v = 100},
                       {.k = 5, .v = 111},
                       {.k = -2, .v = 1123},
                       {.k = 3, .v = 1123}};

        for(; i < sizeof(arrP)/ sizeof(Pair); ++i){
            printf("----- inserting i %d -------\n",i);
            t->insert(t,&arrP[i]);
            t->showTree(t);
        }
    }

    int main(){

      testInsert2();
      return 0;
    }
