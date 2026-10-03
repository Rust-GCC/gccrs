#![feature(type_ascription)]

enum Bug<S> {
    Var = 0: S,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {}

