// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[identity_attr]
mod m {
    pub struct X;

    type A = Y; // { dg-error ".E0412." "" { target *-*-* } }
}

struct Y;
type A = X; // { dg-error ".E0412." "" { target *-*-* } }

#[derive(Copy)] // { dg-error ".E0774." "" { target *-*-* } }
mod n {}

#[empty_attr]
mod module; // { dg-error ".E0658." "" { target *-*-* } }

#[empty_attr]
mod outer {
    mod inner; // { dg-error ".E0658." "" { target *-*-* } }

    mod inner_inline {} // OK
}

#[derive(Empty)]
struct S {
    field: [u8; {
        #[path = "outer/inner.rs"]
        mod inner; // { dg-error ".E0658." "" { target *-*-* } }
        mod inner_inline {} // OK
        0
    }]
}

#[identity_attr]
fn f() {
    #[path = "outer/inner.rs"]
    mod inner; // { dg-error ".E0658." "" { target *-*-* } }
    mod inner_inline {} // OK
}

fn main() {}

