// aux-build:impl-const.rs
// run-pass
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

extern crate impl_const;

use impl_const::*;

pub fn main() {
    let n = Num::<5>;
    n.five();
}

