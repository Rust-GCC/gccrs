#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]

struct Bool<const B: bool>;

trait True {}

impl True for Bool<true> {}

fn test<T, const P: usize>() where Bool<{core::mem::size_of::<T>() > 4}>: True {
    todo!()
}

fn main() {
    test::<2>();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

