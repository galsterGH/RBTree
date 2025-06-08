/**
 * @file RBIterator.h
 * @brief Internal iterator implementation details
 */
#ifndef _RB_ITERATOR_H
#define _RB_ITERATOR_H

#include "RBTreeImpl.h"

/**
 * @brief Concrete iterator used by RBTree
 */
typedef
struct RBIterImpl{
    RBIter_t api;
    Node *currNode;
    Allocator alloc;
    Deallocator dalloc;
} RBIterImpl_t;

/**
 * @brief Create an iterator starting from a node
 * @param from starting node
 * @param alloc memory allocator
 * @param dalloc deallocator
 * @return newly allocated iterator or NULL
 */
RBIter_t*
getIteratorFromNode(
        Node *from,
        Allocator alloc,
        Deallocator dalloc);

/**
 * @brief Return the internal node of an iterator
 * @param iter iterator instance
 * @return pointer to underlying node
 */

extern Node *getNodeFromIter(RBIter_t*);

#endif
