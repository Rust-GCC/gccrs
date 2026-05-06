// { dg-additional-options "-w" }
#![feature(no_core)]
#![no_core]

enum E {
    A,
    B,
    C
}

fn main() -> i32 {
    use E::*;

    match A {
        C => 1,
        _ => 0
    }
}
