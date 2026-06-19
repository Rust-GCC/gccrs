fn main() {}

const fn slice(&[a, b]: &[i32]) -> i32 {
// { dg-error ".E0005." "" { target *-*-* } .-1 }
    a + b
}

