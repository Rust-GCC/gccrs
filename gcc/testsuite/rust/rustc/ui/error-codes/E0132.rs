#![feature(start)]

#[start]
fn f< T >() {} // { dg-error ".E0132." "" { target *-*-* } }

fn main() {
}

