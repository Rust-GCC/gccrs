#![deny(unreachable_patterns)]

#[derive(PartialEq)]
struct Opaque(i32);

impl Eq for Opaque {}

const FOO: Opaque = Opaque(42);

fn main() {
    match FOO {
        FOO => {},
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

