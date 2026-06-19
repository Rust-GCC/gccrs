// only-x86_64

#[target_feature(enable = "sse2")] // { dg-error ".E0658." "" { target *-*-* } }
fn foo() {}

fn main() {}

