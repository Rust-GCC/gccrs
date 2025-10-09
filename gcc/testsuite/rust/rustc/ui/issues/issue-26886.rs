use std::sync::{self, Arc};
use std::sync::Arc; // { dg-error ".E0252." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
use std::sync; // { dg-error ".E0252." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
}

