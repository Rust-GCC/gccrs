#ifndef RUST_TYPE_H
#define RUST_TYPE_H

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

        class TupleType : public TypeNoBounds {};

        /* A type with no values, representing the result of computations that never complete. 
         * Expressions of NeverType can be coerced into any other types. Represented as "!". */
        class NeverType : public TypeNoBounds {};

        class RawPointerType : public TypeNoBounds {};

        class ReferenceType : public TypeNoBounds {};

        class ArrayType : public TypeNoBounds {};

        class SliceType : public TypeNoBounds {};

        class InferredType : public TypeNoBounds {};

        class QualifiedPathInType : public TypeNoBounds {};

        class BareFunctionType : public TypeNoBounds {};

        class MacroInvocation : public TypeNoBounds {};
    }
}

#endif