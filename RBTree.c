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

//forward declare RBTreeImpl
struct RBTreeImpl;

typedef
struct RBNode{
  void *key;
  Node *left;
  Node *right;
  Node *parent;
  struct RBTreeImpl *tree;

  struct Color{
    unsigned rb : 1;
  }color;

}Node;

typedef
struct RBTreeImpl{
   RBTree_t api;
   Node *root;
   Allocator alloc;
   Deallocator dealloc;
   compRes comparator;
}Tree;


static
void
adjustInsert(
  Node *ins){

  assert(ins != NULL);
  Node *p = NULL,*gp = NULL,*uncle = NULL;
  Node *root = ins->tree->root;
  int uncleDir = 0;

  while(ins != root){
    p = ins->parent;
    gp = p->parent;
    uncle = (gp->left == p ? gp->right : gp->left);
    uncldeDir = (gp->left == p);

    if(GET_COLOR(p) == BLACK){
      break;
    }

    //
    // a NULL uncle is a black uncle
    //
    if(uncle &&
       GET_COLOR(p) == RED &&
       GET_COLOR(uncle) == RED){

      COLOR_BLACK(p);
      COLOR_BLACK(uncle);

      //
      //the gradfather could be violating RB props
      //
      ins = gp;
    }
    else{
      if(ins == p->left && !uncleDir){
        rightRotate(p);
        p = ins;
      }
      else if(ins == p->right && uncleDir){
        leftRotate(p);
        p = ins;
      }

      SWAP_COLORS(p);
      SWAP_COLORS(gp);

      if(p == gp->left){
        rightRotate(gp);
      }
      else{
        leftRotate(gp);
      }

      break;
    }
  }

  COLOR_BLACK(root);
}

static
Node*
getInsPoint(
    Node *root,
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
    Node **root = NULL;
    CompRes comp = NULL;
    int res = 0;

    if(treeImpl == NULL){
      return enUninitializedLib;
    }

    root = &(treeImpl->root);
    comp = treeImpl->comparator;
    ALLOC(Node,toIns,treeImpl->alloc,sizeof(Node),enOutOfMem);
    toIns->key = toInsert;
    toIns->tree = tree;
    COLOR_RED(toIns);

    if(!(*root)){
      *root = toIns;
      COLOR_BLACK(*root);
    }
    else{
      insPoint = getInsPoint(*root,toInsert);
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

  Node *curr =

  if(!tree){
     return enUninitializedLib;
  }

  while(curr){

  }

   TO_TREE(tree)->dealloc(tree);
}
