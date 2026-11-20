// Issue #14061: tests the interaction between generic implementation
// parameter bounds and trait objects.

#![feature(box_syntax)]

use std::marker;

struct S<T>(marker::PhantomData<T>);

trait Gettable<T> {
    fn get(&self) -> T { panic!() }
}

impl<T: Send + Copy + 'static> Gettable<T> for S<T> {}

fn f<T>(val: T) {
    let t: S<T> = S(marker::PhantomData);
    let a = &t as &dyn Gettable<T>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

fn g<T>(val: T) {
    let t: S<T> = S(marker::PhantomData);
    let a: &dyn Gettable<T> = &t;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

fn foo<'a>() {
    let t: S<&'a isize> = S(marker::PhantomData);
    let a = &t as &dyn Gettable<&'a isize>;
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

fn foo2<'a>() {
    let t: Box<S<String>> = box S(marker::PhantomData);
    let a = t as Box<dyn Gettable<String>>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn foo3<'a>() {
    struct Foo; // does not impl Copy

    let t: Box<S<Foo>> = box S(marker::PhantomData);
    let a: Box<dyn Gettable<Foo>> = t;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

