// Original Levenshtein distance for both of this is 1. We improved accuracy with
// additional case insensitive comparison.

struct TyUint {}

struct TyInt {}

fn main() {
    TyUInt {};
// { dg-error ".E0422." "" { target *-*-* } .-1 }
}

