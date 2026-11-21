#![feature(no_core)]
#![no_core]

// Rust-GCC/gccrs#5108: a block expression after `return` failed to parse.

fn foo() -> i32 {
    // { dg-warning "function is never used" "" { target *-*-* } .-1 }
    return {
        42
    }
}
