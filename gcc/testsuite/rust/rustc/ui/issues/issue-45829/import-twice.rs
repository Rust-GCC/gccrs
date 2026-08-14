mod foo {
    pub struct A;
    pub struct B;
}

use foo::{A, A};
// { dg-error ".E0252." "" { target *-*-* } .-1 }

fn main() {}

