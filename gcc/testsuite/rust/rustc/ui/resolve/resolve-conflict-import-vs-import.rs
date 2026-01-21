// run-rustfix

#[allow(unused_imports)]
use std::mem::transmute;
use std::mem::transmute;
// { dg-error ".E0252." "" { target *-*-* } .-1 }

fn main() {
}

