pub trait Trait1<X> {
    type Output;
}

pub trait Trait2<X> {}

pub struct A;

impl<X, T> Trait1<X> for T where T: Trait2<X> {
    type Output = ();
}

impl<X> Trait1<Box<X>> for A {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
// { dg-error ".E0119." "" { target *-*-* } .-2 }
    type Output = i32;
}

fn main() {}

