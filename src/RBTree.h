/**
 * @file RBTree.h
 * @brief Public API for a generic red-black tree
 */
#ifndef __REBTREE_H_
#define __REBTREE_H_

#include <stddef.h>

typedef int BOOL;
typedef int compRes;
/**
 * @brief Iterator interface for RBTree
 */

typedef
struct RBIter{
    struct RBIter* (*getNext)(struct RBIter**);
    void* (*get)(struct RBIter*);
    struct RBIter *(*clone) (struct RBIter*);
}RBIter_t;

/**
 * @brief Opaque handle to a red-black tree instance
 */
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

/**< Function used to allocate memory */
typedef void* (*Allocator)(size_t sizeToAllocate);
/**< Memory deallocation function */
typedef void (*Deallocator)(void*);
/**< Called when deleting a node */
typedef void (*OnDeleteCB)(void *);
/**< Comparator used to order keys */
typedef int (*Comparator)(void*, void*);
/**< Routine used to copy keys */
typedef void (*Copy)(void **,void**);

#ifdef _DEBUG_RBTREE_
typedef void (*shower)(void*);
/**< Function to print a key when debugging */
#endif
/**
 * @brief Create a red-black tree.
 * @param alloc allocation function
 * @param dealloc deallocation function
 * @param comparator comparison function
 * @param copy key copy function
 * @return pointer to new tree or NULL
 */

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

/**
 * @brief Create a red-black tree with delete callback.
 * @param alloc allocation function
 * @param dealloc deallocation function
 * @param comparator comparison function
 * @param copy key copy function
 * @param deleteCB callback invoked on node deletion
 * @return pointer to new tree or NULL
 */
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

/**
 * @brief Destroy a tree and free all resources.
 * @param tree tree instance
 * @return TRUE on success
 */
BOOL
deleteRBTree(
    RBTree_t*);

#endif

