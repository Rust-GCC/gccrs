// revisions: full min

#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

use std::mem::MaybeUninit;

struct Bug<S> {
// { dg-error "" "" { target *-*-* } .-1 }
    A: [(); {
        let x: S = MaybeUninit::uninit();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        let b = &*(&x as *const _ as *const S);
// { dg-error "" "" { target *-*-* } .-1 }
        0
    }],
}

fn main() {}

