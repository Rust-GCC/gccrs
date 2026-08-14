enum X {
    A = 3,
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    B(usize)
}

fn main() {}

