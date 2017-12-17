#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "RBTree.h"

#define ALLOC(T,x,alloc,size) \
  do{\
     x = (T*)alloc(size);\
      if(!x){\
         return NULL;\
         }\
  }while(0)


#define TO_TREE(t) ((Tree*)(t))

typedef
struct RBTreeImpl{
   RBTree_t api;
   Allocator alloc;
   Deallocator dealloc;
   compRes comparator;
}Tree;

typedef
struct RBNode{
  void *key;
  Node *left;
  Node *right;
  Node *parent;
}Node;

static Node *root = NULL;


static
ERbstatus insert(
        RBTree_t *tree,
        void *toInsert){

        Tree *treeImpl = TO_TREE(toInsert);
        if(treeImpl == NULL){
                return enUninitializedLib;
        }
}


RBTree_t *createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    compRes comparator){

  Tree *tree = NULL;
  ALLOC(Tree,tree,alloc,sizeof(Tree));

  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->comparator = comparator;
  return &(api);
}

void deleteRBTree(RBTree_t *tree){
   TO_TREE(tree)->dealloc(tree);
}
