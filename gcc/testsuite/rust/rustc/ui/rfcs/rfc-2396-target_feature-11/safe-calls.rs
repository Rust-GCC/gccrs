// only-x86_64

#![feature(target_feature_11)]

#[target_feature(enable = "sse2")]
const fn sse2() {}

#[target_feature(enable = "avx")]
#[target_feature(enable = "bmi2")]
fn avx_bmi2() {}

struct Quux;

impl Quux {
    #[target_feature(enable = "avx")]
    #[target_feature(enable = "bmi2")]
    fn avx_bmi2(&self) {}
}

fn foo() {
    sse2();              // { dg-error ".E0133." "" { target *-*-* } }
    avx_bmi2();          // { dg-error ".E0133." "" { target *-*-* } }
    Quux.avx_bmi2();     // { dg-error ".E0133." "" { target *-*-* } }
}

#[target_feature(enable = "sse2")]
fn bar() {
    avx_bmi2();          // { dg-error ".E0133." "" { target *-*-* } }
    Quux.avx_bmi2();     // { dg-error ".E0133." "" { target *-*-* } }
}

#[target_feature(enable = "avx")]
fn baz() {
    sse2();              // { dg-error ".E0133." "" { target *-*-* } }
    avx_bmi2();          // { dg-error ".E0133." "" { target *-*-* } }
    Quux.avx_bmi2();     // { dg-error ".E0133." "" { target *-*-* } }
}

#[target_feature(enable = "avx")]
#[target_feature(enable = "bmi2")]
fn qux() {
    sse2();              // { dg-error ".E0133." "" { target *-*-* } }
}

const name: () = sse2(); // { dg-error ".E0133." "" { target *-*-* } }

fn main() {}

