use m::f as x; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

mod m {}

fn main() {}

