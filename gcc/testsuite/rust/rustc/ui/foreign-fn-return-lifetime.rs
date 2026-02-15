// run-rustfix

extern "C" {
    pub fn g(_: &u8) -> &u8; // OK
    pub fn f() -> &u8; // { dg-error ".E0106." "" { target *-*-* } }
}

fn main() {}

