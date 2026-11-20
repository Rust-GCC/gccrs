// Test that the use of the box syntax is gated by `box_syntax` feature gate.

fn main() {
    let x = box 3;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

