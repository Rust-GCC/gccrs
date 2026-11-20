#[cfg(foo(bar))] // { dg-error ".E0537." "" { target *-*-* } }
fn check() {}

fn main() {}

