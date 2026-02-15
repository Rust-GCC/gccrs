use std::collections::HashSet;

fn is_subset<T>(this: &HashSet<T>, other: &HashSet<T>) -> bool {
    this.is_subset(other)
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn main() {}

