pub trait Arbitrary: Sized + 'static {}

impl<'a, A: Clone> Arbitrary for ::std::borrow::Cow<'a, A> {} // { dg-error ".E0478." "" { target *-*-* } }

fn main() {
}

