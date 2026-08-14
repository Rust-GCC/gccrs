#![allow(incomplete_features)]
#![feature(generic_associated_types)]
#![feature(associated_type_defaults)]

trait Foo {
    type A<'a>;
    type B<'a, 'b>;
    type C;
    type D<T>;
    type E<'a, T>;
    // Test parameters in default values
    type FOk<T> = Self::E<'static, T>;
    type FErr1 = Self::E<'static, 'static>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
    type FErr2<T> = Self::E<'static, T, u32>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

fn main() {}

