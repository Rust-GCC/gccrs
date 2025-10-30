// Test that we cannot mutate an outer variable that is not declared
// as `mut` through a closure. Also test that we CAN mutate a moved copy,
// unless this is a `Fn` closure. Issue #16749.

#![feature(unboxed_closures)]

use std::mem;

fn to_fn<A,F:Fn<A>>(f: F) -> F { f }
fn to_fn_mut<A,F:FnMut<A>>(f: F) -> F { f }

fn a() {
    let n = 0;
    let mut f = to_fn_mut(|| {
        n += 1; // { dg-error ".E0594." "" { target *-*-* } }
    });
}

fn b() {
    let mut n = 0;
    let mut f = to_fn_mut(|| {
        n += 1; // OK
    });
}

fn c() {
    let n = 0;
    let mut f = to_fn_mut(move || {
        // If we just did a straight-forward desugaring, this would
        // compile, but we do something a bit more subtle, and hence
        // we get an error.
        n += 1; // { dg-error ".E0594." "" { target *-*-* } }
    });
}

fn d() {
    let mut n = 0;
    let mut f = to_fn_mut(move || {
        n += 1; // OK
    });
}

fn e() {
    let n = 0;
    let mut f = to_fn(move || {
        n += 1; // { dg-error ".E0594." "" { target *-*-* } }
    });
}

fn f() {
    let mut n = 0;
    let mut f = to_fn(move || {
        n += 1; // { dg-error ".E0594." "" { target *-*-* } }
    });
}

fn main() { }

