#![feature(no_core)]
#![no_core]

// consts and const blocks in a function are evaluated even when unused

fn f() {
    const X: u8 = 255 + 1; // { dg-error "which would overflow" }
    let _ = const { 255u8 + 1 }; // { dg-error "which would overflow" }
}

fn main() {}
