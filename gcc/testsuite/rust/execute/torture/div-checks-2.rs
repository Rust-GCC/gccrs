// division check for integers when
// i32::MIN is divided by -1
// divisor is 0

// RUNTIME CHECK 2:
// { dg-shouldfail "division by zero" }
// { dg-options "-fdump-tree-original" }

#![feature(no_core)]
#![no_core]

fn divide_fn(a: i32, b: i32) -> i32 {
    a / b
}

fn main() {
    let _res1 = divide_fn(5, 0);
}

// Since division by zero can also come via a hardware trap we check for
// { dg-final { scan-tree-dump-times {__builtin_abort} 1 original } }
