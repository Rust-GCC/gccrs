#![feature(trait_alias)]

trait Foo {}
trait A<T: Foo> {}
trait B<T> = A<T>; // { dg-error ".E0277." "" { target *-*-* } }

fn main() {}

