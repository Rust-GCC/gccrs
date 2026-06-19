#![allow(warnings)]
#![deny(unreachable_code)]

enum Void { }

fn foo(v: Void) {
    match v { }
    let x = 2; // { dg-error "" "" { target *-*-* } }
}

fn main() {
}

