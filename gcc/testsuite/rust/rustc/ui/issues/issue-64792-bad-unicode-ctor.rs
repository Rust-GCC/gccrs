struct X {}

const Y: X = X("ö"); // { dg-error ".E0423." "" { target *-*-* } }

fn main() {}

