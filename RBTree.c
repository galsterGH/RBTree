#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "RBTree.h"

#define ALLOC(T,x,alloc,size,res)                  \
  do{\
     x = (T*)alloc(size);\
      if(!x){\
         return res;\
      }\
      memset(x,0,size);\
  }while(0)

#define RED (0)
#define BLACK (1)
#define TO_TREE(t) ((Tree*)(t))
#define COLOR_RED(n) (n->color).rb = RED
#define COLOR_BLACK(n) (n->color).rb = BLACK
#define GET_COLOR(n) (n->color).rb;
#define SWAP_COLORS(n) (n->color).rb++

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
  RBTreeImpl *tree;

  struct Color{
    unsigned rb : 1;
  }color;

}Node;

static Node *root = NULL;


static
Node*
getInsPoint(
    void *key){

  assert(root != NULL);
  Node *curr = root;
  CompRes comp = curr->tree->comparator;
  int res = 0;

  while(curr){
    res = comp(curr->key,key);

    if(res == 0){
      break;
    }

    if(res < 0){
      if(curr->right)
        curr = curr->right;
      else
        break;
    }
    else{
      if(curr->left)
        curr = curr->left;
      else
        break;
    }
  }

  return curr;
}

static
ERbstatus
insert(
    RBTree_t *tree,
    void *toInsert){

    Tree *treeImpl = TO_TREE(toInsert);
    Node *toIns = NULL;
    Node *insPoint = NULL;
    CompRes comp = NULL;
    int res = 0;

    if(treeImpl == NULL){
          return enUninitializedLib;
    }

    comp = treeImpl->comparator;
    ALLOC(Node,toIns,treeImpl->alloc,sizeof(Node),enOutOfMem);
    toIns->key = toInsert;
    toIns->tree = tree;
    COLOR_RED(toIns);

    if(!root){
      root = toIns;
      COLOR_BLACK(root);
    }
    else{
      insPoint = getInsPoint(toInsert);
      assert(insPoint != NULL);
      res = comp(insPoint->key,toInsert);

      if(res == 0){
        tree->dealloc(toIns);
        return enDupKeys;
      }

      if(res < 0){
        insPoint->left = toIns;
      }
      else{
        insPoint->right = toIns;
      }

      toIns->parent = insPoint;
      if(GET_COLOR(insPoint) == RED){
        adjustInsert(toIns);
      }
    }

    return (enSucc);
}


RBTree_t *
createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    compRes comparator){

  Tree *tree = NULL;
  ALLOC(Tree,tree,alloc,sizeof(Tree),NULL);

  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->comparator = comparator;
  return &(api);
}

void
deleteRBTree(RBTree_t *tree){
   TO_TREE(tree)->dealloc(tree);
}
