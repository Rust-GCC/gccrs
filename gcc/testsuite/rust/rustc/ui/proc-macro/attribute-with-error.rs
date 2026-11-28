// aux-build:test-macros.rs

#![feature(custom_inner_attributes)]

#[macro_use]
extern crate test_macros;

#[recollect_attr]
fn test1() {
    let a: i32 = "foo";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let b: i32 = "f'oo";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn test2() {
    #![recollect_attr]

    // FIXME: should have a type error here and assert it works but it doesn't
}

trait A {
    // FIXME: should have a #[recollect_attr] attribute here and assert that it works
    fn foo(&self) {
        let a: i32 = "foo";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }
}

struct B;

impl A for B {
    #[recollect_attr]
    fn foo(&self) {
        let a: i32 = "foo";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }
}

#[recollect_attr]
fn main() {
}

