#ifndef RUST_PATH_H
#define RUST_PATH_H
// Path handling maybe?

#include <vector>

namespace Rust {
    namespace AST {
        struct GenericArgs {

        };

        struct GenericsArgsAngle : public GenericArgs {

        };

        struct GenericArgsParen : public GenericArgs {
            
        };

        struct SimplePathSegment {
            Symbol identifier;
            //vector<GenericArgs> generic_args;
        };

        struct SimplePath {
            // global paths begin with something else
            vector<SimplePathSegment> segments;
        };

        struct PathSegment {
            Symbol identifier;
            vector<GenericArgs> generic_args;
        };

        struct Path {
            // global paths begin with something else
            vector<SimplePathSegment> segments;
        };
    }
}

#endif