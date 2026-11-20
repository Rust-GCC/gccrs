#![feature(no_core)]
#![no_core]

// { dg-additional-options "-frust-compat-version=1.82" }

fn main() {
    let a = 15;
    let _ = &raw const a;
    let _ = &raw mut a;
}
