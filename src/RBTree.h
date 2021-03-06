#ifndef __REBTREE_H_
#define __REBTREE_H_

#include <stddef.h>

typedef int BOOL;
typedef int compRes;

typedef
struct RBIter{
    struct RBIter* (*getNext)(struct RBIter**);
    void* (*get)(struct RBIter*);
    struct RBIter *(*clone) (struct RBIter*);
}RBIter_t;

typedef
struct RBTree{
    BOOL  (*insert) (struct RBTree* ,void *toInsert);
    BOOL (*del) (struct RBTree*, void *toDelete);
    void*  (*find)(struct RBTree*, void *key);

#ifdef _DEBUG_RBTREE_
    BOOL (*showTree)(struct RBTree*);
#endif

    RBIter_t* (*getIterator)(struct RBTree*, RBIter_t *);
}RBTree_t;

typedef void* (*Allocator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef void (*OnDeleteCB)(void *);
typedef int (*Comparator)(void*, void*);
typedef void (*Copy)(void **,void**);

#ifdef _DEBUG_RBTREE_
typedef void (*shower)(void*);
#endif

RBTree_t *
createRBTree(
    const Allocator alloc,
    const Deallocator dealloc,
    const Comparator comparator,
    const Copy copy
#ifdef _DEBUG_RBTREE_
    ,const shower show
#endif
    );

RBTree_t *
createRBTreeWithCB(
        const Allocator alloc,
        const Deallocator dealloc,
        const Comparator comparator,
        const Copy copy,
#ifdef _DEBUG_RBTREE_
        const shower show,
#endif
        const OnDeleteCB deleteCB
);

BOOL
deleteRBTree(
    RBTree_t*);

#endif
