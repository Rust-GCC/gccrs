#![feature(never_type)]
#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn foo(x: !, y: usize) { }

fn bar(x: !) { }

fn a() {
    // the `22` is unreachable:
    foo(return, 22); // { dg-error "" "" { target *-*-* } }
}

fn b() {
    // the call is unreachable:
    bar(return); // { dg-error "" "" { target *-*-* } }
}

fn main() { }

