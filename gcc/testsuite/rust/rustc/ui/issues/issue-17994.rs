trait Tr {}
type Huh<T> where T: Tr = isize; // { dg-error ".E0091." "" { target *-*-* } }
fn main() {}

