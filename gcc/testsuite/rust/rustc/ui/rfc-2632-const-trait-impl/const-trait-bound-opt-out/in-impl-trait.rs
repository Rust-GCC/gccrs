#![feature(const_trait_bound_opt_out)]
#![feature(associated_type_bounds)]
#![allow(incomplete_features)]

trait T {}
struct S;
impl T for S {}

fn rpit() -> impl ?const T { S }
// { dg-error "" "" { target *-*-* } .-1 }

fn apit(_: impl ?const T) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn rpit_assoc_bound() -> impl IntoIterator<Item: ?const T> { Some(S) }
// { dg-error "" "" { target *-*-* } .-1 }

fn apit_assoc_bound(_: impl IntoIterator<Item: ?const T>) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

