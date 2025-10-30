// aux-build:issue-61963.rs
// aux-build:issue-61963-1.rs
#![deny(bare_trait_objects)]

#[macro_use]
extern crate issue_61963;
#[macro_use]
extern crate issue_61963_1;

// This test checks that the bare trait object lint does not trigger on macro attributes that
// generate code which would trigger the lint.

pub struct Baz;
pub trait Bar { }
pub struct Qux<T>(T);

#[dom_struct]
pub struct Foo {
// { dg-error "" "" { target *-*-* } .-1 }
    qux: Qux<Qux<Baz>>,
    bar: Box<Bar>,
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

