// compile-flags: -Zsave-analysis

// Check that this doesn't ICE when processing associated const (type).

fn func() {
    trait Trait {
        type MyType;
        const CONST: Self::MyType = bogus.field; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn main() {}

