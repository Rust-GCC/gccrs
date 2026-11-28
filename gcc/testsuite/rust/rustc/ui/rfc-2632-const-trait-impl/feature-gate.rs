// revisions: stock gated
// gate-test-const_trait_impl

#![cfg_attr(gated, feature(const_trait_impl))]
#![allow(incomplete_features)]
#![feature(rustc_attrs)]

struct S;
trait T {}
impl const T for S {}
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_error]
fn main() {} // { dg-error "" "" { target *-*-* } }

