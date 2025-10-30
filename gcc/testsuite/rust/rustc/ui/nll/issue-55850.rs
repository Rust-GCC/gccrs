#![allow(unused_mut)]
#![feature(generators, generator_trait)]

use std::marker::Unpin;
use std::ops::Generator;
use std::ops::GeneratorState::Yielded;
use std::pin::Pin;

pub struct GenIter<G>(G);

impl <G> Iterator for GenIter<G>
where
    G: Generator + Unpin,
{
    type Item = G::Yield;

    fn next(&mut self) -> Option<Self::Item> {
        match Pin::new(&mut self.0).resume(()) {
            Yielded(y) => Some(y),
            _ => None
        }
    }
}

fn bug<'a>() -> impl Iterator<Item = &'a str> {
    GenIter(move || {
        let mut s = String::new();
        yield &s[..] // { dg-error ".E0626." "" { target *-*-* } }
// { dg-error ".E0626." "" { target *-*-* } .-2 }
    })
}

fn main() {
    bug();
}

