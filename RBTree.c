#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
#define COLOR_RED(n) ((n)->color).rb = RED
#define COLOR_BLACK(n) ((n)->color).rb = BLACK
#define GET_COLOR(n) ((n)->color).rb
#define SWAP_COLORS(n) ((n)->color).rb++

//forward declare RBTreeImpl
struct RBTreeImpl;

typedef
struct RBNode{
  void *key;
  struct RBNode *left;
  struct RBNode *right;
  struct RBNode *parent;
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
   Comparator comparator;
#ifdef _DEBUG_RBTREE_
  shower show;
#endif
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

/*
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

*/

static
void
leftRotate(Node *n){
  Node *right = NULL,*leftRight = NULL;

  if(!n){
    return;
  }

  right = n->right;
  leftRight = (right ? right->left : NULL);
  n->right = leftRight;
  leftRight->parent = n;
  right->left = n;
  right->parent = n->parent;
  n->parent = right;

  if(n == right->parent->left){
    right->parent->left = right;
  }
  else{
    right->parent->right = right;
  }
}

static
void
rightRotate(Node *n){
  Node *left = NULL,*rightLeft = NULL;

  if(!n){
    return;
  }

  left = n->left;
  rightLeft = (left ? left->right : NULL);
  n->left = rightLeft;
  rightLeft->parent = n;
  left->right = n;
  left->parent = n->parent;
  n->parent = left;

  if(n == left->parent->left){
    left->parent->left = left;
  }
  else{
    left->parent->right = left;
  }
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
    uncleDir = (gp->left == p);

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
  Comparator comp = curr->tree->comparator;
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
bool
insert(
    RBTree_t *tree,
    void *toInsert){

    Tree *treeImpl = TO_TREE(tree);
    Node *toIns = NULL;
    Node *insPoint = NULL;
    Node **root = NULL;
    Comparator comp = NULL;
    int res = 0;

    if(treeImpl == NULL){
      return FALSE;
    }

    root = &(treeImpl->root);
    comp = treeImpl->comparator;
    ALLOC(Node,toIns,treeImpl->alloc,sizeof(Node),FALSE);
    toIns->key = toInsert;
    toIns->tree = treeImpl;
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
        treeImpl->dealloc(toIns);
        return FALSE;
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

    return (TRUE);
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


#ifdef _DEBUG_RBTREE_
static
void
showRecursively(RBTree_t *t, Node *n){
  if(!n){
    return;
  }

  showRecursively(t,n->left);
  t->show(n->key);
  showRecursively(t,n->right);
}

static
void
showTree(RBTree_t *t){
  if(!t || !(t->show)){
    return FALSE;
  }

  Node *n = TO_TREE(t)->root;
  showRecursively(t,n);
  return TRUE;
}

#endif

static
void *
find(RBTree_t * t,
     void *toFind){

  if(!t){
    return NULL;
  }

  Node *n = findNode(TO_TREE(t)->root,toFind);

  if(!n){
    return NULL;
  }

  return n->key;
}

/*
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
*/

static
void
deleteNodes(Node **root){
  //TODO - do it iteratively
  if(!(*root)){
    return;
  }

  deleteNodes(&((*root)->left));
  deleteNodes(&((*root)->right));
  (*root)->tree->dealloc(*root);
  root = NULL;
}


RBTree_t *
createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    Comparator comparator
#ifdef _DEBUG_RBTREE_
    ,shower show
#endif
             ){

  Tree *tree = NULL;
  ALLOC(Tree,tree,alloc,sizeof(Tree),NULL);

  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->comparator = comparator;
  tree->api.insert = &insert;
  tree->api.delete = NULL;
  tree->api.find = &find;

#ifdef _DEBUG_RBTREE_
  tree->show = show;
  tree->api.showTree = showTree;
#endif

  return &(tree->api);
}

bool
deleteRBTree(RBTree_t *tree){

  if(!tree){
    return TRUE;
  }

  deleteNodes(&(TO_TREE(tree)->root));
  TO_TREE(tree)->dealloc(tree);
  return TRUE;
}
