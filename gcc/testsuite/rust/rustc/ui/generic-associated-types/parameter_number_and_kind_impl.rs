#![allow(incomplete_features)]
#![feature(generic_associated_types)]
#![feature(associated_type_defaults)]

// FIXME(#44265) add tests for type-generic and const-genertic associated types.

trait Foo {
    type A<'a>;
    type B<'a, 'b>;
    type C;
}

struct Fooy;

impl Foo for Fooy {
    type A = u32;
// { dg-error ".E0195." "" { target *-*-* } .-1 }
    type B<'a, T> = Vec<T>;
// { dg-error ".E0049." "" { target *-*-* } .-1 }
    type C<'a> = u32;
// { dg-error ".E0195." "" { target *-*-* } .-1 }
}

struct Fooer;

impl Foo for Fooer {
    type A<T> = u32;
// { dg-error ".E0049." "" { target *-*-* } .-1 }
    type B<'a> = u32;
// { dg-error ".E0195." "" { target *-*-* } .-1 }
    type C<T> = T;
// { dg-error ".E0049." "" { target *-*-* } .-1 }
}

fn main() {}

