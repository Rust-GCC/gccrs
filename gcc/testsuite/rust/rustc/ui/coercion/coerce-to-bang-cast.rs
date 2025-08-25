#![feature(never_type)]

fn cast_a() {
    let y = {return; 22} as !;
// { dg-error ".E0605." "" { target *-*-* } .-1 }
}

fn cast_b() {
    let y = 22 as !; // { dg-error ".E0605." "" { target *-*-* } }
}

fn main() { }

