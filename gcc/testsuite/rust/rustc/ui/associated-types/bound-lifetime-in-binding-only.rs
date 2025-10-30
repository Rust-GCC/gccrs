// revisions: angle paren ok elision

#![allow(dead_code)]
#![feature(rustc_attrs)]
#![feature(unboxed_closures)]

trait Foo {
    type Item;
}

#[cfg(angle)]
fn angle<T: for<'a> Foo<Item=&'a i32>>() {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(angle)]
fn angle1<T>() where T: for<'a> Foo<Item=&'a i32> {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(angle)]
fn angle2<T>() where for<'a> T: Foo<Item=&'a i32> {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(angle)]
fn angle3(_: &dyn for<'a> Foo<Item=&'a i32>) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(paren)]
fn paren<T: for<'a> Fn() -> &'a i32>() {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(paren)]
fn paren1<T>() where T: for<'a> Fn() -> &'a i32 {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(paren)]
fn paren2<T>() where for<'a> T: Fn() -> &'a i32 {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(paren)]
fn paren3(_: &dyn for<'a> Fn() -> &'a i32) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(elision)]
fn elision<T: Fn() -> &i32>() {
// { dg-error "" "" { target *-*-* } .-1 }
}

struct Parameterized<'a> { x: &'a str }

#[cfg(ok)]
fn ok1<T: for<'a> Fn(&Parameterized<'a>) -> &'a i32>() {
}

#[cfg(ok)]
fn ok2<T: for<'a,'b> Fn<(&'b Parameterized<'a>,), Output=&'a i32>>() {
}

#[cfg(ok)]
fn ok3<T>() where for<'a> Parameterized<'a>: Foo<Item=&'a i32> {
}

#[rustc_error]
fn main() { } // { dg-error "" "" { target *-*-* } }

