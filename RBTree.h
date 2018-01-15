#ifndef __REBTREE_H_
#define __REBTREE_H_

#include <stddef.h>

typedef int bool;
typedef int compRes;

typedef
struct RBIter{
    struct RBIter* (*getNext)(struct RBIter*);
    void* (*get)(struct RBIter*);
}RBIter_t;

typedef
struct RBTree{
    bool  (*insert) (struct RBTree* ,void *toInsert);
    bool (*delete) (struct RBTree*, void *toDelete);
    void*  (*find)(struct RBTree*, void *key);

#ifdef _DEBUG_RBTREE_
    bool (*showTree)(struct RBTree*);
#endif

    RBIter_t* (*getIterator)(struct RBTree*);
}RBTree_t;


typedef void* (*Allocator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef void (*OnDeleteCB)(void *);
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

RBTree_t *
createRBTreeWithCB(
        const Allocator alloc,
        const Deallocator dealloc,
        const Comparator comparator,
#ifdef _DEBUG_RBTREE_
        const shower show,
#endif
        const OnDeleteCB deleteCB
);


bool
deleteRBTree(
    RBTree_t*);

#endif
