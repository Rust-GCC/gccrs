// `#[macro_export] macro_rules` that doesn't originate from macro expansions can be placed
// into the root module soon enough to act as usual items and shadow globs and preludes.

#![feature(decl_macro)]

// `macro_export` shadows globs
use inner1::*;

mod inner1 {
    pub macro exported() {}
}

exported!();

mod deep {
    fn deep() {
        type Deeper = [u8; {
            #[macro_export]
            macro_rules! exported {
                () => ( struct Б; ) // { dg-error ".E0658." "" { target *-*-* } }
            }

            0
        }];
    }
}

// `macro_export` shadows std prelude
fn main() {
    panic!();
}

mod inner3 {
    #[macro_export]
    macro_rules! panic {
        () => ( struct Г; ) // { dg-error ".E0658." "" { target *-*-* } }
    }
}

// `macro_export` shadows builtin macros
include!();

mod inner4 {
    #[macro_export]
    macro_rules! include {
        () => ( struct Д; ) // { dg-error ".E0658." "" { target *-*-* } }
    }
}

