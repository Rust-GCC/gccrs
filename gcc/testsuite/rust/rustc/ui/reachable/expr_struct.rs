#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]
#![feature(type_ascription)]

struct Foo {
    a: usize,
    b: usize,
}

fn a() {
    // struct expr is unreachable:
    let x = Foo { a: 22, b: 33, ..return }; // { dg-error "" "" { target *-*-* } }
}

fn b() {
    // the `33` is unreachable:
    let x = Foo { a: return, b: 33, ..return }; // { dg-error "" "" { target *-*-* } }
}

fn c() {
    // the `..return` is unreachable:
    let x = Foo { a: 22, b: return, ..return }; // { dg-error "" "" { target *-*-* } }
}

fn d() {
    // the struct expr is unreachable:
    let x = Foo { a: 22, b: return }; // { dg-error "" "" { target *-*-* } }
}

fn main() { }

