#![feature(generator_trait)]
#![feature(generators)]

// Test that we cannot create a generator that returns a value of its
// own type.

use std::ops::Generator;

pub fn want_cyclic_generator_return<T>(_: T)
    where T: Generator<Yield = (), Return = T>
{
}

fn supply_cyclic_generator_return() {
    want_cyclic_generator_return(|| {
// { dg-error ".E0271." "" { target *-*-* } .-1 }
        if false { yield None.unwrap(); }
        None.unwrap()
    })
}

pub fn want_cyclic_generator_yield<T>(_: T)
    where T: Generator<Yield = T, Return = ()>
{
}

fn supply_cyclic_generator_yield() {
    want_cyclic_generator_yield(|| {
// { dg-error ".E0271." "" { target *-*-* } .-1 }
        if false { yield None.unwrap(); }
        None.unwrap()
    })
}

fn main() { }

