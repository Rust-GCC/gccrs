// Test that we use fully-qualified type names in error messages.

use std::option::Option;

fn bar(x: usize) -> Option<usize> {
    return x;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

fn main() {
}

