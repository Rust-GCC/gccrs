#[link(name = "foo", cfg(foo))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
extern {}

fn main() {}

