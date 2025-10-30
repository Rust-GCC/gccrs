#![feature(generators, generator_trait)]
use std::ops::{Generator, GeneratorState};
use std::pin::Pin;

fn dangle(x: &mut i32) -> &'static mut i32 {
    let mut g = || {
        yield;
        x
    };
    loop {
        match Pin::new(&mut g).resume(()) {
            GeneratorState::Complete(c) => return c,
// { dg-error ".E0621." "" { target *-*-* } .-1 }
            GeneratorState::Yielded(_) => (),
        }
    }
}

fn main() {}

