// modulus check for integers when 
// i32::MAX is divided by -1
// divisor is 0

// RUNTIME CHECK:
// { dg-shouldfail "i32::MIN % -1 overflows and division by zero" }
// { dg-options "-fdump-tree-original" }

#![feature(no_core)]
#![no_core]

fn modulus_fn(a: i32, b: i32) -> i32 {
    // { dg-final { scan-tree-dump {== -2147483648} original } }
    // { dg-final { scan-tree-dump {== -1} original } }
    // { dg-final { scan-tree-dump {== 0} original } }
    a % b
}

fn main() {
    let a = -1;
    let b = -2147483648;

    let _res = modulus_fn(b, a);
    let _res2 = modulus_fn(5, 0);
}
