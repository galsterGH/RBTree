# RBTree

A small portable implementation of a generic red‑black tree written in C with a lightweight C++17 wrapper.

## Layout

- `src/` &ndash; C implementation of the tree and iterator.
- `RBTreeLib/` &ndash; example static library project.
- `RBTreeImpl/` &ndash; C++ wrapper demonstrating RAII usage.

## Building

A simple `Makefile` is provided for Linux and macOS users.

```bash
make            # build static library `librbtree.a`
make test       # build the C test harness
make cpp        # build the C++ example program
```

The Makefile uses `gcc` and `g++` by default but respects the `CC` and `CXX` environment variables.

## Usage

Include `src/RBTree.h` and link against `librbtree.a`. The API exposes basic operations such as `insert`, `del`, `find` and iteration.

The C++ wrapper in `RBTreeImpl/` offers an STL-like interface using the same underlying C implementation.


