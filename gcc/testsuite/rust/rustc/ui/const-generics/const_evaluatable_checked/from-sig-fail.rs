#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]

fn test<const N: usize>() -> [u8; N - 1] {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
    todo!()
}

fn main() {
    test::<0>();
}

