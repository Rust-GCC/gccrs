fn f(_: impl Iterator<Item = &'_ ()>) {} // { dg-error ".E0106." "" { target *-*-* } }
fn main() {}

