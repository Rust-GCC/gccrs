// compile-flags: --error-format human-annotate-rs

pub fn main() {
    let x: Iter; // { dg-error ".E0412." "" { target *-*-* } }
}

