mod foo {
    pub struct A;

    pub mod bar {
        pub struct B;
    }
}

use foo::{A, bar::B    as    A};
// { dg-error ".E0252." "" { target *-*-* } .-1 }

fn main() {}

