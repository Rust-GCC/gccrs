#[inline()] // { dg-error ".E0534." "" { target *-*-* } }
pub fn something() {}

fn main() {
    something();
}

