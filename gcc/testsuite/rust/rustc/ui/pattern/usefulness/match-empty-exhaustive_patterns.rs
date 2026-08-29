#![feature(never_type)]
#![feature(exhaustive_patterns)]
#![deny(unreachable_patterns)]
enum Foo {}

struct NonEmptyStruct(bool); // { dg-error "" "" { target *-*-* } }
union NonEmptyUnion1 { // { dg-error "" "" { target *-*-* } }
    foo: (),
}
union NonEmptyUnion2 { // { dg-error "" "" { target *-*-* } }
    foo: (),
    bar: (),
}
enum NonEmptyEnum1 { // { dg-error "" "" { target *-*-* } }
    Foo(bool),
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}
enum NonEmptyEnum2 { // { dg-error "" "" { target *-*-* } }
    Foo(bool),
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    Bar,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}
enum NonEmptyEnum5 { // { dg-error "" "" { target *-*-* } }
    V1, V2, V3, V4, V5,
}

macro_rules! match_empty {
    ($e:expr) => {
        match $e {}
    };
}
macro_rules! match_false {
    ($e:expr) => {
        match $e {
            _ if false => {}
        }
    };
}

fn foo(x: Foo) {
    match_empty!(x); // ok
    match x {
        _ => {}, // { dg-error "" "" { target *-*-* } }
    }
    match x {
        _ if false => {}, // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {
    match None::<!> {
        None => {}
        Some(_) => {} // { dg-error "" "" { target *-*-* } }
    }
    match None::<Foo> {
        None => {}
        Some(_) => {} // { dg-error "" "" { target *-*-* } }
    }

    match_empty!(0u8);
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!(NonEmptyStruct(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!((NonEmptyUnion1 { foo: () }));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!((NonEmptyUnion2 { foo: () }));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!(NonEmptyEnum1::Foo(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!(NonEmptyEnum2::Foo(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_empty!(NonEmptyEnum5::V1);
// { dg-error ".E0004." "" { target *-*-* } .-1 }

    match_false!(0u8);
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!(NonEmptyStruct(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!((NonEmptyUnion1 { foo: () }));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!((NonEmptyUnion2 { foo: () }));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!(NonEmptyEnum1::Foo(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!(NonEmptyEnum2::Foo(true));
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match_false!(NonEmptyEnum5::V1);
// { dg-error ".E0004." "" { target *-*-* } .-1 }
}

