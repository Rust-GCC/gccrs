#[inline(unknown)] // { dg-error ".E0535." "" { target *-*-* } }
pub fn something() {}

fn main() {
    something();
}

