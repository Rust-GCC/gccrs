#![feature(const_trait_bound_opt_out)]
#![feature(const_trait_impl)]
#![allow(incomplete_features)]

struct S;
trait T {}

impl ?const T for S {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

