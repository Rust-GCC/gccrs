#[rustc_diagnostic_item = "foomp"] // { dg-error ".E0658." "" { target *-*-* } }
struct Foomp;
fn main() {}

