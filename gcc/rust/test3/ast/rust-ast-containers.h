#ifndef RUST_AST_CONTAINERS_H
#define RUST_AST_CONTAINERS_H
// crappy redefined AST maybe. may move

/* This is mainly the "logical", more "abstract" representation of the code, while the "AST" itself is
 * more high-level and matches the language better. */

namespace Rust {
    namespace AST {
        struct Module {
          public:
        };

        struct Crate {
          public:
            Module root_module;
        };

        // TODO: replace with rust-types.h version
        struct AttributeList {
          public:
            //::std::vector<Attribute> attribs;
        };

        // TODO: replace with rust-types.h version
        struct Visibility {};

        /*enum VisibilityType {
            Private,
            PublicFull,
            PublicInPath,
            PublicCrate,
            PublicSuper,
            PublicSelfModule
        };

        // Represents visibility - maybe make into an enum or union or something
        struct Visibility {
        }; 

        */
    }
}

#endif