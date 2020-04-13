![C/C++ CI](https://github.com/philberty/gccrs/workflows/C/C++%20CI/badge.svg)
# GCC Rust

This is an implementation of rust following the gccgo style front-end.

## Compilation for Development

```bash
$ apt install build-essential libgmp3-dev libmpfr-dev libmpc-dev flex bison
$ git clone https://github.com/philberty/gccrs
$ mkdir gccrs-build
$ cd gccrs-build
$ ../gccrs/configure --prefix=$HOME/gccrs-install --disable-bootstrap --disable-multilib --enable-languages=c,c++,rust
$ make
$ gdb --args ./gcc/rust1 test.rs -quiet -dumpbase test.rs -mtune=generic -march=x86-64 -auxbase-strip test.s -O0 -version -fdump-tree-gimple -o test.s -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64
```
