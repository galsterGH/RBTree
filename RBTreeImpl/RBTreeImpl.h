//
// Created by guyal on 1/14/2018.
//

#ifndef RedBlackTree_RedBlackTree_H
#define RedBlackTree_RedBlackTree_H

extern "C"{
    #include "../src/RBTree.h"
}

#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>
#include <functional>

namespace Datastr{

    template <typename K>
    struct RBCompInt{
        virtual int compare(const K&, const K&) = 0;
    };

    template <typename K>
    class RedBlackTree :
            private RBCompInt<K>{

    private:

        //
        // aliases which will be used by this class
        // TreeKey - will be the key stored within each node
        //   it is composed of a pointer to a RBCompInt interface (for comparison)
        //   as well as the key passed by the user
        //
        using CompInt = RBCompInt<K>;
        using TreeKey = std::pair<RBCompInt<K>*,K>;
        using TreeComparator = std::function<int(const K&,const K&)>;
        friend class RBIterator;

        //
        // A wrapper iterator class
        // around the RBIter_t structure from RBTree Lib
        //
        class RBIterator{

        private:

           RBIter_t *pIterator;

        public:

            //
            // This constructor initializes the iterator by calling the
            // corresponding getIterator api from RBTree Lib
            // This iterator class supports all expected operators of an iterator
            // e.g. operator*, operator->, operator++
            //
            RBIterator(const RedBlackTree &instance):
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
                return &static_cast<TreeKey*>(
                        pIterator->get(pIterator))->second;
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

        //
        // the static member functions below will be
        // passed to the RBTree Lib
        //

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

        //
        // This will be called for every operation in the RBTree
        // we'll delegate this to the comparator passed in by the user
        //
        static
        int
        comp(void *p1, void *p2){

            auto k1 = reinterpret_cast<TreeKey*>(p1)->second;
            auto k2 = reinterpret_cast<TreeKey*>(p2)->second;

            return reinterpret_cast<TreeKey*>(p1)
                    ->first
                    ->compare(k1,k2);
        }

        //
        // This will be called when a node is begin deleted
        // and the key of its inorder needs to be copied into it
        //
        static
        void
        copy(void **to, void **from){
            auto kt1 = reinterpret_cast<TreeKey **>(to);
            auto kt2 = reinterpret_cast<TreeKey **>(from);

            delete(*kt1);
            *kt1 = *kt2;
            *kt2 = NULL;
        }

        //
        // This will be called everytime a Node is deleted
        // we'll use this notification to destroy the corresponding
        // TreeKey held within that node
        //
        static
        void
        deleteCB(void *p){
            delete(reinterpret_cast<TreeKey *>(p));
        }

        virtual
        int compare(const K& first, const K& second) override{
            return comparator(first,second);
        }

        //
        // Member variables
        //

        //
        // A unique_ptr wrapping around the RBTree_t structure
        // it will use a special deleter that will call the RBTree
        // deallocation function, when it needs to be destructed
        //
        std::unique_ptr<
                RBTree_t,
                std::function<void(void*)>> pTreeImpl;

        //This will be used to compare the keys in the tree
        TreeComparator comparator;

    public:

        using iterator = RBIterator;
        using Comparator = TreeComparator;

        // Default constructor initializes a default comparator
        RedBlackTree():
            RedBlackTree([](const K& first, const K& second)->int{
                return first < second ? -1 : (first == second ? 0 : 1);
            }){}


        //
        // main constructor initializes the unique_ptr by calling the CreateRBTreeWithCb method
        // in the RBTree Lib and passes to it all the functions above
        // it also creates the deleter function that calls deleteRbTree from the RBTree Lib.
        //
        RedBlackTree(const Comparator& compt):
                pTreeImpl(
                        createRBTreeWithCB(alloc,dealloc,comp,copy,deleteCB),
                        [](void*p){deleteRBTree(static_cast<RBTree_t*>(p));}),
                comparator(compt){}

        //currently we don't allow copying a tree
        RedBlackTree(const RedBlackTree &other) = delete;
        const RedBlackTree& operator=(const RedBlackTree &other) = delete;

        RedBlackTree(RedBlackTree &&other) noexcept{
            pTreeImpl = std::move(other.pTreeImpl);
            comparator = std::move(other.comparator);
        }

        const RedBlackTree&
        operator=(RedBlackTree &&other) noexcept{
            if(this != &other){
                pTreeImpl = std::move(other.pTreeImpl);
                comparator = std::move(other.comparator);
            }
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
                delete compoundKey;
                return false;
            }

            int res = pTreeImpl->insert(
                    pTreeImpl.get(),
                    static_cast<void*>(compoundKey));

            if(!res){
                delete compoundKey;
            }

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
#endif //RedBlackTree_RedBlackTree_H
