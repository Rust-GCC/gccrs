// Parsing of range patterns

fn main() {
    let macropus!() ..= 11 = 12; // { dg-error "" "" { target *-*-* } }
}

