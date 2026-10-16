#![feature(generators)]

fn main() {
    let _ = || {
        *(1 as *mut u32) = 42;
// { dg-error ".E0133." "" { target *-*-* } .-1 }
        yield;
    };
}

