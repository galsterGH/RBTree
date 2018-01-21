
#include "RBTreeImpl.h"
#include <iostream>

using namespace std;

struct P{
    int i;
    int j;
};

int main(){
    RedBlackTree::RBTreeImpl<P> tree([](const P& p1, const P& p2)->int{
       return p1.i < p2.i || (p1.i == p2.i && p1.j < p2.j) ? 1 :
              (p1.i == p2.i && p1.j == p2.j ? 0 : -1);
    });

    tree.insert({1,2});
    tree.insert({1,4});
    tree.insert({1,3});
    tree.insert({5,6});
    tree.insert({5,2});
    tree.insert({5,7});
    tree.insert({8,2});
    tree.insert({1,0});
    tree.insert({10,2});
    tree.insert({8,8});
    tree.insert({9,0});

    for(auto i = tree.begin(); i != tree.end(); ++i){
        cout << i->i << " " << i->j << endl;
    }

    tree.del({1,3});
    tree.del({10,2});
    tree.del({5,6});


    cout << "after delete:" << endl;

    for(auto i = tree.begin(); i != tree.end(); ++i){
        cout << i->i << " " << i->j << endl;
    }



    return 0;
}

/*
int main() {
    RedBlackTree::RBTreeImpl<int> tree;
    tree.insert(10);
    tree.insert(100);
    tree.insert(21);
    tree.insert(11);
    tree.insert(101);
    tree.insert(9);
    tree.insert(12);
    tree.insert(90);
    tree.insert(23);
    tree.insert(43);

    for(auto i = tree.begin(); i != tree.end(); ++i){
        std::cout << *i  << std::endl;
    }

    tree.del(11);
    tree.del(90);

    cout << "again: " << endl;

    for(auto i = tree.begin(); i != tree.end(); ++i){
        std::cout << *i  << std::endl;
    }

    return 0;
}*/