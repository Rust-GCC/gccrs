// See issue #12534.

#![feature(bindings_after_at)]

fn main() {}

struct A(Box<u8>);

fn f(a @ A(u): A) -> Box<u8> {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    drop(a);
    u
}

