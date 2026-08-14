#![feature(extern_types)]

extern {
    type A;
    type B;
}

fn foo(r: &A) -> &B {
    r // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() { }

