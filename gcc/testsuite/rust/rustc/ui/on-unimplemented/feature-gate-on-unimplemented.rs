// Test that `#[rustc_on_unimplemented]` is gated by `rustc_attrs` feature gate.

#[rustc_on_unimplemented = "test error `{Self}` with `{Bar}`"]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
trait Foo<Bar>
{}

fn main() {}

