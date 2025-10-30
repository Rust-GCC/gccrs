// aux-build:test-macros.rs

#![feature(stmt_expr_attributes)]

#[macro_use]
extern crate test_macros;

// NB. these errors aren't the best errors right now, but they're definitely
// intended to be errors. Somehow using a custom attribute in these positions
// should either require a feature gate or not be allowed on stable.

fn _test6<#[empty_attr] T>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn _test7() {
    match 1 {
        #[empty_attr] // { dg-error "" "" { target *-*-* } }
        0 => {}
        _ => {}
    }
}

fn main() {
}

