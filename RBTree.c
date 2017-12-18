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

#define FALSE (0)
#define TRUE (1)
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


typedef
struct RBIterImpl{
  RBIter_t iterBase;
  Node *currNode;
}Iterator;

static
int
handleExpection(Node *curr){
  if(curr){
    curr->tree->dealloc(curr);
  }

  return (1);
}

static
Node*
getInorderSucc(Node *curr){
  Node *succ = NULL;

  if(!curr || !(curr->right)){
    return NULL;
  }

  for(succ = curr->right;
      succ->left;
      succ = succ->left);

  return succ;
}

static
bool
hasNext(RBIter_t *it){

  Iterator iter = ((Iterator*)it);
  Node *curr = NULL;

  if(!iter){
    return false;
  }

  curr = iter->currNode;
  assert(curr);

  if(curr->right || curr->parent){
    return TRUE;
  }

  return FALSE;
}

static
bool
hasPrev(RBIter_t *it){

  Iterator iter = ((Iterator*)it);
  Node *curr = NULL;

  if(!iter){
    return false;
  }

  curr = iter->currNode;
  assert(curr);

  if(curr->left || curr->parent){
    return TRUE;
  }

  return FALSE;
}

static
RBIter_t*
getNext(RBIter_t *it){
  Iterator *iter = ((Iterator*)it);
  Node *curr = NULL, *toRet = NULL;
  Iterator *result = NULL;
  assert(it);
  curr = iter->currNode;
  assert(curr);

  if(curr->right){
    toRet = getInorderSucc(curr);
  }
  else{
    toRet = curr->p;
  }

  ALLOC(Iterator,
        result,
        curr->tree->alloc,
        sizeof(Iterator),
        handleExpection(iter));

  result->currNode = toRet;
  memcpy(&(result->iterBase),*it,sizeof(RBIter_t));
  curr->tree->dealloc(it);
  return &(result->iterBase);
}

static
void
adjustInsert(
  Node *ins){

  assert(ins != NULL);
  Node *p = NULL,*gp = NULL,*uncle = NULL;
  Node *root = ins->tree->root;
  int uncleDir = 0;

  while(ins != root &&
        GET_COLOR(ins->parent) == RED){

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
      COLOR_RED(gp);

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

    Tree *treeImpl = TO_TREE(tree);
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

static
Node*
findNode(
    Node *root,
    void *toFind){

  int res = 0;
  while(root){
    res = root->tree->comparator(root->key,toFind);

    if(!res){
      break;
    }

    if(res < 0)
      root = root->right;
    else
      root = root->left;
  }

  return root;
}

static
RBIter_t*
find(
     RBTree_t *tree,
     void *toFind){

  Node *retNode = NULL;
  Iterator *iter = NULL;

  if(!treeImpl){
    return enUninitializedLib;
  }

  retNode = findNode(tree->root,toFind);

  if(!retNode){
    return end();
  }

  ALOCC(Iterator,
        iter,
        tree->alloc,
        sizeof(Iterator),
        end());

  initIterator(iter,retNode);
  return (&(iter-iterBase));
}

static
void
deleteNodes(Nodes **root){
  //TODO - do it iteratively
  if(!(*root)){
    return;
  }

  deleteNodes(&(root->left));
  deleteNodes(&(root->right));
  root->tree->dealloc(*root);
  root = NULL;
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
  tree->api.insert = &insert;
  tree->api.delete = &delete;
  tree->api.find = &find;
  return &(api);
}

void
deleteRBTree(RBTree_t *tree){

  if(!tree){
     return enUninitializedLib;
  }

  deleteNodes(&(tree->root));
  TO_TREE(tree)->dealloc(tree);
}
