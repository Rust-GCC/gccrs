enum Enum {
    X = (1 << 500), // { dg-error ".E0080." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    Y = (1 / 0) // { dg-error ".E0080." "" { target *-*-* } }
}

fn main() {
}

