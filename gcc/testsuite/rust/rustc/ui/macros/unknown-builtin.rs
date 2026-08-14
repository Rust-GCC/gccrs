// error-pattern: attempted to define built-in macro more than once

#![feature(rustc_attrs)]

#[rustc_builtin_macro]
macro_rules! unknown { () => () } // { dg-error "" "" { target *-*-* } }

#[rustc_builtin_macro]
macro_rules! line { () => () } // { dg-note "" "" { target *-*-* } }

fn main() {
    line!();
    std::prelude::v1::line!();
}

