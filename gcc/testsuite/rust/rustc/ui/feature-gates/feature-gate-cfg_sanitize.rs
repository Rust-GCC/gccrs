#[cfg(not(sanitize = "thread"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn main() {}

