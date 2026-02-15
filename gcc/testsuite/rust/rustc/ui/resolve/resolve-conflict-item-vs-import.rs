use std::mem::transmute;

fn transmute() {}
// { dg-error ".E0255." "" { target *-*-* } .-1 }
// { dg-error ".E0255." "" { target *-*-* } .-2 }
// { dg-error ".E0255." "" { target *-*-* } .-3 }
fn main() {
}

