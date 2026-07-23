// revisions: func object clause

#![allow(dead_code)]
#![feature(rustc_attrs)]

trait Foo<'a> {
    type Item;
}

impl<'a> Foo<'a> for() {
    type Item = ();
}

// Check that appearing in a projection input in the argument is not enough:
#[cfg(func)]
fn func1(_: for<'a> fn(<() as Foo<'a>>::Item) -> &'a i32) {
// { dg-error "" "" { target *-*-* } .-1 }
}

// Check that appearing in a projection input in the return still
// causes an error:
#[cfg(func)]
fn func2(_: for<'a> fn() -> <() as Foo<'a>>::Item) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(object)]
fn object1(_: Box<dyn for<'a> Fn(<() as Foo<'a>>::Item) -> &'a i32>) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(object)]
fn object2(_: Box<dyn for<'a> Fn() -> <() as Foo<'a>>::Item>) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(clause)]
fn clause1<T>() where T: for<'a> Fn(<() as Foo<'a>>::Item) -> &'a i32 {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(clause)]
fn clause2<T>() where T: for<'a> Fn() -> <() as Foo<'a>>::Item {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[rustc_error]
fn main() { } // { dg-error "" "" { target *-*-* } }

