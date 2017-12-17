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
    void* (*getNext)(RBIter_t*);
    void* (*getPrev)(RBIter_t*);
}RBIter_t ;

typedef
struct RBTree{
    ERbstatus  (*insert) (RBTree_t* ,void *toInsert);
    ERbstatus (*delete) (RBTree_ *,void *toDelete);
    RBIter * (*find) (RBTree_t *,void *toFind);
    RBIter * (*lowerBound)(RBTree_t*,void *k);
    RBIter * (*upperBound)(RBTree_t*,void *k);
}RBTree_t;

typedef void* (*Allcoator)(size_t sizeToAllocate);
typedef void (*Deallocator)(void*);
typedef compRes (*Comparator)(void*, void*);

RBTree_t *
createRBTree(
    Allocator alloc,
    Deallocator dealloc,
    compRes comparator);

void
deleteRBTree(
    RBTree_t*)

#endif
