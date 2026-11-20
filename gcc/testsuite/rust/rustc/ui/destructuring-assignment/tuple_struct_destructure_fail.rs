#![feature(destructuring_assignment)]

struct TupleStruct<S, T>(S, T);

enum Enum<S, T> {
    SingleVariant(S, T)
}

type Alias<S> = Enum<S, isize>;

trait Test {
    fn test() -> TupleStruct<isize, isize> {
        TupleStruct(0, 0)
    }
}

impl Test for Alias<isize> {}

fn test() -> TupleStruct<isize, isize> {
    TupleStruct(0, 0)
}

fn main() {
    let (mut a, mut b);
    TupleStruct(a, .., b, ..) = TupleStruct(0, 1);
// { dg-error "" "" { target *-*-* } .-1 }
    Enum::SingleVariant(a, .., b, ..) = Enum::SingleVariant(0, 1);
// { dg-error "" "" { target *-*-* } .-1 }

    TupleStruct(a, a, b) = TupleStruct(1, 2);
// { dg-error ".E0023." "" { target *-*-* } .-1 }
    Enum::SingleVariant(a, a, b) = Enum::SingleVariant(1, 2);
// { dg-error ".E0023." "" { target *-*-* } .-1 }

    // Check if `test` is recognized as not a tuple struct but a function call:
    test() = TupleStruct(0, 0);
// { dg-error ".E0070." "" { target *-*-* } .-1 }
    (test)() = TupleStruct(0, 0);
// { dg-error ".E0070." "" { target *-*-* } .-1 }
    <Alias::<isize> as Test>::test() = TupleStruct(0, 0);
// { dg-error ".E0070." "" { target *-*-* } .-1 }
}

