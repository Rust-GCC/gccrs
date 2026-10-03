#![feature(generators)]

fn main() {
    |(), ()| {
// { dg-error ".E0628." "" { target *-*-* } .-1 }
        yield;
    };
}

