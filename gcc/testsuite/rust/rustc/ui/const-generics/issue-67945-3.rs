// revisions: full min

#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

struct Bug<S: ?Sized> {
    A: [(); {
// { dg-error "" "" { target *-*-* } .-1 }
        let x: Option<Box<Self>> = None;
// { dg-error "" "" { target *-*-* } .-1 }
        0
    }],
    B: S
}

fn main() {}

