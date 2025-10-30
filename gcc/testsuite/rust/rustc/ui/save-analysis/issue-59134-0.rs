// compile-flags: -Zsave-analysis

// Check that this doesn't ICE when processing associated const (field expr).

pub fn f() {
    trait Trait {}
    impl dyn Trait {
        const FLAG: u32 = bogus.field; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn main() {}

