#ifndef RUST_AST_H
#define RUST_AST_H
// crappy redefined AST maybe. may move

namespace Rust {
    namespace AST {
        struct Module {
          public:
        };

        struct Crate {
          public:
            Module root_module;
        };
    }
}

#endif