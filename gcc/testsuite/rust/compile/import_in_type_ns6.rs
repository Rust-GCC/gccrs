#![feature(no_core)]
#![no_core]

mod inner {
    mod intrinsics {
        fn unchecked_shl() {}
        fn unchecked_snl() {}
        fn unchecked_adult_swim() {}
    }
}

use inner::intrinsics::{self, unchecked_snl};

fn foo() /* { dg-warning "never used" } */
{
    use intrinsics::{unchecked_adult_swim, unchecked_shl};
}
