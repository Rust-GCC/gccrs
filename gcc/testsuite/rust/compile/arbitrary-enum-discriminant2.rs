#![feature(no_core)]
#![no_core]

enum A1 {
    A = 12, // { dg-error "custom discriminant values are not allowed in enums with tuple or struct variants." }
    B(i32),
}

enum A2 {
    A = 12, // { dg-error "custom discriminant values are not allowed in enums with tuple or struct variants." }
    B { i: i32},
}

enum A3 {
    A(i32) = 12, // { dg-error "discriminants on non-unit variants are experimental." }
}

enum A4 {
    A { i: i32 } = 12, // { dg-error "discriminants on non-unit variants are experimental." }
}

fn main() -> i32 {
    0
}
