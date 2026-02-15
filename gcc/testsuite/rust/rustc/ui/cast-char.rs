#![deny(overflowing_literals)]

fn main() {
    const XYZ: char = 0x1F888 as char;
// { dg-error "" "" { target *-*-* } .-1 }
    const XY: char = 129160 as char;
// { dg-error "" "" { target *-*-* } .-1 }
    const ZYX: char = '\u{01F888}';
    println!("{}", XYZ);
}

