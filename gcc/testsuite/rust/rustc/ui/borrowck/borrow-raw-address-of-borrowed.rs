#![feature(raw_ref_op)]

fn address_of_shared() {
    let mut x = 0;
    let y = &x;

    let q = &raw mut x;                 // { dg-error ".E0502." "" { target *-*-* } }

    drop(y);
}

fn address_of_mutably_borrowed() {
    let mut x = 0;
    let y = &mut x;

    let p = &raw const x;               // { dg-error ".E0502." "" { target *-*-* } }
    let q = &raw mut x;                 // { dg-error ".E0499." "" { target *-*-* } }

    drop(y);
}

fn main() {}

