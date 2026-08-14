// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

type Arr<const N: usize> = [u8; N - 1];
// { dg-error "" "" { target *-*-* } .-1 }

fn test<const N: usize>() -> Arr<N> where Arr<N>: Default {
// { dg-error "" "" { target *-*-* } .-1 }
    Default::default()
}

fn main() {
    let x = test::<33>();
    assert_eq!(x, [0; 32]);
}

