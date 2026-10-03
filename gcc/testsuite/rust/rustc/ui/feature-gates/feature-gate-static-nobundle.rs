#[link(name="foo", kind="static-nobundle")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
extern {}

fn main() {}

