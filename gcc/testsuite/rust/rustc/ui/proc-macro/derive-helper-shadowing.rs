// edition:2018
// aux-build:test-macros.rs
// aux-build:derive-helper-shadowing.rs

#[macro_use]
extern crate test_macros;
#[macro_use]
extern crate derive_helper_shadowing;

use test_macros::empty_attr as empty_helper;

macro_rules! gen_helper_use {
    () => {
        #[empty_helper] // { dg-error "" "" { target *-*-* } }
        struct W;
    }
}

#[empty_helper] // { dg-error ".E0659." "" { target *-*-* } }
#[derive(Empty)]
struct S {
    #[empty_helper] // OK, no ambiguity, derive helpers have highest priority
    field: [u8; {
        use empty_helper; // { dg-error ".E0659." "" { target *-*-* } }

        #[empty_helper] // OK, no ambiguity, derive helpers have highest priority
        struct U;

        mod inner {
            // OK, no ambiguity, the non-helper attribute is not in scope here, only the helper.
            #[empty_helper]
            struct V;

            gen_helper_use!();

            #[derive(GenHelperUse)] // { dg-error "" "" { target *-*-* } }
            struct Owo;

            use empty_helper as renamed;
            #[renamed] // { dg-error "" "" { target *-*-* } }
            struct Wow;
        }

        0
    }]
}

// OK, no ambiguity, only the non-helper attribute is in scope.
#[empty_helper]
struct Z;

fn main() {
    let s = S { field: [] };
}

