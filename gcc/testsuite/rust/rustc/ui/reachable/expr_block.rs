#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn a() {
    // Here the tail expression is considered unreachable:
    let x = {
        return;
        22 // { dg-error "" "" { target *-*-* } }
    };
}

fn b() {
    // Here the `x` assignment is considered unreachable, not the block:
    let x = {
        return;
    };
}

fn c() {
    // Here the `println!` is unreachable:
    let x = {
        return;
        println!("foo");
// { dg-error "" "" { target *-*-* } .-1 }
        22
    };
}

fn main() { }

