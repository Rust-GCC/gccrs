#![feature(no_core)]
#![no_core]

#[repr(u8)]
enum Foo {
    A = 256, // { dg-error "integer overflows the respective type .u8." }
}

#[repr(u8)]
enum Bar {
    A = 100 + 200, // { dg-error {attempt to compute .100 \+ 200., which would overflow \[E0080\]} }
}

#[repr(u8)]
enum Baz {
    A = 255,
    B, // { dg-error "enum discriminant overflowed .E0370." }
}

#[repr(i8)]
enum Qux {
    A = 127,
    B, // { dg-error "enum discriminant overflowed .E0370." }
}
fn main() {}
