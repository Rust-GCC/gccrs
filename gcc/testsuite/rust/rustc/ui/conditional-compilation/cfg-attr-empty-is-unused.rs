// Check that `#[cfg_attr($PREDICATE,)]` triggers the `unused_attribute` lint.

// compile-flags: --cfg TRUE

#![deny(unused)]

#[cfg_attr(FALSE,)] // { dg-error "" "" { target *-*-* } }
fn _f() {}

#[cfg_attr(TRUE,)] // { dg-error "" "" { target *-*-* } }
fn _g() {}

fn main() {}

