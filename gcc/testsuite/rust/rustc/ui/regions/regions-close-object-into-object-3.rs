#![feature(box_syntax)]
#![allow(warnings)]

trait A<T> { }
struct B<'a, T:'a>(&'a (A<T>+'a));

trait X { }
impl<'a, T> X for B<'a, T> {}

fn h<'a, T, U:'static>(v: Box<A<U>+'static>) -> Box<X+'static> {
    box B(&*v) as Box<X> // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {}

