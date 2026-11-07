// modulus check for integers when
// i32::MIN is divided by -1
// divisor is 0

// { dg-options "-fdump-tree-original" }

#![feature(no_core)]
#![no_core]

fn modulus_fn(a: i32, b: i32) -> i32 {
    // { dg-final { scan-tree-dump {== -2147483648} original } }
    // { dg-final { scan-tree-dump {== -1} original } }
    // { dg-final { scan-tree-dump {== 0} original } }
    a % b
}

fn main() -> i32 {
    let a = 5;
    let b = 11;
    let res = modulus_fn(b, a);
    if res != 1 {
        return 1;
    }
    0
}
