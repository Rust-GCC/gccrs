// aux-build:resolved-located-at.rs

#[macro_use]
extern crate resolved_located_at;

fn main() {
    resolve_located_at!(a b)
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

