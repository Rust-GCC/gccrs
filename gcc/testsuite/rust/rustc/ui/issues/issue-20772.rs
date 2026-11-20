trait T : Iterator<Item=Self::Item>
// { dg-error ".E0391." "" { target *-*-* } .-1 }
{}

fn main() {}

