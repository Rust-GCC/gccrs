// ignore-cloudabi no std::fs support

#![feature(try_trait)]

use std::ops::Try;

fn main() {
    // error for a `Try` type on a non-`Try` fn
    std::fs::File::open("foo")?; // { dg-error ".E0277." "" { target *-*-* } }

    // a non-`Try` type on a non-`Try` fn
    ()?; // { dg-error ".E0277." "" { target *-*-* } }

    // an unrelated use of `Try`
    try_trait_generic::<()>(); // { dg-error ".E0277." "" { target *-*-* } }
}



fn try_trait_generic<T: Try>() -> T {
    // and a non-`Try` object on a `Try` fn.
    ()?; // { dg-error ".E0277." "" { target *-*-* } }

    loop {}
}

