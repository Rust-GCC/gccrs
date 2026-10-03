#![allow(dead_code)]

#[inline(please_no)] // { dg-error ".E0535." "" { target *-*-* } }
fn a() {
}

#[inline(please,no)] // { dg-error ".E0534." "" { target *-*-* } }
fn b() {
}

#[inline()] // { dg-error ".E0534." "" { target *-*-* } }
fn c() {
}

fn main() {
    a();
    b();
    c();
}

