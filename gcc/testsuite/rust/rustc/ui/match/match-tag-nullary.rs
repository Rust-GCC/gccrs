enum A { A }
enum B { B }

fn main() { let x: A = A::A; match x { B::B => { } } } // { dg-error ".E0308." "" { target *-*-* } }

