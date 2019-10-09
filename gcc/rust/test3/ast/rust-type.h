#ifndef RUST_AST_TYPE_H
#define RUST_AST_TYPE_H

namespace Rust {
    namespace AST {
        // Abstract base class representing a type param bound - Lifetime and TraitBound extends it
        class TypeParamBound {};

        // Represents a lifetime (and is also a kind of type param bound)
        class Lifetime : public TypeParamBound {
            enum LifetimeType {
                NAMED,      // corresponds to LIFETIME_OR_LABEL
                STATIC,     // corresponds to 'static
                WILDCARD    // corresponds to '_
            } lifetime_type;

            // TODO: LIFETIME_OR_LABEL (aka lifetime token) is only field
            // find way of enclosing token or something
            ::std::string lifetime_name;
            // only applies for NAMED lifetime_type
        };

        // A trait bound 
        class TraitBound : public TypeParamBound {
            bool in_parens;
            bool opening_question_mark;

            bool has_for_lifetimes;
            ForLifetimes for_lifetimes;

            TypePath type_path;
        };

        // TODO: inline
        struct LifetimeBounds {
            ::std::vector<Lifetime> bounds;
        };

        // TODO: inline
        struct TypeParamBounds {
            ::std::vector<TypeParamBound> bounds;
        };

        // Base class for types as represented in AST - abstract
        class Type {};

        // A type without parentheses?
        class TypeNoBounds : public Type {};

        // An impl trait? Poor reference material here.
        class ImplTraitType : public Type {
            TypeParamBounds type_param_bounds;
        };

        // An opaque value of another type that implements a set of traits 
        class TraitObjectType : public Type {
            bool has_dyn;
            TypeParamBounds type_param_bounds;
        };

        // A type with parentheses around it, used to avoid ambiguity.
        class ParenthesisedType : public TypeNoBounds {
            Type type_in_parens;
        };

        // Impl trait with a single bound? Poor reference material here.
        class ImplTraitTypeOneBound : public TypeNoBounds {
            TraitBound trait_bound;
        };

        // A trait object with a single trait bound
        class TraitObjectTypeOneBound : public TypeNoBounds {
            bool has_dyn;
            TraitBound trait_bound;
        };

        // TODO: this the same TypePath as in rust-path.h.
        class TypePath; // definition moved to "rust-path.h"

        // A type consisting of the "product" of others (the tuple's elements) in a specific order
        class TupleType : public TypeNoBounds {
            ::std::vector<Type> elems;

            public:
            // Returns whether the tuple type is the unit type, i.e. has no elements.
            inline bool is_unit_type() const {
                return elems.empty();
            }
        };

        /* A type with no values, representing the result of computations that never complete.
         * Expressions of NeverType can be coerced into any other types. Represented as "!". */
        class NeverType : public TypeNoBounds {};

        // A type consisting of a pointer without safety or liveness guarantees
        class RawPointerType : public TypeNoBounds {
            enum PointerType {
                Mut,
                Const
            } pointer_type;

            TypeNoBounds type;

            public:
            // Returns whether the pointer is mutable or constant.
            inline PointerType get_pointer_type() const {
                return pointer_type;
            }
        };

        // A type pointing to memory owned by another value
        class ReferenceType : public TypeNoBounds {
            bool has_lifetime;
            Lifetime lifetime;

            bool has_mut;

            TypeNoBounds type;

            public:
            // Returns whether the reference is mutable or immutable.
            inline bool is_mut() const {
                return has_mut;
            }
        };

        // A fixed-size sequence of elements of a specified type
        class ArrayType : public TypeNoBounds {
            Type elem_type;
            Expr* size;

            public:
            ~ArrayType() {
                delete size;
            }
        };

        // A dynamically-sized type representing a "view" into a sequence of elements of a type
        class SliceType : public TypeNoBounds {
            Type elem_type;
        };

        // Type used in generic arguments to explicitly request type inference (wildcard pattern)
        class InferredType : public TypeNoBounds {
            // e.g. Vec<_> = whatever
        };

        // TODO: this is the same QualifiedPathInType as in "rust-path.h"
        class QualifiedPathInType; // definition moved to "rust-path.h"

        // TODO: inline as TypeNoBounds
        struct BareFunctionReturnType {
            TypeNoBounds type;
        };

        // A possibly named param used in a BaseFunctionType
        struct MaybeNamedParam {
            Type param_type;

            enum ParamType {
                UNNAMED,
                IDENTIFIER,
                WILDCARD
            } param_type;
            Identifier name; // technically, can be an identifier or '_'
        };

        /* A function pointer type - can be created via coercion from function items and non-
         * capturing closures. */
        class BareFunctionType : public TypeNoBounds {
            bool has_for_lifetimes;
            //ForLifetimes for_lifetimes;
            ::std::vector<LifetimeParam> for_lifetimes; // inlined version

            FunctionQualifiers function_qualifiers;
            ::std::vector<MaybeNamedParam> params;
            bool is_variadic;

            bool has_return_type;
            BareFunctionReturnType return_type;
        };

        // Forward decl - defined in rust-macro.h
        class MacroInvocation;

        /*// AST node of a macro invocation, which is replaced by the macro result at compile time
        class MacroInvocation : public TypeNoBounds, public Pattern, public ExprWithoutBlock {
            SimplePath path;
            DelimTokenTree token_tree;
        };*/

        // struct type?
        // "enum" (tagged union) type?
        // C-like union type?
        // function item type?
        // closure expression types?

        /* Incomplete spec references:
         *  anonymous type parameters, aka "impl Trait in argument position" - impl then trait bounds
         *  abstract return types, aka "impl Trait in return position" - impl then trait bounds */
    }
}

#endif