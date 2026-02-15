#![feature(unsize)]

use std::marker::Unsize;

pub struct MyType;

impl<T> Unsize<T> for MyType {}
// { dg-error ".E0328." "" { target *-*-* } .-1 }

fn main() {}

