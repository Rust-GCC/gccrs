#![allow(mixed_script_confusables)]

fn foo<
    'β, // { dg-error ".E0658." "" { target *-*-* } }
    γ  // { dg-error ".E0658." "" { target *-*-* } }
// { dg-warning ".E0658." "" { target *-*-* } .-1 }
>() {}

struct X {
    δ: usize // { dg-error ".E0658." "" { target *-*-* } }
}

pub fn main() {
    let α = 0.00001f64; // { dg-error ".E0658." "" { target *-*-* } }
}

