pub trait Bound {}
pub struct Foo<T: Bound>(T);

pub trait Trait1 {}
impl<T: Bound> Trait1 for Foo<T> {}

pub trait Trait2 {}
impl<T> Trait2 for Foo<T> {} // { dg-error ".E0277." "" { target *-*-* } }

fn main() {}

