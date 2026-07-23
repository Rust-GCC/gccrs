// gate-test-const_fn_floating_point_arithmetic

// revisions: stock gated

#![feature(rustc_attrs)]
#![cfg_attr(gated, feature(const_fn_floating_point_arithmetic))]

const fn add(f: f32) -> f32 { f + 2.0 }
// { dg-error "" "" { target *-*-* } .-1 }
const fn sub(f: f32) -> f32 { 2.0 - f }
// { dg-error "" "" { target *-*-* } .-1 }
const fn mul(f: f32, g: f32) -> f32 { f * g }
// { dg-error "" "" { target *-*-* } .-1 }
const fn div(f: f32, g: f32) -> f32 { f / g }
// { dg-error "" "" { target *-*-* } .-1 }
const fn neg(f: f32) -> f32 { -f }
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_error]
fn main() {} // { dg-error "" "" { target *-*-* } }

