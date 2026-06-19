#![feature(member_constraints)]
#![feature(type_alias_impl_trait)]

pub trait A {
    type B;
    fn f(&self) -> Self::B;
}
impl<'a, 'b> A for () {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
// { dg-error ".E0207." "" { target *-*-* } .-2 }
    type B = impl core::fmt::Debug;

    fn f(&self) -> Self::B {}
}

fn main() {}

