// Test that `fn foo::bar::{self}` only imports `bar` in the type namespace.

mod foo {
    pub fn f() { }
}
use foo::f::{self}; // { dg-error ".E0432." "" { target *-*-* } }

mod bar {
    pub fn baz() {}
    pub mod baz {}
}
use bar::baz::{self};

fn main() {
    baz(); // { dg-error ".E0423." "" { target *-*-* } }
}

