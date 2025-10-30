// Test that changing what a `type` points to does not go unnoticed
// by the variance analysis.

// compile-flags: -Z query-dep-graph

#![feature(rustc_attrs)]
#![allow(dead_code)]
#![allow(unused_variables)]
#![rustc_if_this_changed(hir_crate)]
fn main() {}

struct Foo<T> {
    f: T,
}

type TypeAlias<T> = Foo<T>;

#[rustc_then_this_would_need(variances_of)] // { dg-error "" "" { target *-*-* } }
struct Use<T> {
    x: TypeAlias<T>,
}

