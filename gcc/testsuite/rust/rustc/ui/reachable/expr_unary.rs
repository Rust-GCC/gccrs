#![feature(never_type)]
#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn foo() {
    let x: ! = ! { return; }; // { dg-error ".E0600." "" { target *-*-* } }
// { dg-error ".E0600." "" { target *-*-* } .-2 }
}

fn main() { }

