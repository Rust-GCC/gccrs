// aux-build:complex_impl_support.rs

extern crate complex_impl_support;

use complex_impl_support::{External, M};

struct Q;

impl<R> External for (Q, R) {} // { dg-error ".E0117." "" { target *-*-* } }
// { dg-error ".E0117." "" { target *-*-* } .-1 }

fn main() {}

