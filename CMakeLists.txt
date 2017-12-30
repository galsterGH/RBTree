cmake_minimum_required(VERSION 3.9)
project(RBTree C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES  RBTree.c RBTreeTester.c)
add_executable(RBTree ${SOURCE_FILES})
