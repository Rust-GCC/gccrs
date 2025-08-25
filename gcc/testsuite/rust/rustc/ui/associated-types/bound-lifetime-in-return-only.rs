// revisions: sig local structure ok elision

#![allow(dead_code)]
#![feature(rustc_attrs)]
#![feature(unboxed_closures)]

trait Foo {
    type Item;
}

#[cfg(sig)]
fn sig1(_: for<'a> fn() -> &'a i32) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(sig)]
fn sig2(_: for<'a, 'b> fn(&'b i32) -> &'a i32) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(local)]
fn local1() {
    let _: for<'a> fn() -> &'a i32 = loop { };
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(structure)]
struct Struct1 {
    x: for<'a> fn() -> &'a i32
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(elision)]
fn elision(_: fn() -> &i32) {
// { dg-error "" "" { target *-*-* } .-1 }
}

struct Parameterized<'a> { x: &'a str }

#[cfg(ok)]
fn ok1(_: &dyn for<'a> Fn(&Parameterized<'a>) -> &'a i32) {
}

#[cfg(ok)]
fn ok2(_: &dyn for<'a,'b> Fn<(&'b Parameterized<'a>,), Output=&'a i32>) {
}

#[rustc_error]
fn main() { } // { dg-error "" "" { target *-*-* } }

