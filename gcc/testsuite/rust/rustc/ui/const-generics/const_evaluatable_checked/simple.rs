// [full] run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]
#![feature(const_evaluatable_checked)]
#![allow(incomplete_features)]

fn test<const N: usize>() -> [u8; N - 1] where [u8; N - 1]: Default {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    Default::default()
}

fn main() {
    let x = test::<33>();
    assert_eq!(x, [0; 32]);
}

