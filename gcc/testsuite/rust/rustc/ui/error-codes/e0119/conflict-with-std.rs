use std::marker::PhantomData;
use std::convert::{TryFrom, AsRef};

struct Q;
impl AsRef<Q> for Box<Q> { // { dg-error ".E0119." "" { target *-*-* } }
    fn as_ref(&self) -> &Q {
        &**self
    }
}

struct S;
impl From<S> for S { // { dg-error ".E0119." "" { target *-*-* } }
    fn from(s: S) -> S {
        s
    }
}

struct X;
impl TryFrom<X> for X { // { dg-error ".E0119." "" { target *-*-* } }
    type Error = ();
    fn try_from(u: X) -> Result<X, ()> {
        Ok(u)
    }
}

fn main() {}

