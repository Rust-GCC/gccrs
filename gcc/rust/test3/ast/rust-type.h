#ifndef RUST_AST_TYPE_H
#define RUST_AST_TYPE_H

namespace Rust {
    namespace AST {
        // Base class for types as represented in AST - abstract
        class Type {};

        // A type without parentheses?
        class TypeNoBounds : public Type {};

        class ImplTraitType : public Type {};

        class TraitObjectType : public Type {};

        // A type with parentheses around it, used to avoid ambiguity.
        class ParenthesisedType : public TypeNoBounds {
            Type type_in_parens;
        };

        class ImplTraitTypeOneBound : public TypeNoBounds {};

        class TraitObjectTypeOneBound : public TypeNoBounds {};

        class TypePath : public TypeNoBounds {};

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

        class InferredType : public TypeNoBounds {};

        class QualifiedPathInType : public TypeNoBounds {};

        class BareFunctionType : public TypeNoBounds {};

        class MacroInvocation : public TypeNoBounds {};

        // struct type?
        // "enum" (tagged union) type?
        // C-like union type?
        // function item type?
        // closure expression types?
    }
}

#endif