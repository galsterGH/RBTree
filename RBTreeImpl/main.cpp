
#include "RBTreeImpl.h"
#include <iostream>

int main() {
    RedBlackTree::RBTreeImpl<int,std::string> tree;
    tree.insert(10,"hello");
    tree.insert(100,"world");
    tree.insert(200,"");
    tree.insert(120,"wild");
    tree.insert(40,"abc");
    tree.insert(32,"efg");

    for(auto i = tree.begin(); i != tree.end(); ++i){
        std::cout << std::get<0>(*i) << " " << std::get<1>(*i) << std::endl;
    }

    return 0;
}