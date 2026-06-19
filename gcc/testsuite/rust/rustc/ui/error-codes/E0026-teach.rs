// compile-flags: -Z teach

struct Thing {
    x: u32,
    y: u32
}

fn main() {
    let thing = Thing { x: 0, y: 0 };
    match thing {
        Thing { x, y, z } => {}
// { dg-error ".E0026." "" { target *-*-* } .-1 }
    }
}

