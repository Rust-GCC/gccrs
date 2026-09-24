// `const`s shouldn't suggest `.into()`

const TEN: u8 = 10;
const TWELVE: u16 = TEN + 2;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

fn main() {
    const TEN: u8 = 10;
    const ALSO_TEN: u16 = TEN;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

