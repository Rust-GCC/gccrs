#ifndef RUST_AST_PATH_H
#define RUST_AST_PATH_H
// "Path" (identifier within namespaces, essentially) handling

#include "rust-ast.h"

#include <string>
#include <vector>

namespace Rust {
    namespace AST {
        // make intellisense calm
        /*typedef ::std::string Symbol;
        typedef int Lifetime;
        typedef int Type;
        typedef int Binding;*/
        
        // forward decls:
        class TypePath;

        // TODO: move applicable stuff into here
        // A segment of a path (maybe)
        class PathSegment {};

        // A segment of a simple path without generic or type arguments
        class SimplePathSegment : public PathSegment {
            ::std::string segment_name;

            // only allow identifiers, "super", "self", "crate", or "$crate"
        };

        // A simple path without generic or type arguments
        class SimplePath {
            bool has_opening_scope_resolution;
            ::std::vector<SimplePathSegment> segments;
        };

        // The "identifier" (not generic args) aspect of each path expression segment
        class PathIdentSegment {
            ::std::string segment_name;

            // only allow identifiers, "super", "self", "Self", "crate", or "$crate"
        };

        // A binding of an identifier to a type used in generic arguments in paths
        struct GenericArgsBinding {
            Identifier identifier;
            //Type type;
            ::gnu::unique_ptr<Type> type;
        };

        // Generic arguments allowed in each path expression segment
        struct GenericArgs {
            ::std::vector<Lifetime> lifetime_args;
            //::std::vector<Type> type_args;
            ::std::vector< ::gnu::unique_ptr<Type> > type_args;
            ::std::vector<GenericArgsBinding> binding_args;
        };

        // A segment of a path in expression, including an identifier aspect and maybe generic args
        class PathExprSegment { // or should this extend PathIdentSegment?
            PathIdentSegment segment_name;

            bool has_generic_args;
            GenericArgs generic_args;
        };

        // AST node representing a path-in-expression pattern (path that allows generic arguments)
        class PathInExpression : public PathPattern {
            bool has_opening_scope_resolution;

          public:
            ::std::string as_string() const;
        };

        struct QualifiedPathType {
            //Type type_to_invoke_on;
            ::gnu::unique_ptr<Type> type_to_invoke_on;

            bool has_as_clause;
            TypePath trait_path;
        };

        /* AST node representing a qualified path-in-expression pattern (path that allows specifying
         * trait functions) */
        class QualifiedPathInExpression : public PathPattern {
            QualifiedPathType path_type;

          public:
            ::std::string as_string() const;
        };

        // Abstract base class for segments used in type paths
        class TypePathSegment {
            PathIdentSegment ident_segment;
            bool has_separating_scope_resolution;
        };

        // Segment used in type path with generic args
        class TypePathSegmentGeneric : public TypePathSegment {
            GenericArgs generic_args;
        };

        // Represents a qualified path in a type; used for disambiguating trait function calls
        class QualifiedPathInType : public TypeNoBounds {
            QualifiedPathType path_type;
            // ::std::vector<TypePathSegment> segments;
            ::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments;
        };

        // TODO: inline
        struct TypePathFnInputs {
            //::std::vector<Type> inputs;
            ::std::vector< ::gnu::unique_ptr<Type> > inputs;
        };

        // A function as represented in a type path
        struct TypePathFn {
            // TODO: remove
            /*bool has_inputs;
            TypePathFnInputs inputs;*/
            //::std::vector<Type> inputs; // inlined from TypePathFnInputs
            ::std::vector< ::gnu::unique_ptr<Type> > inputs;

            bool has_type;
            //Type type;
            ::gnu::unique_ptr<Type> type;
        };

        // Segment used in type path with a function argument
        class TypePathSegmentFunction : public TypePathSegment {
            TypePathFn function_path;
        };

        // Path used inside types
        class TypePath : public TypeNoBounds {
            bool has_opening_scope_resolution;
            ::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments;
        };
    }
}

#endif