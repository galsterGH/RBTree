#include <stdlib.h>
#include <string.h>
#include "RBIterator.h"

#define TO_ITER(i) ((RBIterImpl_t*)(i))


static Allocator alloc = NULL;
static Deallocator dalloc = NULL;

static
RBIter_t*
getNext(RBIter_t *i){

    Node *curr = NULL,*lastNode = NULL,*lastNodeP = NULL;
    RBIterImpl_t *iter = TO_ITER(i), *newIter = NULL;
    assert(iter != NULL);
    curr = iter->currNode;
    lastNode = iter->lastNodeInSubTree;
    lastNodeP = iter->parentOfLastNode;

    //
    // first get rid of the current iterator so that we don't leak memory
    //

    dalloc(i);
    assert(curr != lastNode || !lastNodeP);

    ALLOC(
      RBIterImpl_t,
      newIter,
      alloc,
      sizeof(RBIterImpl_t),
      NULL);

    if(curr == lastNode){
        curr = lastNodeP;
        lastNode = NULL;
        lastNodeP = NULL;
    }


}

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

    //
    // Set the function pointers to the allocator and deallocator
    //

    alloc = allc;
    dalloc = dlloc;

    ALLOC(
        RBIterImpl_t,
        iter,
        alloc,
        sizeof(RBIterImpl_t),
        NULL);

    iter->currNode = from;
    iter->api->getNext = &getNext;
    iter->api->hasNext = &hasNext;
    return (iter->api);
}

