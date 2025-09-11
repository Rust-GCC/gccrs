// Check projection of an associated type out of a higher-ranked trait-bound
// in the context of a struct definition.

pub trait Foo<T> {
    type A;

    fn get(&self, t: T) -> Self::A;
}

struct SomeStruct<I: for<'x> Foo<&'x isize>> {
    field: I::A
// { dg-error ".E0212." "" { target *-*-* } .-1 }
}

enum SomeEnum<'b, I: for<'a> Foo<&'a isize>> {
    TupleVariant(I::A),
// { dg-error ".E0212." "" { target *-*-* } .-1 }
    StructVariant { field: I::A },
// { dg-error ".E0212." "" { target *-*-* } .-1 }
    OkVariant(&'b usize),
}

// FIXME(eddyb) This one doesn't even compile because of the unsupported syntax.

// struct AnotherStruct<I : for<'x> Foo<&'x isize>> {
//     field: <I as for<'y> Foo<&'y isize>>::A
// }

struct YetAnotherStruct<'a, I: for<'x> Foo<&'x isize>> {
    field: <I as Foo<&'a isize>>::A,
}

struct Why<'a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i, 'j, 'k, 'n, 'o, 'p, 'q, 'r, 's, 't, 'u, 'v, 'w, 'x,
    'y, 'z, 'aa, I: for<'l, 'm> Foo<&'l &'m isize>> {
    field: I::A,
// { dg-error ".E0212." "" { target *-*-* } .-1 }
}

pub fn main() {}

