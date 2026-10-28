// modulus check for integers when
// i32::MIN is divided by -1
// divisor is 0

// RUNTIME CHECK 2:
// { dg-shouldfail "modulo 0" }
// { dg-options "-fdump-tree-original" }

#![feature(no_core)]
#![no_core]

fn modulus_fn(a: i32, b: i32) -> i32 {
    a % b
}

fn main() {
    let _res = modulus_fn(5, 0);
}
