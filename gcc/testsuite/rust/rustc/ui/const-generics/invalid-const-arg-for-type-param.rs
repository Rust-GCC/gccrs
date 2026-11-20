use std::convert::TryInto;

struct S;

fn main() {
    let _: u32 = 5i32.try_into::<32>().unwrap(); // { dg-error ".E0107." "" { target *-*-* } }
    S.f::<0>(); // { dg-error ".E0599." "" { target *-*-* } }
    S::<0>; // { dg-error ".E0107." "" { target *-*-* } }
}

