// Verify that floating-point range patterns compile without errors.
#![feature(no_core)]
#![no_core]

fn foo(x: f64) -> i32 {
    match x {
        -1.0..=0.0 => -1,
        0.0..=1.0 => 0,
        _ => 42,
    }
}

fn main() {
    foo(0.5);
}
