#![feature(no_core)]
#![no_core]

fn main() {
    let n = 2;
    let _a = [0u8; n]; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}
