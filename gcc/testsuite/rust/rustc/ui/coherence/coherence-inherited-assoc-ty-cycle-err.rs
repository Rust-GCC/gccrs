// Formerly this ICEd with the following message:
// Tried to project an inherited associated type during coherence checking,
// which is currently not supported.
//
// No we expect to run into a more user-friendly cycle error instead.
#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait Trait<T> { type Assoc; }
// { dg-error ".E0391." "" { target *-*-* } .-1 }

impl<T> Trait<T> for Vec<T> {
    type Assoc = ();
}

impl Trait<u8> for Vec<u8> {}

impl<T> Trait<T> for String {
    type Assoc = ();
}

impl Trait<<Vec<u8> as Trait<u8>>::Assoc> for String {}

fn main() {}

