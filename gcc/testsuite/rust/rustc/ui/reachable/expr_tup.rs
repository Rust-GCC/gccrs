#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]
#![feature(type_ascription)]

fn a() {
    // the `2` is unreachable:
    let x: (usize, usize) = (return, 2); // { dg-error "" "" { target *-*-* } }
}

fn b() {
    // the tuple is unreachable:
    let x: (usize, usize) = (2, return); // { dg-error "" "" { target *-*-* } }
}

fn main() { }

