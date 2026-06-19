// Check that generators respect the muatability of their upvars.

#![feature(generators, nll)]

fn mutate_upvar() {
    let x = 0;
    move || {
        x = 1;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
        yield;
    };
}

fn main() {}

