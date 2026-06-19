#![allow(incomplete_features)]
#![feature(const_trait_impl)]
#![feature(const_fn)]

pub trait Plus {
    fn plus(self, rhs: Self) -> Self;
}

impl const Plus for i32 {
    fn plus(self, rhs: Self) -> Self {
        self + rhs
    }
}

impl Plus for u32 {
    fn plus(self, rhs: Self) -> Self {
        self + rhs
    }
}

pub const fn add_i32(a: i32, b: i32) -> i32 {
    a.plus(b) // ok
}

pub const fn add_u32(a: u32, b: u32) -> u32 {
    a.plus(b)
// { dg-error ".E0015." "" { target *-*-* } .-1 }
}

fn main() {}

