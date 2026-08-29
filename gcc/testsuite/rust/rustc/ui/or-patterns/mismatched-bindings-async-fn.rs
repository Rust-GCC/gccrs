// Regression test for #71297
// edition:2018

#![feature(or_patterns)]

async fn a((x | s): String) {}
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }

async fn b() {
    let x | s = String::new();
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
}

fn main() {}

