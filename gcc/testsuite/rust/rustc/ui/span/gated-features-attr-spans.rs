#[repr(simd)] // { dg-error ".E0658." "" { target *-*-* } }
struct Coord {
    x: u32,
    y: u32,
}

fn main() {}

