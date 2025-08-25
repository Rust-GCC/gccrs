#![feature(raw_ref_op)]

const fn mutable_address_of_in_const() {
    let mut a = 0;
    let b = &raw mut a;         // { dg-error ".E0658." "" { target *-*-* } }
}

struct X;

impl X {
    const fn inherent_mutable_address_of_in_const() {
        let mut a = 0;
        let b = &raw mut a;     // { dg-error ".E0658." "" { target *-*-* } }
    }
}

fn main() {}

