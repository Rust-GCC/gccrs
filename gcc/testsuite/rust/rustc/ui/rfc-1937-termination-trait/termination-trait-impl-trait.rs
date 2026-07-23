// Tests that an `impl Trait` that is not `impl Termination` will not work.
fn main() -> impl Copy { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

