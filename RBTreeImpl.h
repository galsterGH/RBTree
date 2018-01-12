#ifndef _RBTREE_IMPL_H
#define _RBTREE_IMPL_H

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