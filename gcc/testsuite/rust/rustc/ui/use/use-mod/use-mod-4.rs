use foo::self; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

use std::mem::self;
// { dg-error ".E0429." "" { target *-*-* } .-1 }

fn main() {}

