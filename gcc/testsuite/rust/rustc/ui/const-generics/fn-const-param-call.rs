// Check that functions cannot be used as const parameters.
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

fn function() -> u32 {
    17
}

struct Wrapper<const F: fn() -> u32>; // { dg-error "" "" { target *-*-* } }

impl<const F: fn() -> u32> Wrapper<F> {
// { dg-error "" "" { target *-*-* } .-1 }
    fn call() -> u32 {
        F()
    }
}

fn main() {
    assert_eq!(Wrapper::<function>::call(), 17);
}

