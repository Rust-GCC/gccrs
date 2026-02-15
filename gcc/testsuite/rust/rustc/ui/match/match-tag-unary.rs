enum A { A(isize) }
enum B { B(isize) }

fn main() { let x: A = A::A(0); match x { B::B(y) => { } } } // { dg-error ".E0308." "" { target *-*-* } }

