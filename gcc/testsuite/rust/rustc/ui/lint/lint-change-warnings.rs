#![deny(warnings)]
#![allow(dead_code)]

fn main() {
    while true {} // { dg-error "" "" { target *-*-* } }
}

#[allow(warnings)]
fn foo() {
    while true {}
}

#[warn(warnings)]
fn bar() {
    while true {} // { dg-warning "" "" { target *-*-* } }
}

#[forbid(warnings)]
fn baz() {
    while true {} // { dg-error "" "" { target *-*-* } }
}

