#![deny(unreachable_code)]
#![allow(dead_code)]

#![feature(never_type)]

fn foo(x: !) -> bool {
    // Explicit matches on the never type are unwarned.
    match x {}
    // But matches in unreachable code are warned.
    match x {} // { dg-error "" "" { target *-*-* } }
}

fn bar() {
    match (return) {
        () => () // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {
    return;
    match () { // { dg-error "" "" { target *-*-* } }
        () => (),
    }
}

