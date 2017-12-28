#ifndef __REBTREE_H_
#define __REBTREE_H_

typedef int bool;
typedef int compRes;

typedef
enum RBStatus{
   enSucc,
   enOutOfMem,
   enUninitializedLib,
   enKeyNotFound,
   enDupKeys
}ERbstatus;

typedef
struct RBIter{
    bool (*hasNext)(RBIter_t*);
    bool (*hasPrev)(RBIter_t*);
    RBIter_t* (*getNext)(RBIter_t*);
    RBIter_t* (*getPrev)(RBIter_t*);
}RBIter_t ;

typedef
struct RBTree{
    bool  (*insert) (RBTree_t* ,void *toInsert);
    bool (*delete) (RBTree_t*, void *toDelete);
    void*  (*find)(RBTree_t *, void *key);
#ifdef _DEBUG_RBTREE_
    void (*showTree)(RBTree_t*)
#endif
    /*
    RBIter_t* (*begin)(RBTree_t*);
    RBITer_t* (*end)(RBTree_t*);
    RBIter_t* (*find) (RBTree_t *,void *toFind);
    RBIter_t* (*lowerBound)(RBTree_t*,void *k);
    RBIter_t* (*upperBound)(RBTree_t*,void *k);*/
}RBTree_t;

typedef void* (*Allcoator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef compRes (*Comparator)(void*, void*);

#ifdef _DEBUG_RBTREE_
typedef void (*shower)(void*);
#endif


RBTree_t *
createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    compRes comparator
#ifdef _DEBUG_RBTREE_
    ,shower show
#endif
    );

void
deleteRBTree(
    RBTree_t*)

#endif
