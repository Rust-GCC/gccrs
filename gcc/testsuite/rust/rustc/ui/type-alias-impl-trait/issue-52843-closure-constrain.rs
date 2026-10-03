// Checks to ensure that we properly detect when a closure constrains an opaque type

#![feature(type_alias_impl_trait)]

use std::fmt::Debug;

fn main() {
    type Opaque = impl Debug;
    fn _unused() -> Opaque { String::new() }
    let null = || -> Opaque { 0 };
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{:?}", null());
}

