fn main() {
    let x = 255u8;
    let 0u8..=x = 0;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

