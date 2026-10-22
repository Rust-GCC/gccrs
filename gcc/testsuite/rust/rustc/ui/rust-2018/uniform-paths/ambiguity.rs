// edition:2018

#![allow(non_camel_case_types)]

use std::io;
// { dg-error ".E0659." "" { target *-*-* } .-1 }

mod std {
    pub struct io;
}

fn main() {}

