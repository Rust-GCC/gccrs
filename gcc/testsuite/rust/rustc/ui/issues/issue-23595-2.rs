#![feature(associated_type_defaults)]

pub struct C<AType: A> {a:AType}

pub trait A {
    type B = C<Self::anything_here_kills_it>;
// { dg-error ".E0220." "" { target *-*-* } .-1 }
}

fn main() {}

