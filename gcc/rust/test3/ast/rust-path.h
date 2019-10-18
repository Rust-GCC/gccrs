#ifndef RUST_AST_PATH_H
#define RUST_AST_PATH_H
/* "Path" (identifier within namespaces, essentially) handling. Required include for virtually all
 * AST-related functionality. */

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

        // The "identifier" (not generic args) aspect of each path expression segment
        class PathIdentSegment {
            ::std::string segment_name;

            // only allow identifiers, "super", "self", "Self", "crate", or "$crate"
          public:
            PathIdentSegment(::std::string segment_name) : segment_name(segment_name) {}

            // TODO: insert check in constructor for this?
        };

        // A binding of an identifier to a type used in generic arguments in paths
        struct GenericArgsBinding {
          private:
            Identifier identifier;
            // Type type;
            ::gnu::unique_ptr<Type> type;

          public:
            // Pointer type for type in constructor to enable polymorphism
            GenericArgsBinding(Identifier ident, Type* type_ptr) :
              identifier(ident), type(type_ptr) {}

            // Copy constructor has to deep copy the type as it is a unique pointer
            GenericArgsBinding(GenericArgsBinding const& other) :
              identifier(other.identifier), type(other.type->clone_type()) {}

            // default destructor
            ~GenericArgsBinding() = default;

            // Overload assignment operator to deep copy the pointed-to type
            GenericArgsBinding& operator=(GenericArgsBinding const& other) {
                identifier = other.identifier;
                type = other.type->clone_type();
                return *this;
            }

            // no move constructors as not supported in c++03
            /*GenericArgsBinding(GenericArgsBinding&& other) = default;
            GenericArgsBinding& operator=(GenericArgsBinding&& other) = default;*/
        };

        // Generic arguments allowed in each path expression segment - inline?
        struct GenericArgs {
            ::std::vector<Lifetime> lifetime_args;
            //::std::vector<Type> type_args;
            ::std::vector< ::gnu::unique_ptr<Type> > type_args;
            ::std::vector<GenericArgsBinding> binding_args;

          public:
            // Returns true if there are any generic arguments
            inline bool has_generic_args() const {
                return !(lifetime_args.empty() && type_args.empty() && binding_args.empty());
            }

            GenericArgs(::std::vector<Lifetime> lifetime_args,
              ::std::vector< ::gnu::unique_ptr<Type> > type_args,
              ::std::vector<GenericArgsBinding> binding_args) :
              lifetime_args(lifetime_args),
              type_args(type_args), binding_args(binding_args) {}

            ~GenericArgs() = default;

            // Creates an empty GenericArgs (no arguments)
            static GenericArgs empty() {
                ::std::vector<Lifetime> empty_lifetime_args;
                ::std::vector< ::gnu::unique_ptr<Type> > empty_type_args;
                ::std::vector<GenericArgsBinding> empty_binding_args;

                return GenericArgs(empty_lifetime_args, empty_type_args, empty_binding_args);
            }
        };

        // A segment of a path in expression, including an identifier aspect and maybe generic args
        class PathExprSegment { // or should this extend PathIdentSegment?
          private:
            PathIdentSegment segment_name;

            // bool has_generic_args;
            GenericArgs generic_args;

          public:
            // Returns true if there are any generic arguments
            inline bool has_generic_args() const {
                return generic_args.has_generic_args();
            }

            // Constructor for segment without generic arguments (from IdentSegment)
            PathExprSegment(PathIdentSegment segment_name) :
              segment_name(segment_name), generic_args(GenericArgs::empty()) {}

            // Constructor for segment with generic arguments (from IdentSegment and GenericArgs)
            PathExprSegment(PathIdentSegment segment_name, GenericArgs generic_args) :
              segment_name(segment_name), generic_args(generic_args) {}

            // Constructor for segment without generic arguments (from segment name itself)
            PathExprSegment(::std::string segment_name) :
              segment_name(PathIdentSegment(segment_name)), generic_args(GenericArgs::empty()) {}

            // Constructor for segment with generic arguments (from segment name and all args)
            PathExprSegment(::std::string segment_name, ::std::vector<Lifetime> lifetime_args,
              ::std::vector< ::gnu::unique_ptr<Type> > type_args,
              ::std::vector<GenericArgsBinding> binding_args) :
              segment_name(PathIdentSegment(segment_name)),
              generic_args(GenericArgs(lifetime_args, type_args, binding_args)) {}
        };

        // AST node representing a pattern that involves a "path" - abstract base class
        class PathPattern : public Pattern {
          protected:
            ::std::vector<PathExprSegment> segments;

            PathPattern(::std::vector<PathExprSegment> segments) : segments(segments) {}
        };

        // AST node representing a path-in-expression pattern (path that allows generic arguments)
        class PathInExpression : public PathPattern {
            bool has_opening_scope_resolution;

          public:
            ::std::string as_string() const;

            // Constructor with potentially an opening scope resolution
            PathInExpression(
              ::std::vector<PathExprSegment> path_segments, bool has_opening_scope_resolution) :
              has_opening_scope_resolution(has_opening_scope_resolution),
              PathPattern(segments) {}

            // Constructor with no opening scope resolution
            PathInExpression(::std::vector<PathExprSegment> path_segments) :
              has_opening_scope_resolution(false), PathPattern(segments) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual PathInExpression* clone_pattern_impl() const OVERRIDE {
                return new PathInExpression(*this);
            }
        };

        // Abstract base class for segments used in type paths
        class TypePathSegment {
          protected:
            PathIdentSegment ident_segment;
            bool has_separating_scope_resolution;

            // Protected constructor for initialising base class members
            TypePathSegment(PathIdentSegment ident_segment, bool has_separating_scope_resolution) :
              ident_segment(ident_segment),
              has_separating_scope_resolution(has_separating_scope_resolution) {}

            // Protected constructor for initialising base class members (from segment name)
            TypePathSegment(::std::string segment_name, bool has_separating_scope_resolution) :
              ident_segment(PathIdentSegment(segment_name)),
              has_separating_scope_resolution(has_separating_scope_resolution) {}

          public:
            virtual ~TypePathSegment() {}
        };

        // Segment used in type path with generic args
        class TypePathSegmentGeneric : public TypePathSegment {
            GenericArgs generic_args;

          public:
            inline bool has_generic_args() const {
                return generic_args.has_generic_args();
            }

            // Constructor with PathIdentSegment and GenericArgs
            TypePathSegmentGeneric(PathIdentSegment ident_segment,
              bool has_separating_scope_resolution, GenericArgs generic_args) :
              generic_args(generic_args),
              TypePathSegment(ident_segment, has_separating_scope_resolution) {}

            // Constructor from segment name and all args
            TypePathSegmentGeneric(::std::string segment_name, bool has_separating_scope_resolution,
              ::std::vector<Lifetime> lifetime_args,
              ::std::vector< ::gnu::unique_ptr<Type> > type_args,
              ::std::vector<GenericArgsBinding> binding_args) :
              generic_args(GenericArgs(lifetime_args, type_args, binding_args)),
              TypePathSegment(segment_name, has_separating_scope_resolution) {}
        };

        // TODO: inline
        /*struct TypePathFnInputs {
            //::std::vector<Type> inputs;
            ::std::vector< ::gnu::unique_ptr<Type> > inputs;
        };*/

        // A function as represented in a type path
        struct TypePathFn {
            // TODO: remove
            /*bool has_inputs;
            TypePathFnInputs inputs;*/
            //::std::vector<Type> inputs; // inlined from TypePathFnInputs
            ::std::vector< ::gnu::unique_ptr<Type> > inputs;

            // bool has_type;
            // Type type;
            ::gnu::unique_ptr<Type> type;

          public:
            // Returns whether the type of the function has been specified.
            inline bool has_type() const {
                return type != NULL;
            }

            // Returns whether the function has inputs.
            inline bool has_inputs() const {
                return !inputs.empty();
            }

            // Constructor for function with no specified type.
            TypePathFn(::std::vector< ::gnu::unique_ptr<Type> > inputs) :
              inputs(inputs)/*, type(NULL)*/ {}

            // Constructor for function with a specified type.
            TypePathFn(::std::vector< ::gnu::unique_ptr<Type> > inputs, Type* type) :
              inputs(inputs), type(type) {}

            // Copy constructor with clone
            TypePathFn(TypePathFn const& other) :
              inputs(other.inputs), type(other.type->clone_type()) {}

            ~TypePathFn() = default;

            // Overloaded assignment operator to clone type
            TypePathFn& operator=(TypePathFn const& other) {
                inputs = other.inputs;
                type = other.type->clone_type();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*TypePathFn(TypePathFn&& other) = default;
            TypePathFn& operator=(TypePathFn&& other) = default;*/
        };

        // Segment used in type path with a function argument
        class TypePathSegmentFunction : public TypePathSegment {
            TypePathFn function_path;

          public:
            // Constructor with PathIdentSegment and TypePathFn
            TypePathSegmentFunction(PathIdentSegment ident_segment,
              bool has_separating_scope_resolution, TypePathFn function_path) :
              function_path(function_path),
              TypePathSegment(ident_segment, has_separating_scope_resolution) {}

            // Constructor with segment name and TypePathFn
            TypePathSegmentFunction(::std::string segment_name, bool has_separating_scope_resolution,
              TypePathFn function_path) :
              function_path(function_path),
              TypePathSegment(segment_name, has_separating_scope_resolution) {}
        };

        // Path used inside types
        class TypePath : public TypeNoBounds {
            bool has_opening_scope_resolution;
            ::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual TypePath* clone_type_impl() const OVERRIDE {
                return new TypePath(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual TypePath* clone_type_no_bounds_impl() const OVERRIDE {
                return new TypePath(*this);
            }

          public:
            /* Returns whether the TypePath has an opening scope resolution operator (i.e. is global
             * path or crate-relative path, not module-relative) */
            inline bool has_opening_scope_resolution_op() const {
                return has_opening_scope_resolution;
            }

            // Constructor for TypePath with TypePathSegments and maybe an opening scope resolution
            TypePath(::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments,
              bool has_opening_scope_resolution) :
              segments(segments),
              has_opening_scope_resolution(has_opening_scope_resolution) {}

            // Constructor for TypePath with TypePathSegments and no opening scope resolution
            TypePath(::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments) :
              segments(segments), has_opening_scope_resolution(false) {}
        };

        struct QualifiedPathType {
          private:
            // Type type_to_invoke_on;
            ::gnu::unique_ptr<Type> type_to_invoke_on;

            bool has_as_clause;
            TypePath trait_path;

          public:
            // Constructor uses Type pointer for polymorphism
            QualifiedPathType(Type* invoke_on_type, bool has_as_clause_, TypePath trait_path_) :
              type_to_invoke_on(invoke_on_type), has_as_clause(has_as_clause_),
              trait_path(trait_path_) {}

            // Copy constructor uses custom deep copy for Type to preserve polymorphism
            QualifiedPathType(QualifiedPathType const& other) :
              type_to_invoke_on(other.type_to_invoke_on->clone_type()),
              has_as_clause(other.has_as_clause), trait_path(other.trait_path) {}

            // default destructor
            ~QualifiedPathType() = default;

            // overload assignment operator to use custom clone method
            QualifiedPathType& operator=(QualifiedPathType const& other) {
                type_to_invoke_on = other.type_to_invoke_on->clone_type();
                has_as_clause = other.has_as_clause;
                trait_path = other.trait_path;
                return *this;
            }

            // default move semantics but no move in c++03
            /*QualifiedPathType(QualifiedPathType&& other) = default;
            QualifiedPathType& operator=(QualifiedPathType&& other) = default;*/
        };

        /* AST node representing a qualified path-in-expression pattern (path that allows specifying
         * trait functions) */
        class QualifiedPathInExpression : public PathPattern {
            QualifiedPathType path_type;

          public:
            ::std::string as_string() const;

            QualifiedPathInExpression(
              QualifiedPathType qual_path_type, ::std::vector<PathExprSegment> path_segments) :
              path_type(qual_path_type),
              PathPattern(path_segments) {}

            // TODO: maybe make a shortcut constructor that has QualifiedPathType elements as params

            // Copy constructor, destructor, and assignment operator overload shouldn't be required
          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual QualifiedPathInExpression* clone_pattern_impl() const OVERRIDE {
                return new QualifiedPathInExpression(*this);
            }
        };

        // Represents a qualified path in a type; used for disambiguating trait function calls
        class QualifiedPathInType : public TypeNoBounds {
            QualifiedPathType path_type;
            // ::std::vector<TypePathSegment> segments;
            ::std::vector< ::gnu::unique_ptr<TypePathSegment> > segments;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual QualifiedPathInType* clone_type_impl() const OVERRIDE {
                return new QualifiedPathInType(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual QualifiedPathInType* clone_type_no_bounds_impl() const OVERRIDE {
                return new QualifiedPathInType(*this);
            }

          public:
            QualifiedPathInType(QualifiedPathType qual_path_type,
              ::std::vector< ::gnu::unique_ptr<TypePathSegment> > path_segments) :
              path_type(qual_path_type),
              segments(path_segments) {}

            // TODO: maybe make a shortcut constructor that has QualifiedPathType elements as params

            // Copy constructor, destructor, and assignment operator overload shouldn't be required
        };
    }
}

#endif