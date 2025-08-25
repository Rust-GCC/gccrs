const A: i32 = B; // { dg-error ".E0391." "" { target *-*-* } }

const B: i32 = A;

fn main() { }

