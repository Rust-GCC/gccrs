#![allow(incomplete_features)]
#![feature(const_trait_impl)]

pub struct Int(i32);

impl const std::ops::Add for i32 {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        self + rhs
    }
}

impl std::ops::Add for Int {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Int(self.0 + rhs.0)
    }
}

impl const std::ops::Add for Int {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Int(self.0 + rhs.0)
    }
}

fn main() {}

