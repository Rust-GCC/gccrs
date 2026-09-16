// Most traits cannot be derived for unions.

#[derive(
    PartialEq, // { dg-error "" "" { target *-*-* } }
    PartialOrd, // { dg-error "" "" { target *-*-* } }
    Ord, // { dg-error "" "" { target *-*-* } }
    Hash, // { dg-error "" "" { target *-*-* } }
    Default, // { dg-error "" "" { target *-*-* } }
    Debug, // { dg-error "" "" { target *-*-* } }
)]
union U {
    a: u8,
    b: u16,
}

fn main() {}

