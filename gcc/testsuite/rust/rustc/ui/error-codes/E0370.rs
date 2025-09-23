#![allow(dead_code)]

#[deny(overflowing_literals)]
#[repr(i64)]
enum Foo {
    X = 0x7fffffffffffffff,
    Y, // { dg-error ".E0370." "" { target *-*-* } }
}

fn main() {}

