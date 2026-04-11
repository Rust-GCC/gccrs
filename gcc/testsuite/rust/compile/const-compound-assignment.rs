// { dg-options "-w -O0 -fdump-tree-gimple" }
#![feature(no_core)]
#![no_core]

const fn test(mut x: i32) -> i32 {
    x += 5;
    x
}

const X: i32 = test(10);

fn main() {
    // { dg-final { scan-tree-dump-times {x = 15} 1 gimple } }
    let x = X;
}