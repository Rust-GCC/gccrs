#![feature(unsize)]

use std::marker::Unsize;

pub trait CastTo<T: ?Sized>: Unsize<T> {
    fn cast_to(&self) -> &T;
}

impl<T: ?Sized, U: ?Sized + Unsize<T>> CastTo<T> for U {
    fn cast_to(&self) -> &T {
        self
    }
}

impl<T: ?Sized> Cast for T {}
pub trait Cast {
    fn cast<T: ?Sized>(&self) -> &T
    where
        Self: CastTo<T>,
    {
        self
    }
}

pub trait Foo: CastTo<[i32]> {}
impl Foo for [i32; 0] {}

fn main() {
    let x: &dyn Foo = &[];
    let x = x.cast::<[i32]>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

