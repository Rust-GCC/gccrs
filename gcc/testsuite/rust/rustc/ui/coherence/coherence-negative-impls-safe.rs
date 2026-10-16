#![feature(negative_impls)]

use std::marker::Send;

struct TestType;

unsafe impl !Send for TestType {}
// { dg-error ".E0198." "" { target *-*-* } .-1 }

fn main() {}

