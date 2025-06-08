CC ?= gcc
CXX ?= g++
CFLAGS ?= -Wall -O2
CXXFLAGS ?= -Wall -O2 -std=c++17

SRC = src/RBTree.c src/RBIterator.c
OBJ = $(SRC:.c=.o)
LIB = librbtree.a

.PHONY: all clean test cpp

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB)
	$(CC) $(CFLAGS) -Isrc tests/test_rbtree.c $(LIB) -o rbtree_test

cpp: $(LIB)
	$(CXX) $(CXXFLAGS) RBTreeImpl/main.cpp $(LIB) -o rbtree_cpp

clean:
	rm -f $(OBJ) $(LIB) rbtree_test rbtree_cpp
