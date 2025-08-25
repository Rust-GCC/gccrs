#![feature(rustc_attrs)]

#[rustc_error]
fn main() {
// { dg-error "" "" { target *-*-* } .-1 }
}

