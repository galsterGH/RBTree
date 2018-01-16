cmake_minimum_required(VERSION 3.9)
project(RBTreeImpl)

set(CMAKE_CXX_STANDARD 17)
add_executable(RBTreeImpl main.cpp)
target_link_libraries(RBTreeImpl LINK_PUBLIC RBTreeLib)