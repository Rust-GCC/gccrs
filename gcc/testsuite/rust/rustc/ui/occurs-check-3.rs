// From Issue #778

enum Clam<T> { A(T) }
fn main() { let c; c = Clam::A(c); match c { Clam::A::<isize>(_) => { } } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

