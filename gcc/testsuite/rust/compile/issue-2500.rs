#![allow(unused)]
fn main() {
struct Foo {
    x: i32
}

impl *mut Foo {}
// { dg-error "cannot define inherent `impl` for primitive types" "" { target *-*-* } .-1 }
}