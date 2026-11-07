// modulus check for integers when
// i32::MIN is divided by -1
// divisor is 0

// RUNTIME CHECK:
// { dg-shouldfail "i32::MIN % -1 overflows" }
// { dg-options "-fdump-tree-original" }

#![feature(no_core)]
#![no_core]

fn modulus_fn(a: i32, b: i32) -> i32 {
    a % b
}

fn main() {
    let a = -1;
    let b = -2147483648;

    let _res = modulus_fn(b, a);
}
