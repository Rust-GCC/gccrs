// rust-lang/rust#57979 : the initial support for `impl Trait` didn't
// properly check syntax hidden behind an associated type projection.
// Here we test behavior of occurrences of `impl Trait` within a path
// component in that context.

pub trait Bar { }
pub trait Quux<T> { type Assoc; }
pub fn demo(_: impl Quux<(), Assoc=<() as Quux<impl Bar>>::Assoc>) { }
// { dg-error ".E0667." "" { target *-*-* } .-1 }
impl<T> Quux<T> for () { type Assoc = u32; }

fn main() { }

