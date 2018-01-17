//
// Created by guyal on 1/14/2018.
//

#ifndef RBTREEIMPL_RBTREEIMPL_H
#define RBTREEIMPL_RBTREEIMPL_H

extern "C"{
    #include "../src/RBTree.h"
}

#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>
#include <functional>

namespace RedBlackTree{

    template <typename K,typename ... T>
    class RBTreeImpl{
    private:

        using Key = const K;
        using TreeKey = std::tuple<Key,const T...>;
        friend class RBIterator;


        class RBIterator{

        private:

           RBIter_t *pIterator;

        public:

            RBIterator(const RBTreeImpl &instance):
                    pIterator(instance.pTreeImpl->getIterator(
                            instance.pTreeImpl.get())){
            }

            RBIterator(RBIter_t *ptr):
                pIterator(ptr){

            }


            RBIterator(const RBIterator &other){
                return operator=(other);
            }

            RBIterator& operator=(const RBIterator& other){
                pIterator = other.pIterator;
                return (*this);
            }

            RBIterator(RBIterator &&other){
                 operator=(std::move(other));
            }

            RBIterator& operator=(RBIterator &&other){
                pIterator = std::move(other.pIterator);
                other.pIterator = nullptr;
                return (*this);
            }

            TreeKey& operator*(){
                return *static_cast<TreeKey*>(pIterator->get(pIterator));
            }

             void operator++(){
                pIterator = pIterator->getNext(&pIterator);
            }

            bool operator==(const RBIterator& other){
                return (pIterator == other.pIterator);
            }

            bool operator!=(const RBIterator &other){
                return !operator==(other);
            }

            ~RBIterator(){
                std::free(pIterator);
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

        std::unique_ptr<
                RBTree_t,
                std::function<void(void*)>> pTreeImpl;
    public:

        using iterator = RBIterator;

        RBTreeImpl():
            pTreeImpl(
                    createRBTreeWithCB(alloc,dealloc,comp,deleteCB),
                    [](void*p){deleteRBTree(static_cast<RBTree_t*>(p));}){
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

            if(!pTreeImpl){
                return false;
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

            if(!pTreeImpl){
                return false;
            }

            return pTreeImpl->del(
                    pTreeImpl.get(),
                    const_cast<std::tuple<Key> *>(&tKey));
        }

        iterator begin(){

            if(!pTreeImpl){
                return end();
            }

            return iterator(*this);
        }

        iterator end(){
            return iterator(nullptr);
        }
    };
};
#endif //RBTREEIMPL_RBTREEIMPL_H
