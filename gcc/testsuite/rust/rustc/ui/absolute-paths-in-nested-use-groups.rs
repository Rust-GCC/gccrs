#![allow(unused_imports)]

mod foo {}

use foo::{
    ::bar,       // { dg-error ".E0433." "" { target *-*-* } }
    super::bar,  // { dg-error ".E0433." "" { target *-*-* } }
    self::bar,   // { dg-error ".E0433." "" { target *-*-* } }
};

fn main() {}

