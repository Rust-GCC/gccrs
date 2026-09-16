// Tests that enum-to-float casts are disallowed.

enum E {
    L0 = -1,
    H0 = 1
}

enum F {
    L1 = 1,
    H1 = 0xFFFFFFFFFFFFFFFF
}

pub fn main() {
    let a = E::L0 as f32;  // { dg-error ".E0606." "" { target *-*-* } }
    let c = F::H1 as f32;  // { dg-error ".E0606." "" { target *-*-* } }
    assert_eq!(a, -1.0f32);
    assert_eq!(c, -1.0f32);
}

