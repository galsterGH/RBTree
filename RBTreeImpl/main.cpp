
#include "RBTreeImpl.h"

int main() {
    RedBlackTree::RBTreeImpl<int> tree;
    tree.insert(10);
    tree.insert(100);
    tree.insert(200);
    tree.insert(120);
    tree.insert(40);
    tree.insert(32);
    tree.del(10);
    tree.del(32);
    tree.del(40);
    return 0;
}