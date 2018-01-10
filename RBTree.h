#ifndef __REBTREE_H_
#define __REBTREE_H_

#include <stddef.h>

#define _DEBUG_RBTREE_

typedef int bool;
typedef int compRes;

typedef
struct RBTree{
    bool  (*insert) (struct RBTree* ,void *toInsert);
    bool (*delete) (struct RBTree*, void *toDelete);
    void*  (*find)(struct RBTree*, void *key);
#ifdef _DEBUG_RBTREE_
    bool (*showTree)(struct RBTree*);
#endif
}RBTree_t;

typedef void* (*Allocator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef int (*Comparator)(void*, void*);

#ifdef _DEBUG_RBTREE_
typedef void (*shower)(void*);
#endif

RBTree_t *
createRBTree(
    const Allocator alloc,
    const Deallocator dealloc,
    const Comparator comparator
#ifdef _DEBUG_RBTREE_
    ,const shower show
#endif
    );

bool
deleteRBTree(
    RBTree_t*);

#endif
