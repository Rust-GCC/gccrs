fn main() {
    static BUG: fn(_) -> u8 = |_| 8;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

