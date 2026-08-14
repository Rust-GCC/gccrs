#![feature(no_sanitize)]

#[no_sanitize(brontosaurus)] // { dg-error "" "" { target *-*-* } }
fn main() {
}

