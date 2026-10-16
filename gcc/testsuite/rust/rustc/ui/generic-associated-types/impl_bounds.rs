#![allow(incomplete_features)]
#![feature(generic_associated_types)]
#![feature(associated_type_defaults)]

trait Foo {
    type A<'a> where Self: 'a;
    type B<'a, 'b> where 'a: 'b;
    type C where Self: Clone;
}

#[derive(Copy, Clone)]
struct Fooy<T>(T);

impl<T> Foo for Fooy<T> {
    type A<'a> where Self: 'static = (&'a ());
// { dg-error ".E0310." "" { target *-*-* } .-1 }
    type B<'a, 'b> where 'b: 'a = (&'a(), &'b ());
// { dg-error ".E0478." "" { target *-*-* } .-1 }
// { dg-error ".E0478." "" { target *-*-* } .-2 }
    type C where Self: Copy = String;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

