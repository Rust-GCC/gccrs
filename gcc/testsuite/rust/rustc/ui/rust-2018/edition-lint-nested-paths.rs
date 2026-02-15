// run-rustfix

#![feature(rust_2018_preview, crate_visibility_modifier)]
#![deny(absolute_paths_not_starting_with_crate)]

use foo::{a, b};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

mod foo {
    crate fn a() {}
    crate fn b() {}
    crate fn c() {}
}

fn main() {
    a();
    b();

    {
        use foo::{self as x, c};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        x::a();
        c();
    }
}

