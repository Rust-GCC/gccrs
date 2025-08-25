#![allow(non_camel_case_types)]

use foo::baz;
use bar::baz; // { dg-error ".E0252." "" { target *-*-* } }

mod foo {
    pub struct baz;
}

mod bar {
    pub mod baz {}
}

fn main() {
}

