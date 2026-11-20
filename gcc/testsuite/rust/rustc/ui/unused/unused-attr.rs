#![deny(unused_attributes)]
#![feature(rustc_attrs)]

#![rustc_dummy] // { dg-error "" "" { target *-*-* } }

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
extern crate core;

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
use std::collections;

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
extern "C" {
    #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
    fn foo();
}

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
mod foo {
    #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
    pub enum Foo {
        #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
        Bar,
    }
}

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
fn bar(f: foo::Foo) {
    match f {
        #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
        foo::Foo::Bar => {}
    }
}

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
struct Foo {
    #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
    a: isize
}

#[rustc_dummy] // { dg-error "" "" { target *-*-* } }
trait Baz {
    #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
    fn blah(&self);
    #[rustc_dummy] // { dg-error "" "" { target *-*-* } }
    fn blah2(&self) {}
}

fn main() {}

