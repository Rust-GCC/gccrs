// run-rustfix

use std::ops::Add;

struct A<B>(B);

impl<B> Add for A<B> where B: Add {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        A(self.0 + rhs.0) // { dg-error ".E0308." "" { target *-*-* } }
    }
}

struct C<B>(B);

impl<B: Add> Add for C<B> {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Self(self.0 + rhs.0) // { dg-error ".E0308." "" { target *-*-* } }
    }
}

struct D<B>(B);

impl<B> Add for D<B> {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Self(self.0 + rhs.0) // { dg-error ".E0369." "" { target *-*-* } }
    }
}

struct E<B>(B);

impl<B: Add> Add for E<B> where B: Add<Output = B> {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Self(self.0 + rhs.0)
    }
}

fn main() {}

