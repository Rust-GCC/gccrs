#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]

// We do not yet want to support let-bindings in abstract consts,
// so this test should keep failing for now.
fn test<const N: usize>() -> [u8; { let x = N; N + 1 }] where [u8; { let x = N; N + 1 }]: Default {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    Default::default()
}

fn main() {
    let x = test::<31>();
    assert_eq!(x, [0; 32]);
}

