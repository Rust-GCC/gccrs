fn main() {}

struct X;

impl X {
    const Y: u8; // { dg-error "" "" { target *-*-* } }
}

