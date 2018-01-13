#ifndef _RBTREE_IMPL_H
#define _RBTREE_IMPL_H

#include "RBTree.h"

#define FALSE (0)
#define TRUE (1)
#define RED (0)
#define BLACK (1)
#define TO_TREE(t) ((Tree*)(t))
#define COLOR_RED(n) ((n)->color).rb = RED
#define COLOR_BLACK(n) ((n)->color).rb = BLACK
#define GET_COLOR(n) ((n)->color).rb
#define SWAP_COLORS(n) ((n)->color).rb++
#define RIGHT_ROTATE(n) ((n).dir)
#define LEFT_OF(n) ((n)->left)
#define RIGHT_OF(n) ((n)->right)

#define ALLOC(T,x,alloc,size,res) \
      do{\
         x = (T*)(alloc(size));\
          if(!x){\
             return res;\
          }\
          memset(x,0,size);\
      }while(0)

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

#endif