#![feature(no_core)]
#![no_core]
#![feature(arbitrary_enum_discriminant)]

#[repr(u32)]
enum SingleVTuple {
    A(i32) = 12,
}

#[repr(C)]
enum SingleVTupleC {
    A(i32) = 12,
}

#[repr(u32)]
enum SingleVStruct {
    A {
        i: i32,
        j: i32,
    } = 12,
}

#[repr(u32)]
enum MultiVTuple {
    A(i32) = 12,
    B(u32) = 0,
}

#[repr(u32)]
enum MultiVStruct {
    A {
        i: i32,
        j: i32,
    } = 12,
    B {
        i: u32,
        j: u32,
    } = 0,
}

#[repr(u32)]
enum E1 {
    A = 12,
    B = 13,
    C(i32),
}

enum E1Invalid { // { dg-error "..repr.inttype... must be specified .E0732." }
    A = 12,
    B = 13,
    C(i32)
}

enum E2 {
    A = 12,
    B,
    C = 15,
    D,
    E,
    F = 14,
}

enum E2Invalid {
    A = 12,
    B,      // 13
    C = 15,
    D,      // 16
    E,      // 17
    F = 13, // { dg-error "discriminant value .13. already exists .E0081." }
}

enum E3Invalid {
    A = 1,
    B = 0,
    C, // { dg-error "discriminant value .1. already exists .E0081." }
}

enum E4Invalid {
    A = 9223372036854775807,
    B, // { dg-error "enum discriminant overflowed .E0370." }
}

macro_rules! ten { () => { 10 } }

#[repr(u32)]
enum EMacro {
    Bar(i32) = ten!(),
}

fn main() -> i32 {
    0
}
