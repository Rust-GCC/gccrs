#![deny(dead_code)]

#[derive(Clone)]
enum Enum {
    Variant1, // { dg-error "" "" { target *-*-* } }
    Variant2,
}

fn main() {
    let e = Enum::Variant2;
    e.clone();
}

