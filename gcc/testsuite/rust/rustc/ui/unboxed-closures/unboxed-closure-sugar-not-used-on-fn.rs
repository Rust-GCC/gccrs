// Test that the `Fn` traits require `()` form without a feature gate.

fn bar1(x: &dyn Fn<(), Output=()>) {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn bar2<T>(x: &T) where T: Fn<()> {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() { }

