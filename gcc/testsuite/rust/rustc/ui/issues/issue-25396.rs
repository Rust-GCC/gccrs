#![allow(non_camel_case_types)]

use foo::baz;
use bar::baz; // { dg-error ".E0252." "" { target *-*-* } }

use foo::Quux;
use bar::Quux; // { dg-error ".E0252." "" { target *-*-* } }

use foo::blah;
use bar::blah; // { dg-error ".E0252." "" { target *-*-* } }

use foo::WOMP;
use bar::WOMP; // { dg-error ".E0252." "" { target *-*-* } }

fn main() {}

mod foo {
    pub mod baz {}
    pub trait Quux { }
    pub type blah = (f64, u32);
    pub const WOMP: u8 = 5;
}

mod bar {
    pub mod baz {}
    pub type Quux = i32;
    pub struct blah { x: i8 }
    pub const WOMP: i8 = -5;
}

