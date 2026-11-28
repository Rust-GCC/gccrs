#![feature(no_core)]
#![no_core]

// a const or static inside a function can't use the function's parameters,
// even when nothing uses the const

fn param(p: i32) {
    const Y: i32 = p; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn param_static(p: i32) {
    static Z: i32 = p; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn used_twice(p: i32) -> i32 {
    const W: i32 = p; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
    W + W
}

fn main() {}
