//
// Created by guyal on 1/14/2018.
//

#ifndef RBTREEIMPL_RBTREEIMPL_H
#define RBTREEIMPL_RBTREEIMPL_H

extern "C"{
    #include "..\src\RBTree.h"
}

#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>

namespace RedBlackTree{

    template <typename K,typename ... T>
    class RBTreeImpl{
    private:

        using Key = const K;
        using TreeKey = std::tuple<Key,const T...>;

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
            auto v1 = static_cast<TreeKey*>(p1);
            auto v2 = static_cast<TreeKey*>(p2);
            auto k1 = std::get<0>(*v1);
            auto k2 = std::get<0>(*v2);

            if(k1 < k2){
                return -1;
            }
            else if(k1 == k2){
                return 0;
            }

            return 1;
        }

        static
        void
        deleteCB(void *p){
            delete(static_cast<TreeKey *>(p));
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
        insert(Key& key, const T&... vals){
            TreeKey *compoundKey =
                    new TreeKey(std::make_tuple(key,vals...));

            if(!compoundKey){
                throw std::string("out of memory");
            }

            int res =
            pTreeImpl->insert(
                    pTreeImpl.get(),
                    static_cast<void*>(compoundKey));

            return (res);
        }

        bool
        del(Key& k){

            std::tuple<Key> tKey =
                    std::make_tuple(k);

            return pTreeImpl->del(
                    pTreeImpl.get(),
                    const_cast<std::tuple<Key> *>(&tKey));
        }
    };
};
#endif //RBTREEIMPL_RBTREEIMPL_H
