#![feature(const_trait_bound_opt_out)]
#![allow(incomplete_features)]

trait Super {}
trait T: ?const Super {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

