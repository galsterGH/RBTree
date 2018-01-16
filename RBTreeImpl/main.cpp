
#include "RBTreeImpl.h"

int main() {
    RedBlackTree::RBTreeImpl<int,std::string> tree;
    tree.insert(10,"hello");
    tree.insert(100,"world");
    tree.insert(200,"");
    tree.insert(120,"wild");
    tree.insert(40,"abc");
    tree.insert(32,"efg");
    tree.del(10);
    tree.del(32);
    tree.del(40);
    return 0;
}