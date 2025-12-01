#![allow(dead_code)]
#![feature(negative_impls)]

// Overlapping negative impls for `MyStruct` are not permitted:
struct MyStruct;
impl !Send for MyStruct {}
impl !Send for MyStruct {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

