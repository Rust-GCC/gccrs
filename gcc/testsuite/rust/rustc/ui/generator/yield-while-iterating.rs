#![feature(generators, generator_trait)]

use std::ops::{GeneratorState, Generator};
use std::cell::Cell;
use std::pin::Pin;

fn yield_during_iter_owned_data(x: Vec<i32>) {
    // The generator owns `x`, so we error out when yielding with a
    // reference to it.  This winds up becoming a rather confusing
    // regionck error -- in particular, we would freeze with the
    // reference in scope, and it doesn't live long enough.
    let _b = move || {
        for p in &x { // { dg-error ".E0626." "" { target *-*-* } }
            yield();
        }
    };
}

fn yield_during_iter_borrowed_slice(x: &[i32]) {
    let _b = move || {
        for p in x {
            yield();
        }
    };
}

fn yield_during_iter_borrowed_slice_2() {
    let mut x = vec![22_i32];
    let _b = || {
        for p in &x {
            yield();
        }
    };
    println!("{:?}", x);
}

fn yield_during_iter_borrowed_slice_3() {
    // OK to take a mutable ref to `x` and yield
    // up pointers from it:
    let mut x = vec![22_i32];
    let mut b = || {
        for p in &mut x {
            yield p;
        }
    };
    Pin::new(&mut b).resume(());
}

fn yield_during_iter_borrowed_slice_4() {
    // ...but not OK to do that while reading
    // from `x` too
    let mut x = vec![22_i32];
    let mut b = || {
        for p in &mut x {
            yield p;
        }
    };
    println!("{}", x[0]); // { dg-error ".E0502." "" { target *-*-* } }
    Pin::new(&mut b).resume(());
}

fn yield_during_range_iter() {
    // Should be OK.
    let mut b = || {
        let v = vec![1,2,3];
        let len = v.len();
        for i in 0..len {
            let x = v[i];
            yield x;
        }
    };
    Pin::new(&mut b).resume(());
}

fn main() { }

