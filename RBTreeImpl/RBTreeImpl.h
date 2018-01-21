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


    template <typename K>
    struct RBCompInt{
        virtual int compare(const K&, const K&) = 0;
    };

    template <typename K>
    class RBTreeImpl :
            private RBCompInt<K>{

    private:

        using CompInt = RBCompInt<K>;
        using TreeKey = std::pair<RBCompInt<K>*,K>;
        using TreeComparator = std::function<int(const K&,const K&)>;
        friend class RBIterator;

        class RBIterator{

        private:

           RBIter_t *pIterator;

        public:

            RBIterator(const RBTreeImpl &instance):
                    pIterator(instance.pTreeImpl->getIterator(
                            instance.pTreeImpl.get(),NULL)){
            }

           RBIterator(RBIter_t *ptr):
                pIterator(ptr){
            }

            RBIterator(const RBIterator &other){
                 operator=(other);
            }

            RBIterator& operator=(const RBIterator& other) {
                pIterator =
                        other.pIterator ?
                        (other.pIterator)->clone(other.pIterator) :
                        NULL;

                return *this;
            }

            RBIterator(RBIterator &&other){
                 operator=(std::move(other));
            }

            RBIterator& operator=(RBIterator &&other){
                pIterator = std::move(other.pIterator);
                other.pIterator = nullptr;
                return (*this);
            }

            const K& operator*() const{
                return static_cast<TreeKey*>(pIterator->get(pIterator))->second;
            }

            K* operator->(){
                return &static_cast<TreeKey*>(pIterator->get(pIterator))->second;
            }

            RBIterator operator++(){
                pIterator = pIterator->getNext(&pIterator);
                return *this;
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

            auto k1 = static_cast<TreeKey*>(p1)->second;
            auto k2 = static_cast<TreeKey*>(p2)->second;

            return static_cast<TreeKey*>(p1)
                    ->first
                    ->compare(k1,k2);
        }

        static
        void
        copy(void **to, void **from){
            auto kt1 = reinterpret_cast<TreeKey **>(to);
            auto kt2 = reinterpret_cast<TreeKey **>(from);

            delete(*kt1);
            *kt1 = *kt2;
            *kt2 = NULL;
        }

        static
        void
        deleteCB(void *p){
            delete(static_cast<TreeKey *>(p));
        }

        virtual
        int compare(const K& first, const K& second) override{
            return comparator(first,second);
        }

        std::unique_ptr<
                RBTree_t,
                std::function<void(void*)>> pTreeImpl;

        TreeComparator comparator;


    public:

        using iterator = RBIterator;
        using Comparator = TreeComparator;

        RBTreeImpl():
            RBTreeImpl([](const K& first, const K& second)->int{
                return first < second ? -1 : (first == second ? 0 : 1);
            }){}


        RBTreeImpl(const Comparator& compt):
                pTreeImpl(
                        createRBTreeWithCB(alloc,dealloc,comp,copy,deleteCB),
                        [](void*p){deleteRBTree(static_cast<RBTree_t*>(p));}),
                comparator(compt){}

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
        insert(K key){
            TreeKey *compoundKey =
                    new TreeKey({this,key});

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
        del(K k){

            auto tKey =
                    std::make_pair(this,k);

            if(!pTreeImpl){
                return false;
            }

            return pTreeImpl->del(
                    pTreeImpl.get(),
                    &tKey);
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
