#![deny(unused)]

struct F;
struct B;

enum E {
    Foo(F),
    Bar(B), // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let _ = E::Foo(F);
}

