// Regression test for issue #68542
// Tests that we don't ICE when a closure appears
// in the length part of an array.

struct Bug {
    a: [(); (|| { 0 })()] // { dg-error ".E0080." "" { target *-*-* } }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

fn main() {}

