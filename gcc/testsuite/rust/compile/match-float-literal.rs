// Verify that floating-point literal patterns compile without errors.
// Previously gccrs emitted "sorry, unimplemented: floating-point literal
// in pattern". This was removed per rust-lang/rust#116284.
#![feature(no_core)]
#![no_core]

fn foo(x: f64) -> i32 {
    match x {
        1.0 => 1,
        2.0 => 2,
        -1.0 => -1,
        0.0 => 0,
        _ => 42,
    }
}

fn bar(x: f32) -> i32 {
    match x {
        1.0 => 1,
        -0.0 => 0, // -0.0 matches 0.0 under == semantics
        _ => 42,
    }
}

fn main() {
    foo(1.0);
    bar(1.0);
}
