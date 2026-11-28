#[link(name="foo", kind="raw-dylib")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
extern {}

fn main() {}

