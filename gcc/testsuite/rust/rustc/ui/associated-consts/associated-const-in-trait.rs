// #29924

#![feature(const_fn, associated_consts)]

trait Trait {
    const N: usize;
}

impl dyn Trait {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    const fn n() -> usize { Self::N }
}

fn main() {}

