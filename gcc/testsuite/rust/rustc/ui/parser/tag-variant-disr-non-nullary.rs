enum Color {
    Red = 0xff0000,
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    Green = 0x00ff00,
    Blue = 0x0000ff,
    Black = 0x000000,
    White = 0xffffff,
    Other(usize),
    Other2(usize, usize),
}

fn main() {}

