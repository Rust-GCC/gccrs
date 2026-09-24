// Checks that applied type arguments of enums, and aliases to them, are respected.
// For example, `Self` is never a type constructor. Therefore, no types can be applied to it.
//
// We also check that the variant to an type-aliased enum cannot be type applied whether
// that alias is generic or monomorphic.

enum Enum<T> { TSVariant(T), SVariant { v: T }, UVariant }
type Alias<T> = Enum<T>;
type AliasFixed = Enum<()>;

impl<T> Enum<T> {
    fn ts_variant() {
        Self::TSVariant(());
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        Self::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }
        Self::<()>::TSVariant(());
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        Self::<()>::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }
    }

    fn s_variant() {
        Self::SVariant { v: () };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        Self::SVariant::<()> { v: () };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        Self::<()>::SVariant { v: () };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        Self::<()>::SVariant::<()> { v: () };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    }

    fn u_variant() {
        Self::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
        Self::<()>::UVariant;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
        Self::<()>::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }
    }
}

fn main() {
    // Tuple struct variant

    Enum::<()>::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    Alias::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    Alias::<()>::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    AliasFixed::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    AliasFixed::<()>::TSVariant(());
// { dg-error ".E0107." "" { target *-*-* } .-1 }
    AliasFixed::<()>::TSVariant::<()>(());
// { dg-error ".E0109." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }

    // Struct variant

    Enum::<()>::SVariant::<()> { v: () };
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    Alias::SVariant::<()> { v: () };
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    Alias::<()>::SVariant::<()> { v: () };
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    AliasFixed::SVariant::<()> { v: () };
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    AliasFixed::<()>::SVariant { v: () };
// { dg-error ".E0107." "" { target *-*-* } .-1 }
    AliasFixed::<()>::SVariant::<()> { v: () };
// { dg-error ".E0109." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }

    // Unit variant

    Enum::<()>::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    Alias::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    Alias::<()>::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }

    AliasFixed::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
    AliasFixed::<()>::UVariant;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
    AliasFixed::<()>::UVariant::<()>;
// { dg-error ".E0109." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }
}

