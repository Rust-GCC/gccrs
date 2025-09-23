use foo::bar::{
    self,
// { dg-error ".E0430." "" { target *-*-* } .-1 }
    Bar,
    self
// { dg-error ".E0252." "" { target *-*-* } .-1 }
};

use {self};
// { dg-error ".E0431." "" { target *-*-* } .-1 }

mod foo {
    pub mod bar {
        pub struct Bar;
        pub struct Baz;
    }
}

fn main() {}

