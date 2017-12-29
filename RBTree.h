#ifndef __REBTREE_H_
#define __REBTREE_H_

#include <stddef.h>


#define _DEBUG_RBTREE_

typedef int bool;
typedef int compRes;

typedef
struct RBIter{
    bool (*hasNext)(struct RBIter*);
    bool (*hasPrev)(struct RBIter*);
    struct RBIter* (*getNext)(struct RBIter*);
    struct RBIter* (*getPrev)(struct RBIter*);
}RBIter_t ;

typedef
struct RBTree{
    bool  (*insert) (struct RBTree* ,void *toInsert);
    bool (*delete) (struct RBTree*, void *toDelete);
    void*  (*find)(struct RBTree*, void *key);
#ifdef _DEBUG_RBTREE_
    bool (*showTree)(struct RBTree*);
#endif
    /*
    RBIter_t* (*begin)(RBTree_t*);
    RBITer_t* (*end)(RBTree_t*);
    RBIter_t* (*find) (RBTree_t *,void *toFind);
    RBIter_t* (*lowerBound)(RBTree_t*,void *k);
    RBIter_t* (*upperBound)(RBTree_t*,void *k);*/
}RBTree_t;

typedef void* (*Allocator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef int (*Comparator)(void*, void*);

#ifdef _DEBUG_RBTREE_
typedef void (*shower)(void*);
#endif


RBTree_t *
createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    Comparator comparator
#ifdef _DEBUG_RBTREE_
    ,shower show
#endif
    );

bool
deleteRBTree(
    RBTree_t*);

#endif
