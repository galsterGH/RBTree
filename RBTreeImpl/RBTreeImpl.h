//
// Created by guyal on 1/14/2018.
//

#ifndef RBTREEIMPL_RBTREEIMPL_H
#define RBTREEIMPL_RBTREEIMPL_H

extern "C"{
    #include "RBTree.h"
}

#include <cstdlib>
#include <memory>
#include <string>

namespace RedBlackTree{

    template <typename V>
    class RBTreeImpl{
    private:

        struct RBTreeDeleter{
            void operator()(RBTree_t *ptr){
                deleteRBTree(ptr);
            }
        };

        static
        void*
        alloc(size_t size){
            return std::malloc(size);
        }

        static
        void
        dealloc(void *p){
            return std::free(p);
        }

        static
        int
        comp(void *p1, void *p2){
            auto v1 = static_cast<V*>(p1);
            auto v2 = static_cast<V*>(p2);

            if(*v1 < *v2){
                return -1;
            }
            else if(*v1 == *v2){
                return 0;
            }

            return 1;
        }

        static
        void
        deleteCB(void *p){
            delete(static_cast<V*>(p));
        }

        std::unique_ptr<RBTree_t, RBTreeDeleter>
                pTreeImpl;
    public:

        RBTreeImpl():
            pTreeImpl(
                    createRBTreeWithCB(alloc,dealloc,comp,deleteCB),
                    RBTreeDeleter()){
        }

        RBTreeImpl(const RBTreeImpl &other) = delete;
        const RBTreeImpl& operator=(const RBTreeImpl &other) = delete;

        RBTreeImpl(RBTreeImpl &&other){
            operator=(std::move(other));
        }

        const RBTreeImpl&
        operator=(RBTreeImpl &&other){
            pTreeImpl = std::move(other);
            return (*this);
        }

        bool
        insert(const V& val){
            V *newV = new V(val);

            if(!newV){
                throw std::string("out of memory");
            }

            int res =
            pTreeImpl->insert(
                    pTreeImpl.get(),
                    static_cast<void*>(newV));

            return (res);
        }

        bool
        del(const V& val){
            return pTreeImpl->del(
                    pTreeImpl.get(),
                    const_cast<V*>(&val));
        }
    };
};
#endif //RBTREEIMPL_RBTREEIMPL_H
