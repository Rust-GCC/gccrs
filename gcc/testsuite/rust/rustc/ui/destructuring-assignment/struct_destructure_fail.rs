#![feature(destructuring_assignment)]
struct Struct<S, T> {
    a: S,
    b: T,
}

fn main() {
    let (mut a, b);
    let mut c;
    let d = Struct { a: 0, b: 1 };
    Struct { a, b, c } = Struct { a: 0, b: 1 }; // { dg-error ".E0026." "" { target *-*-* } }
    Struct { a, ..d } = Struct { a: 1, b: 2 };
// { dg-error "" "" { target *-*-* } .-1 }
    Struct { a, .. }; // { dg-error "" "" { target *-*-* } }
}

