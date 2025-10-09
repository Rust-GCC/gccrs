#![allow(dead_code)]

#[repr(i128)] // { dg-error ".E0517." "" { target *-*-* } }
struct Foo;

#[repr(u128)] // { dg-error ".E0517." "" { target *-*-* } }
struct Bar;

fn main() {}

