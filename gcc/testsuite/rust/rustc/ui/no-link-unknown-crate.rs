#[no_link]
extern crate doesnt_exist; // { dg-error ".E0463." "" { target *-*-* } }

fn main() {}

