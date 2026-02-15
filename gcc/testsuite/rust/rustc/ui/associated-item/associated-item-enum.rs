enum Enum { Variant }

impl Enum {
    const MISSPELLABLE: i32 = 0;
    fn misspellable() {}
}

trait Trait {
    fn misspellable_trait() {}
}

impl Trait for Enum {
    fn misspellable_trait() {}
}

fn main() {
    Enum::mispellable(); // { dg-error ".E0599." "" { target *-*-* } }
    Enum::mispellable_trait(); // { dg-error ".E0599." "" { target *-*-* } }
    Enum::MISPELLABLE; // { dg-error ".E0599." "" { target *-*-* } }
}

