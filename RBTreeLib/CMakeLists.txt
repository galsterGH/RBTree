cmake_minimum_required(VERSION 3.9)
project(RBTreeLib C)

set(CMAKE_C_STANDARD 11)

add_library(RBTreeLib ../src/RBIterator.c ../src/RBTree.c ../src/RBTree.h ../src/RBTreeImpl.h)
target_include_directories (RBTreeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
