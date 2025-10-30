// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]
#![feature(const_evaluatable_checked)]
#![allow(incomplete_features)]

type Arr<const N: usize> = [u8; N - 1]; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

fn test<const N: usize>() -> Arr<N> where Arr<N>: Sized {
    todo!()
}

fn main() {
    test::<0>();
}

