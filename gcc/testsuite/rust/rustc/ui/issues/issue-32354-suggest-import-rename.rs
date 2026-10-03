// run-rustfix

#![allow(unused_imports)]

pub mod extension1 {
    pub trait ConstructorExtension {}
}

pub mod extension2 {
    pub trait ConstructorExtension {}
}

use extension1::ConstructorExtension;
use extension2::ConstructorExtension; // { dg-error ".E0252." "" { target *-*-* } }

fn main() {}

