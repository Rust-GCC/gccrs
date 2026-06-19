mod rustfmt {}

#[rustfmt::skip] // { dg-error ".E0433." "" { target *-*-* } }
fn main() {}

