fn foo<'static>(x: &'static str) { } // { dg-error ".E0262." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

