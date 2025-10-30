#![feature(unboxed_closures)]
#![feature(rustc_attrs)]
// Test for projection cache. We should be able to project distinct
// lifetimes from `foo` as we reinstantiate it multiple times, but not
// if we do it just once. In this variant, the region `'a` is used in
// an invariant position, which affects the results.

// revisions: ok oneuse transmute krisskross
#![allow(dead_code, unused_variables)]

use std::marker::PhantomData;

struct Type<'a> {
    // Invariant
    data: PhantomData<fn(&'a u32) -> &'a u32>,
}

fn foo<'a>() -> Type<'a> {
    loop {}
}

fn bar<T>(t: T, x: T::Output) -> T::Output
where
    T: FnOnce<()>,
{
    t()
}

#[cfg(ok)] // two instantiations: OK
fn baz<'a, 'b>(x: Type<'a>, y: Type<'b>) -> (Type<'a>, Type<'b>) {
    let a = bar(foo, x);
    let b = bar(foo, y);
    (a, b)
}

#[cfg(oneuse)] // one instantiation: BAD
fn baz<'a, 'b>(x: Type<'a>, y: Type<'b>) -> (Type<'a>, Type<'b>) {
    let f = foo; // <-- No consistent type can be inferred for `f` here.
    let a = bar(f, x);
    let b = bar(f, y); // { dg-error "" "" { target *-*-* } }
    (a, b)
}

#[cfg(transmute)] // one instantiations: BAD
fn baz<'a, 'b>(x: Type<'a>) -> Type<'static> {
    // Cannot instantiate `foo` with any lifetime other than `'a`,
    // since it is provided as input.

    bar(foo, x) // { dg-error "" "" { target *-*-* } }
}

#[cfg(krisskross)] // two instantiations, mixing and matching: BAD
fn transmute<'a, 'b>(x: Type<'a>, y: Type<'b>) -> (Type<'a>, Type<'b>) {
    let a = bar(foo, y); // { dg-error "" "" { target *-*-* } }
    let b = bar(foo, x);
    (a, b) // { dg-error "" "" { target *-*-* } }
}

#[rustc_error]
fn main() {}
// { dg-error "" "" { target *-*-* } .-1 }

