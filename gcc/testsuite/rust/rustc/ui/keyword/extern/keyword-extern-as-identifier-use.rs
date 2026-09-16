use extern::foo; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-2 }

fn main() {}

