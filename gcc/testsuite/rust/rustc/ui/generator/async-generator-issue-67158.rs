#![feature(generators)]
// edition:2018
// Regression test for #67158.
fn main() {
    async { yield print!(":C") }; // { dg-error ".E0727." "" { target *-*-* } }
}

