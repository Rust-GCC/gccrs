// Test that `#[rustc_*]` attributes are gated by `rustc_attrs` feature gate.

#![feature(decl_macro)]

mod rustc { pub macro unknown() {} }
mod unknown { pub macro rustc() {} }

#[rustc::unknown]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
fn f() {}

#[unknown::rustc]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
fn g() {}

#[rustc_dummy]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
#[rustc_unknown]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {}

