#include "RBIterator.h"

#define TO_ITER(i) ((RBIterImpl_t*)(i))

static RBIter_t *clone(RBIter_t*);
static void* get(RBIter_t *i);
static RBIter_t* getNext(RBIter_t **i);

static
/**
 * @brief Clone an iterator.
 * @param from iterator to clone.
 * @return pointer to cloned iterator or NULL.
 */
RBIter_t*
clone(RBIter_t *from){

    RBIterImpl_t *iter = TO_ITER(from),
            *ret = NULL;

    if(!from){
        return NULL;
    }

    ALLOC(
            RBIterImpl_t,
            ret,
            iter->alloc,
            sizeof(RBIterImpl_t),
            NULL);


    ret->currNode = iter->currNode;
    ret->alloc = iter->alloc;
    ret->dalloc = iter->dalloc;
    (ret->api).getNext = &getNext;
    (ret->api).get = &get;
    (ret->api).clone = &clone;
    return &(ret->api);
}

static
/**
 * @brief Retrieve the key at the iterator
 * @param i iterator instance
 * @return pointer to stored key
 */
void*
get(RBIter_t *i){
    RBIterImpl_t *iter = TO_ITER(i);

    assert(iter != NULL);
    return (iter->currNode->key);
}


static
/**
 * @brief Get iterator to next node in order
 * @param i iterator pointer to update
 * @return next iterator or NULL
 */
RBIter_t*
getNext(RBIter_t **i){

    Node *curr = NULL,*prev = NULL;
    RBIterImpl_t *iter = TO_ITER(*i), *newIter = NULL;
    Allocator allc;
    Deallocator dllc;

    assert(iter != NULL);
    curr = iter->currNode;
    allc = iter->alloc;
    dllc = iter->dalloc;

    /// first free the memory of i so that we
    /// don't leak it
    dllc(iter);
    *i = NULL;

    if(curr->right) {
        curr = curr->right;

        while (curr->left) {
            curr = curr->left;
        }
    }else{

        ///
        /// we use our parent pointer to detect who
        /// is the next inorder node
        /// if current node (pointed to by prev)
        /// is the right son of its parent, then we've
        /// already visited that parent before
        /// we stop when we either find a parent which the current node
        /// is its left child, or when we reach the parent of root-> NULL
        ///

        for(prev = curr,curr = curr->parent;
            curr && (RIGHT_OF(curr) == prev);
            prev = curr,curr = curr->parent);
    }

    if(!curr){
        return NULL;
    }

    ALLOC(
        RBIterImpl_t,
        newIter,
        allc,
        sizeof(RBIterImpl_t),
        NULL);

    newIter->currNode = curr;
    newIter->alloc = allc;
    newIter->dalloc = dllc;
    (newIter->api).getNext = &getNext;
    (newIter->api).get = &get;
    (newIter->api).clone = &clone;
    return &(newIter->api);
}

/**
 * @brief Create an iterator from a node
 * @param from starting node
 * @param allc allocator
 * @param dlloc deallocator
 * @return iterator starting at the smallest key under from
 */

RBIter_t*
getIteratorFromNode(
    Node *from,
    Allocator allc,
    Deallocator dlloc){

    RBIterImpl_t *iter = NULL;

    if(!from){
        return NULL;
    }

    while(from->left){
        from = from->left;
    }

    ALLOC(
        RBIterImpl_t,
        iter,
        allc,
        sizeof(RBIterImpl_t),
        NULL);

    iter->currNode = from;
    iter->alloc = allc;
    iter->dalloc = dlloc;
    (iter->api).getNext = &getNext;
    (iter->api).get = &get;
    (iter->api).clone = &clone;
    return &(iter->api);
}

/**
 * @brief Return the node associated with an iterator
 * @param iter iterator instance
 * @return internal node pointer
 */
Node*
getNodeFromIter(RBIter_t* iter){
    assert(iter);
    return TO_ITER(iter)->currNode;
}


