pub trait T<X, Y> {
    type A;
    type B;
    type C;
}
pub struct Foo {
    i: Box<dyn T<usize, usize, usize, usize, B=usize>>,
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
}


fn main() {}

