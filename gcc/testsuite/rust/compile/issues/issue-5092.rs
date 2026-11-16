#![feature(no_core)]
#![no_core]

// Rust-GCC/gccrs#5092: a local in an array length printed a broken
// "non const capacity domain '%T'" instead of E0435.

const N: usize = 2;

fn f(p: usize) {
    let n = 2;
    let _a = [0u8; n]; // { dg-error "attempt to use a non-constant value in a constant" }
    let _b = [0u8; p]; // { dg-error "attempt to use a non-constant value in a constant" }
    let _c = [0u8; N];
}

fn main() {
    f(1);
}
