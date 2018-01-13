#ifndef _RB_ITERATOR_H
#define _RB_ITERATOR_H

#include "RBTreeImpl.h"

typedef
struct RBIterImpl{
    RBIter_t *api;
    Node *currNode;
} RBIterImpl_t;


RBIter_t*
getIteratorFromNode(
        Node *from,
        Allocator alloc,
        Deallocator dalloc);

#endif