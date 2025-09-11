const fn mutable_ref_in_const() -> u8 {
    let mut a = 0;
    let b = &mut a; // { dg-error ".E0658." "" { target *-*-* } }
    *b
}

struct X;

impl X {
    const fn inherent_mutable_ref_in_const() -> u8 {
        let mut a = 0;
        let b = &mut a; // { dg-error ".E0658." "" { target *-*-* } }
        *b
    }
}

fn main() {}

