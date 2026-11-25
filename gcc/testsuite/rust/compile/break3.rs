#![feature(no_core)]
#![no_core]

// Rust-GCC/gccrs#5108: a block expression after `break` failed to parse.

fn foo() -> i32 {
    // { dg-warning "function is never used" "" { target *-*-* } .-1 }
    let mut i = 0;
    loop {
        break {
            i = 42;
        }
    }
    i
}
