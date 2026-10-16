type X = Vec<X>;
// { dg-error ".E0391." "" { target *-*-* } .-1 }

fn main() { let b: X = Vec::new(); }

