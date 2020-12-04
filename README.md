![C/C++ CI](https://github.com/philberty/gccrs/workflows/C/C++%20CI/badge.svg)
![Docker Build](https://img.shields.io/docker/cloud/build/philberty/gccrs)
![Docker Pulls](https://img.shields.io/docker/pulls/philberty/gccrs)
# GCC Rust

This is a full alternative implementaion of the Rust language ontop of GCC.
Please see the accompanying reporting repository: https://github.com/Rust-GCC/Reporting for
status reports from Philip Herron and tast tracking overviews.

## Development Enviroment

Fetch dependancies for ubuntu:

```bash
$ apt install build-essential libgmp3-dev libmpfr-dev libmpc-dev flex bison autogen gcc-multilib
```

Clone the repository

```bash
$ git clone git@github.com:Rust-GCC/gccrs.git
```

Compilation script. It is important to remember that GNU toolchain projects are designed to be built outside of its source directory
this is why a build directory is created.

```bash
$ mkdir gccrs-build
$ cd gccrs-build
$ ../gccrs/configure --prefix=$HOME/gccrs-install --disable-bootstrap --enable-multilib --enable-languages=rust
$ make
```

Running the compiler itself without make install we can simply imvoke the compiler proper:

```
$ gdb --args ./gcc/rust1 test.rs -frust-dump-parse -Warray-bounds -dumpbase test.rs -mtune=generic -march=x86-64 -O0 -version -fdump-tree-gimple -o test.s -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64
```

Invoking the compiler driver we need to:

```
$ make install
```

Then invoke the compiler as expected:

```
$ gccrs -g -O2 -c test.rs -o test.o
$ gccrs -o test test.o
```

## Testsuite

The test suite can be invoked via:

```
$ make check-rust
```

Test cases can be found within gcc/testsuite/rust.test please feel free to contriobute your specific
test cases referencing any issues on github.

## Docker image

There is a docker image hosted over on: 

https://hub.docker.com/repository/docker/philberty/gccrs

Or you can build your own image:

```
$ docker build . -t gccrs-dev
$ docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp \
    gccrs-dev:latest gccrs -g -O2 -c \
    gcc/testsuite/rust.test/compilable/type_infer1.rs -o type_infer1.o
```

## Contributing

Please be aware this project is designed to be pushed upstream to GCC when we reach some milestones and this means we require
contribtions to have copyright assignment in place. Please see: https://gcc.gnu.org/contribute.html

Not all contributions must be code, we would love to see new test cases or bugs and issues to be reported. Feel free to add any comments on open PRs

## Community

We can be found on all usual Rust channels such as Zulip but we also have our own channels:

 * GCC Rust Zulip: https://gcc-rust.zulipchat.com/
 * Twitter: https://twitter.com/gcc_rust
