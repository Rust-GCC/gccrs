#![feature(trivial_bounds)]
#![allow(unused)]
#![deny(trivial_bounds)]

struct A where i32: Copy; // { dg-error "" "" { target *-*-* } }

trait X<T: Copy> {}

trait Y<T>: Copy {}

trait Z {
    type S: Copy;
}

// Check only the bound the user writes trigger the lint
fn trivial_elaboration<T>() where T: X<i32> + Z<S = i32>, i32: Y<T> {} // OK

fn global_param() where i32: X<()> {} // { dg-error "" "" { target *-*-* } }

// Should only error on the trait bound, not the implicit
// projection bound <i32 as Z>::S == i32.
fn global_projection() where i32: Z<S = i32> {} // { dg-error "" "" { target *-*-* } }

impl A {
    fn new() -> A { A }
}

// Lifetime bounds should be linted as well
fn global_lifetimes() where i32: 'static, &'static str: 'static {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn local_lifetimes<'a>() where i32: 'a, &'a str: 'a {} // OK

fn global_outlives() where 'static: 'static {} // { dg-error "" "" { target *-*-* } }

// Check that each bound is checked individually
fn mixed_bounds<T: Copy>() where i32: X<T> + Copy {} // { dg-error "" "" { target *-*-* } }

fn main() {}

