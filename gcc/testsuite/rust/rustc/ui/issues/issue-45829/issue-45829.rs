mod foo {
    pub struct A;
    pub struct B;
}

use foo::{A, B as A};
// { dg-error ".E0252." "" { target *-*-* } .-1 }

fn main() {}

