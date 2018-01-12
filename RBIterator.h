#ifndef _RB_ITERATOR_H
#define _RB_ITERATOR_H

#include "RBTreeImpl.h"

typedef
struct RBIterImpl{
    RBIter_t *api;
    Node *currNode;
} RBIterImpl_t;


RBIter_t*
getIteratorFromNode(const Node *from);

RBIter_t*
getNextIterator(RBIter_t *curr);


#endif