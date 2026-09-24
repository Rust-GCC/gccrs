struct Rgb(u8, u8, u8);

fn main() {
    let _ = Rgb { 0, 1, 2 };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
// { dg-error ".E0063." "" { target *-*-* } .-2 }
// { dg-error ".E0063." "" { target *-*-* } .-3 }
// { dg-error ".E0063." "" { target *-*-* } .-4 }
}

